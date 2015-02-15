#include "CG_TextBox.h"

#include "GLFW/glfw3.h"
//#include "ofEvents.h"

#define TEXT_CLIPPING_ADDITIONAL_PIXELS 3
#define CURSOR_ADDITIONAL_X_OFFSET 2

#define CURSOR_BLINK_DURATION 500
#define FRAMES_PER_CURSOR_BLINK 30

namespace CG {

	TextBox::TextBox(void) :
		_cursorDisplayedFrames(0),
		_cursorVisible(false),
		_clearFieldOnActivation(false),
		_linesOfText(1),
		_cursorXOffset(0)
	{

		ofSetEscapeQuitsApp(false); //esc (is/can be) used to cancel text entry.

		_displayInfo.offset = 0;
		_displayInfo.length = 0;

		_highlightedChars.offset = 0;
		_highlightedChars.length = 0;

		colors["disabled"] = ofColor(127);
	}

	TextBox::~TextBox(void) {
	}

	void TextBox::draw(void) {

		int tempHeight = (_lineHeight * _linesOfText) + (edgeWidth * 2);

		ofSetColor(colors["edge"]);
		ofRect(boundingBox);

		if (isEnabled()) {
			ofSetColor(colors["background"]);
		} else {
			ofSetColor(colors["disabled"]);
		}
		ofRect(boundingBox.x + edgeWidth, boundingBox.y + edgeWidth, boundingBox.width - 2 * edgeWidth, boundingBox.height - 2 * edgeWidth);

		if (_currentEntry != "") {
			int hlOff = _highlightedChars.offset;
			if (_highlightedChars.offset < _displayInfo.offset) {
				hlOff = _displayInfo.offset;
			}
			int hlLength = _highlightedChars.length;
			if (hlOff + hlLength > _displayInfo.offset + _displayInfo.length) {
				hlLength = _displayInfo.offset + _displayInfo.length - hlOff;
			}

			int highlightStartX = _calculateCharX(hlOff);
			int highlightEndX = _calculateCharX(hlOff + hlLength);

			ofSetColor(colors["hovered"]);
			ofRect(boundingBox.x + highlightStartX, boundingBox.y + edgeWidth * 2, highlightEndX - highlightStartX, boundingBox.height - 4 * edgeWidth);


			ofSetColor(colors["text"]);
			this->drawString(_currentEntry.substr(_displayInfo.offset, _displayInfo.length),
							 boundingBox.x + edgeWidth, boundingBox.y + (_lineHeight * _linesOfText) - edgeWidth);
		}

		if (isActive()) {
			if (++_cursorDisplayedFrames > FRAMES_PER_CURSOR_BLINK) {
				_cursorDisplayedFrames = 0;
				_cursorVisible = !_cursorVisible;
			}
			if (_cursorVisible) {
				ofSetColor(colors["edge"]);
				ofLine(boundingBox.x + edgeWidth + _cursorXOffset, boundingBox.y + (_lineHeight * (_linesOfText - 1)) + edgeWidth + 1,
					   boundingBox.x + edgeWidth + _cursorXOffset, boundingBox.y + (_lineHeight * _linesOfText));
			}
		}

	}

	CG::InteractionResult TextBox::mouseButtonEvent(const MouseButtonEvent &ev) {
		CG::InteractionResult result;

		if ((ev.button != 0) || (ev.eventType != MouseButtonEvent::PRESSED)) {
			return result;
		}

		if (this->inside(ev.x, ev.y)) {
			_activate();

			_setCursorFromClick(ofPoint(ev.x, ev.y));
		} else {
			_submitEntry();

			result.newDataAvailable = true;
			result.contolStateChanged = true;
			result.newStateLabel = _currentEntry;

			notifyCallback();
		}
		return result;
	}

	CG::InteractionResult TextBox::mouseMovedEvent(int x, int y) {
		if (this->inside(x, y)) {
			setHoveredState(true);
			if (isEnabled()) {
				setCursor(UIC_TEXT_SELECT);
			}
		} else {
			setHoveredState(false);
		}
		return CG::InteractionResult();
	}

	CG::InteractionResult TextBox::keyEvent(KeyEvent ev) {
		CG::InteractionResult result;

		if (!isActive() || !isEnabled() || (ev.eventType == KeyEventType::RELEASED)) {
			return result;
		}

		switch (ev.key) {
		case OF_KEY_RETURN:
			_submitEntry();

			result.contolStateChanged = true;
			result.newDataAvailable = true;
			result.newStateLabel = _currentEntry;

			_clearHighlight();
			break;

		case OF_KEY_LEFT:
			if (_cursorCharacterIndex > 0) {
				--_cursorCharacterIndex;
			}
			_setDisplayedString(_currentEntry);
			_clearHighlight();
			break;
		case OF_KEY_RIGHT:
			if (_cursorCharacterIndex < _currentEntry.size()) {
				++_cursorCharacterIndex;
			}
			_setDisplayedString(_currentEntry);
			_clearHighlight();
			break;
		case OF_KEY_BACKSPACE:


			if (_highlightedChars.length == 0) {
				if ((_cursorCharacterIndex > 0) && (_currentEntry.size() >= _cursorCharacterIndex - 1)) {
					_currentEntry.erase(_currentEntry.begin() + _cursorCharacterIndex - 1);
					--_cursorCharacterIndex;
				}
				_setDisplayedString(_currentEntry);
			} else {

				string s = "";
				int start = 0;
				int count = _highlightedChars.offset - start;
				s = _currentEntry.substr(start, count);
				start = _highlightedChars.offset + _highlightedChars.length - 1;
				count = _currentEntry.length() - (_highlightedChars.offset + _highlightedChars.length);
				s.append(_currentEntry.substr(start, count));
				_currentEntry = s;

				_setDisplayedString(_currentEntry);

				_cursorCharacterIndex = _highlightedChars.offset;

				_clearHighlight(); //TODO: Delete multiple chars
			}
			break;
		case OF_KEY_DEL:
			if (_currentEntry.size() >= _cursorCharacterIndex) {
				_currentEntry.erase(_currentEntry.begin() + _cursorCharacterIndex);
			}
			_setDisplayedString(_currentEntry);
			_clearHighlight(); //TODO: Delete multiple chars
			break;
		case OF_KEY_ESC:
			_cancelEntry();
			_clearHighlight();
			break;

		default:
			if (ev.key >= OF_KEY_MODIFIER) {
				break;
			}

			//Select all shortcut
			if (ev.heldModifiers.ctrl && ((ev.key == 'a') || (ev.key == 'A'))) {
				_highlightChars(0, _currentEntry.length());
				break;
			}

			if (ev.heldModifiers.shift && isShiftableChar(ev.key)) {
				ev.key = getShiftedChar(ev.key);
			}

			//Handle numpad numbers
			if (ev.key >= GLFW_KEY_KP_0 && ev.key <= GLFW_KEY_KP_9) {
				ev.key = (ev.key - GLFW_KEY_KP_0) + '0';
			}

			if (_highlightedChars.length > 0) {
				_currentEntry.erase(_highlightedChars.offset, _highlightedChars.length);
				_cursorCharacterIndex = _highlightedChars.offset;
			}

			_currentEntry.insert(_currentEntry.begin() + _cursorCharacterIndex, (char)ev.key);
			++_cursorCharacterIndex;
			_setDisplayedString(_currentEntry);
			_clearHighlight(); //TODO: Overwrite highlight
			break;

		}

		_calculateCursorOffset();

		//cout << "Entry length " << _currentEntry.length() << endl;

		notifyControlNeedsRedraw();

		if (result.contolStateChanged) {
			notifyCallback();
		}

		return result;
	}


	void TextBox::_setDisplayedString(string s) {

		if (textFont->stringWidth(s) < boundingBox.width - (edgeWidth * 2) - TEXT_CLIPPING_ADDITIONAL_PIXELS) {
			//_displayedString = s;
			_displayInfo.offset = 0;
			_displayInfo.length = s.size();
		} else {
			//If the cursor is to the left of the offset (not displayed), then
			if (_cursorCharacterIndex < _displayInfo.offset) {
				_displayInfo.offset = _cursorCharacterIndex - 3;

				if (_displayInfo.offset < 0) {
					_displayInfo.offset = 0;
				}
				//cout << "Offset decreased to " << _displayInfo.offset << endl;
			}

			//If the string cannot be displayed with the cursor because the cursor is out of the displayed area, then move so that the
			//offset is just to the left of the cursor.
			if (textFont->stringWidth(s.substr(_displayInfo.offset, _cursorCharacterIndex - _displayInfo.offset)) >
				(boundingBox.width - (edgeWidth * 2) - TEXT_CLIPPING_ADDITIONAL_PIXELS))
			{
				_displayInfo.offset = _cursorCharacterIndex - 3;

				if (_displayInfo.offset > s.size() - 1) {
					_displayInfo.offset = s.size() - 1;
				}
				//cout << "Offset increased to " << _displayInfo.offset << endl;
			}

			for (_displayInfo.length = s.size(); _displayInfo.length > 0; _displayInfo.length--) {

				if (_displayInfo.offset + _displayInfo.length > s.size()) {
					_displayInfo.length = s.size() - _displayInfo.offset;
				}

				float tempStringWidth = textFont->stringWidth(s.substr(_displayInfo.offset, _displayInfo.length));

				if (tempStringWidth <= boundingBox.width - (edgeWidth * 2) - TEXT_CLIPPING_ADDITIONAL_PIXELS) {
					break;
				}
			}
		}

		this->notifyControlNeedsRedraw();
	}


	void TextBox::setFont(ofTrueTypeFont *font) {
		textFont = font;
		textFont->setSpaceSize(1);

		_lineHeight = ceil(textFont->getSize() * 1.5); //TODO: Make this work with font sizes other than 12
		//boundingBox.height = textFont->getSize() + 6;

		notifyControlNeedsRedraw();
	}

	void TextBox::setLabel(string label) {
		_currentEntry = label;
		_lastEntry = label;
		_setDisplayedString(label);
	}


	void TextBox::_submitEntry(void) {
		_lastEntry = _currentEntry;
		_setDisplayedString(_currentEntry);
		_deactivate();
	}

	void TextBox::_cancelEntry(void) {
		_currentEntry = _lastEntry;
		_setDisplayedString(_lastEntry);
		_deactivate();
	}

	void TextBox::_activate(void) {
		if (!isEnabled() || isActive()) {
			return;
		}

		setActiveState(true);
		_cursorDisplayedFrames = 0;
		_cursorCharacterIndex = 0;
		_displayInfo.offset = 0;
		_displayInfo.length = 0;
		_calculateCursorOffset();

		if (_clearFieldOnActivation) {
			_currentEntry = "";
			_setDisplayedString("");
		} else {
			_currentEntry = _lastEntry;
			_setDisplayedString(_lastEntry);
		}

		//controlVisible = true;
	}

	void TextBox::_deactivate(void) {
		setActiveState(false);
		_setDisplayedString(_lastEntry);
	}

	void TextBox::_calculateCursorOffset(void) {
		float widthFromDisplayStart = CURSOR_ADDITIONAL_X_OFFSET;

		if (_currentEntry.size() > 0) {
			string tempStr = _currentEntry.substr(_displayInfo.offset, _cursorCharacterIndex - _displayInfo.offset);
			widthFromDisplayStart += getStringBoundingBox(tempStr).width;
			if ((tempStr.size() > 0) && (tempStr.back() == ' ')) {
				widthFromDisplayStart += getStringBoundingBox("_").width;
			}
		}

		_cursorXOffset = widthFromDisplayStart;
	}

	float TextBox::_calculateCharX(int charIndex) {
		float widthFromDisplayStart = CURSOR_ADDITIONAL_X_OFFSET;

		if (_currentEntry.size() > 0) {
			string tempStr = _currentEntry.substr(_displayInfo.offset, charIndex - _displayInfo.offset);
			widthFromDisplayStart += getStringBoundingBox(tempStr).width;
		}

		return widthFromDisplayStart;
	}

	void TextBox::_highlightChars(int offset, int length) {
		_highlightedChars.offset = offset;
		_highlightedChars.length = length;
	}

	void TextBox::_clearHighlight(void) {
		_highlightedChars.offset = 0;
		_highlightedChars.length = 0;
	}

	void TextBox::_setCursorFromClick(ofPoint clickLocation) {

		int distanceFromStringStart = clickLocation.x - (boundingBox.x + edgeWidth);

		//cout << "Distance from start " << distanceFromStringStart << endl;

		string s = _currentEntry.substr(_displayInfo.offset, _displayInfo.length);

		//cout << "The substring is: " << s << endl;

		int prevCharDist;
		int nextCharDist;
		int prevCharOffset = -1;

		for (int i = 1; i <= s.size(); i++) {
			int currentWidth = getStringBoundingBox(s.substr(0, i)).width;

			//cout << "Current width " << currentWidth << endl;

			if (currentWidth > distanceFromStringStart) {
				nextCharDist = currentWidth;
				prevCharDist = getStringBoundingBox(s.substr(0, i - 1)).width;

				prevCharOffset = i - 1;

				//cout << "Next char distance: " << nextCharDist << endl <<
				//	"Prev char distance: " << prevCharDist << endl <<
				//	"Prev char offset: " << prevCharOffset << endl;
				break;
			}
		}

		if (prevCharOffset != -1) {

			if (abs(prevCharDist - distanceFromStringStart) < abs(nextCharDist - distanceFromStringStart)) {
				_cursorCharacterIndex = _displayInfo.offset + prevCharOffset;
			} else {
				_cursorCharacterIndex = _displayInfo.offset + prevCharOffset + 1;
			}

		} else {

			int fullStringWidth = getStringBoundingBox(s).width;
			if (fullStringWidth < distanceFromStringStart) {
				_cursorCharacterIndex = _displayInfo.offset + _displayInfo.length;
			} else {
				_cursorCharacterIndex = _displayInfo.offset;
			}

		}

		//cout << "Cursor char index is now " << _cursorCharacterIndex << endl;

		_calculateCursorOffset();

		//Keep the cursor visible for the full interval if it is currently visible. Otherwise, make it become visible immediately.
		if (_cursorVisible) {
			_cursorDisplayedFrames = 0;
		} else {
			_cursorDisplayedFrames = FRAMES_PER_CURSOR_BLINK + 1;
			_cursorVisible = false;
		}

		_clearHighlight();
	}

}