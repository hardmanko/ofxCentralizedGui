#include "CG_Dropdown.h"

namespace CG {

#define NOTHING_SELECTED (numeric_limits<int>::min())
#define DROPDOWN_ARROW_BOX_WIDTH 15
#define TEXT_HORIZ_SPACING 2

	Dropdown::Dropdown(void) :
		_currentSelectionIndex(NOTHING_SELECTED),
		_highlightedSelectionIndex(NOTHING_SELECTED)
	{
		setActiveState(false);
	}

	void Dropdown::draw(void) {

		ofSetColor(colors["edge"]);
		ofRect(boundingBox);

		if (isEnabled()) {
			ofSetColor(colors["background"]);
		} else {
			ofSetColor(colors["disabled"]);
		}
		ofRect(boundingBox.x + edgeWidth, boundingBox.y + edgeWidth, boundingBox.width - (edgeWidth * 2), boundingBox.height - (edgeWidth * 2));

		if (isActive()) {

			ofSetColor(colors["edge"]);
			ofRect(_dropdownBoxPosition);
			ofSetColor(colors["background"]);
			ofRect(_dropdownBoxPosition.x + edgeWidth, _dropdownBoxPosition.y + edgeWidth,
				   _dropdownBoxPosition.width - (edgeWidth * 2), _dropdownBoxPosition.height - (edgeWidth * 2)
				   );


			//Draw the highlight.
			if (_highlightedSelectionIndex != NOTHING_SELECTED) {
				ofSetColor(colors["hovered"]);
				ofRect(_dropdownBoxPosition.x + edgeWidth,
					   _dropdownBoxPosition.y + edgeWidth + (_highlightedSelectionIndex * _selectionLineHeight),
					   _dropdownBoxPosition.width - (edgeWidth * 2),
					   _selectionLineHeight
					   );
			}

			//Draw each selection in the dropdown
			ofSetColor(colors["text"]);
			for (size_t i = 0; i < _selectionOptions.size(); ++i) {
				this->drawString(_selectionOptions.at(i).label,
								 _dropdownBoxPosition.x + edgeWidth + TEXT_HORIZ_SPACING,
								 _dropdownBoxPosition.y + ((i + 1) * _selectionLineHeight)
								 );
			}

		} else {
			//Draw the prompt that is shown in the dropdown box.
			ofSetColor(colors["text"]);
			this->drawString(_displayedText,
							 boundingBox.x + edgeWidth + TEXT_HORIZ_SPACING,
							 boundingBox.y + _selectionLineHeight*.8
							 );
		}

		ofSetColor(colors["edge"]);
		ofRect(_dropdownArrowPosition);

		if (isHovered()) {
			ofSetColor(colors["hovered"]);
		} else {
			ofSetColor(colors["edge"]);
		}
		ofRect(_dropdownArrowPosition.x + edgeWidth, _dropdownArrowPosition.y + edgeWidth,
			   _dropdownArrowPosition.width - (2 * edgeWidth), _dropdownArrowPosition.height - (2 * edgeWidth));

		ofSetLineWidth(1);
		ofSetColor(colors["background"]);
		ofPoint arrowCenter = _dropdownArrowPosition.getCenter();
		ofLine(arrowCenter.x, arrowCenter.y + 2, arrowCenter.x - 5, arrowCenter.y - 3);
		ofLine(arrowCenter.x, arrowCenter.y + 2, arrowCenter.x + 5, arrowCenter.y - 3);

	}


	void Dropdown::addSelection(int id, string label) {
		Dropdown::Selection temp;
		temp.id = id;
		temp.label = label;
		_selectionOptions.push_back(temp);

		_recalculateDropdownBoxSize();
		notifyControlNeedsRedraw();
	}

	void Dropdown::addSelection(std::string label) {
		int id = 0;
		if (!_selectionOptions.empty()) {
			for (auto sel : _selectionOptions) {
				id = std::max(id, sel.id);
			}
			id++;
		}
		addSelection(id, label);
	}

	void Dropdown::clearSelectionOptions(void) {
		_selectionOptions.clear();
		_recalculateDropdownBoxSize();
		notifyControlNeedsRedraw();
	}



	int Dropdown::_getSelectionLineAtLocation(int mouseX, int mouseY) {
		for (size_t currentLine = 0; currentLine < _selectionOptions.size(); currentLine++) {
			if (mouseX > _dropdownBoxPosition.x &&
				mouseX < _dropdownBoxPosition.x + _dropdownBoxPosition.width &&
				mouseY > _dropdownBoxPosition.y + (currentLine * _selectionLineHeight) &&
				mouseY < _dropdownBoxPosition.y + ((currentLine + 1) * _selectionLineHeight))
			{
				return currentLine;
			}
		}
		return NOTHING_SELECTED;
	}

	CG::InteractionResult Dropdown::mouseMovedEvent(int x, int y) {
		if (this->inside(x, y)) {
			setHoveredState(true);
		} else {
			setHoveredState(false);
		}
		if (isActive()) {
			_highlightedSelectionIndex = _getSelectionLineAtLocation(x, y);
			notifyControlNeedsRedraw();
		}
		return CG::InteractionResult();
	}

	CG::InteractionResult Dropdown::mouseButtonEvent(const MouseButtonEvent &ev) {
		CG::InteractionResult result;

		if (ev.eventType != MouseButtonEvent::PRESSED || ev.button != 0) {
			return result;
		}

		if (isActive()) {
			setActiveState(false);

			result.contolStateChanged = true;

			int currentSelection = _getSelectionLineAtLocation(ev.x, ev.y);

			if (currentSelection != NOTHING_SELECTED) {
				result.newDataAvailable = true;
				result.newControlState = _selectionOptions.at(currentSelection).id;
				result.newStateLabel = _selectionOptions.at(currentSelection).label;

				setSelectedOption(currentSelection);

				notifyCallback();
			}

		} else {
			if (boundingBox.inside(ev.x, ev.y)) {
				setActiveState(true);
				result.contolStateChanged = true;
			}
		}

		return result;
	}

	void Dropdown::setSelectedOption(int index) {
		if (index < 0 || size_t(index) >= _selectionOptions.size()) {
			_currentSelectionIndex = NOTHING_SELECTED;
			_displayedText = _noSelectionText;
			return;
		}

		if (index == _currentSelectionIndex) {
			return;
		}

		_currentSelectionIndex = index;
		_displayedText = _selectionOptions.at(index).label;

		if (textFont->isLoaded()) {
			while (getStringBoundingBox(_displayedText).width > boundingBox.width) {
				_displayedText = _displayedText.substr(0, _displayedText.size() - 1);
			}
		}

		notifyControlNeedsRedraw();
	}

	bool Dropdown::inside(float x, float y) const {
		bool inside = false;
		inside = inside || boundingBox.inside(x, y);
		if (isActive()) {
			inside = inside || _dropdownBoxPosition.inside(x, y);
		}
		return inside;
	}

	void Dropdown::setSelectedOptionById(int id) {
		for (size_t i = 0; i < _selectionOptions.size(); i++) {
			if (_selectionOptions[i].id == id) {
				setSelectedOption(int(i));
				break;
			}
		}
		this->notifyControlNeedsRedraw();
	}

	void Dropdown::setSelectedOption(std::string selectionLabel) {
		for (size_t i = 0; i < _selectionOptions.size(); i++) {
			if (_selectionOptions[i].label == selectionLabel) {
				setSelectedOption(int(i));
				break;
			}
		}
		this->notifyControlNeedsRedraw();
	}

	void Dropdown::setToNothingSelected(void) {
		setSelectedOption(NOTHING_SELECTED);
	}

	/*! This function does not change the current selection, it just changes what prompt is current shown. */
	void Dropdown::setDisplayedText(string text) {
		_displayedText = text;
		if (textFont->isLoaded()) {
			while (getStringBoundingBox(_displayedText).width > boundingBox.width) {
				_displayedText = _displayedText.substr(0, _displayedText.size() - 1);
			}
		}
		notifyControlNeedsRedraw();
	}

	string Dropdown::getSelectionLabel(int id) {
		if (id == NOTHING_SELECTED) {
			return "Error: Nothing selected";
		}

		for (size_t i = 0; i < _selectionOptions.size(); ++i) {
			if (_selectionOptions.at(i).id == id) {
				return _selectionOptions.at(i).label;
			}
		}
		return "Error: Id not found";
	}

	string Dropdown::getCurrentSelectionLabel(void) {
		return getSelectionLabel(getCurrentSelectionId());
	}

	int Dropdown::getCurrentSelectionId(void) {
		if (_currentSelectionIndex == NOTHING_SELECTED) {
			return NOTHING_SELECTED;
		}

		return _selectionOptions.at(_currentSelectionIndex).id; 
	};

	void Dropdown::setBoundingBox(ofRectangle controlPosition) {
		boundingBox = controlPosition;

		_dropdownArrowPosition.x = boundingBox.x + boundingBox.width - DROPDOWN_ARROW_BOX_WIDTH;
		_dropdownArrowPosition.y = boundingBox.y;
		_dropdownArrowPosition.width = DROPDOWN_ARROW_BOX_WIDTH;
		_dropdownArrowPosition.height = boundingBox.height;

		_dropdownBoxPosition.x = boundingBox.x;
		_dropdownBoxPosition.y = boundingBox.y + boundingBox.height;

		notifyControlNeedsRedraw();
	}


	void Dropdown::setFont(ofTrueTypeFont *font) {
		textFont = font;
		_recalculateDropdownBoxSize();
		notifyControlNeedsRedraw();
	}

	void Dropdown::setNothingSelectedText(string text) {
		_noSelectionText = text;
		setSelectedOption(_currentSelectionIndex);
		notifyControlNeedsRedraw();
	}

	void Dropdown::_recalculateDropdownBoxSize(void) {
		if (textFont == nullptr) {
			return;
		}

		int availableSelections = _selectionOptions.size();

		_selectionLineHeight = textFont->getSize() * 1.5;
		_dropdownBoxPosition.height = availableSelections * _selectionLineHeight + (edgeWidth * 2);

		float maxWidth = 0;
		for (size_t i = 0; i < _selectionOptions.size(); i++) {
			if (textFont->stringWidth(_selectionOptions[i].label) > maxWidth) {
				maxWidth = textFont->stringWidth(_selectionOptions[i].label);
			}
		}

		_dropdownBoxPosition.width = maxWidth + ((edgeWidth + TEXT_HORIZ_SPACING) * 2);
	}

}