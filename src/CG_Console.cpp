#include "CG_Console.h"

namespace CG {

	Console::Console(void) {
		_lineStrings.resize(1);
		_lineStrings.at(0) = "";
		_lastChar = '\0';

		_currentLineBlank = true;

		//int lineHeight = getStringBoundingBox("Ag").height; How it might be calculated.
		//int lineHeight = textFont->getSize() + 5; //How it is calculated elsewhere.

		//setMaxVisibleLines( getBoundingBox().height/lineHeight );
		//setMaxLineLength(60);
		_maxVisibleLines = 1;
		//setMaxVisibleLines(1);
	}

	void Console::setFont(ofTrueTypeFont *font) {
		textFont = font;

		int lineHeight = textFont->getSize() + 5;
		setMaxVisibleLines(getBoundingBox().height / lineHeight);
	}

	void Console::setBoundingBox(ofRectangle bb) {
		boundingBox = bb;

		int lineHeight = textFont->getSize() + 5;
		setMaxVisibleLines(getBoundingBox().height / lineHeight);
	}

	void Console::setMaxVisibleLines(int lines) {
		_maxVisibleLines = lines;
		notifyControlNeedsRedraw();
	}

	void Console::setMaxLineLength(int characters) {
		_maxLineLength = characters;
		notifyControlNeedsRedraw();
	}

	//Shouldn't these be based on OF standards?
#define BACKSPACE_CHAR 0x08
#define DELETE_CHAR 0x7F

	void Console::handleChar(char c) {
		if (c == '\n' || c == '\r') {
			if (_lastChar == '\n' || _lastChar == '\r') {
				//If the last char was a part of CRLF, don't do anything with this char.
			} else {
				//Handle EOL
				if (!_currentLineBlank) {
					_currentLineBlank = true;

					while (_lineStrings.size() > _maxVisibleLines) {
						_lineStrings.pop_front();
					}
				}
			}
		} else if (c == BACKSPACE_CHAR || c == DELETE_CHAR) {
			if (!_currentLineBlank && _lineStrings.back().size() > 0) {
				_lineStrings.back().erase(_lineStrings.back().end() - 1);
			}
			return;
		} else if (c < ' ' || c > '~') {
			//Non-printable/non-standard character, ignore.
			return;
		} else {
			//Printable char.

			if (_currentLineBlank) {
				_lineStrings.push_back("");
				while (_lineStrings.size() > _maxVisibleLines) {
					_lineStrings.pop_front();
				}
				_currentLineBlank = false;
			}

			string s = _lineStrings.back() + c;

			ofRectangle sbb = getStringBoundingBox(s, 0, 0);

			//If the string is too wide to be displayed, wrap it (hard wrap).
			//For display purposes, it works just fine to insert a newline ('\n').
			if (sbb.width > (boundingBox.width - 20)) {
				_lineStrings.push_back("   ");
				_lineStrings.back() += c;
			} else {
				_lineStrings.back() = s;
			}

		}
		_lastChar = c;
		notifyControlNeedsRedraw();
	}

	void Console::handleString(string s) {
		for (int i = 0; i < s.size(); i++) {
			handleChar(s[i]);
		}
	}

	void Console::handleString(char *s) {
		while (*s) {
			handleChar(*s++);
		}
	}

	void Console::draw(void) {
		ofSetColor(colors["edge"]);
		ofRect(boundingBox);

		ofSetColor(colors["background"]);
		ofRect(boundingBox.x + edgeWidth, boundingBox.y + edgeWidth, boundingBox.width - edgeWidth * 2, boundingBox.height - edgeWidth * 2);

		ofSetColor(colors["text"]);

		int location = 0;
		for (int i = 0; i < _lineStrings.size(); i++) {
			drawString(_lineStrings[i], boundingBox.x + 10, boundingBox.y + (textFont->getSize() + 5) * (++location));
		}
	}

	void Console::clear(void) {
		_lineStrings.clear();
		notifyControlNeedsRedraw();
	}

}