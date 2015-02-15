#include "CG_CheckBox.h"

namespace CG {

	CheckBox::CheckBox() :
		_checkMarkType(CheckTypes::CHECK),
		_relativeLabelLocation(1, 0),
		_checked(false),
		_usingRelativeLocation(true)
	{
		colors["checkedBackground"] = colors["background"];
	}

	void CheckBox::draw(void) {
		ofSetColor(colors["edge"]);
		ofRect(boundingBox);

		if (!isEnabled()) {
			ofSetColor(colors["disabled"]);
		} else if (isHovered() && _checked) {
			ofSetColor(colors["hovered"].getLerped(colors["checkedBackground"], .5));
		} else if (isHovered()) {
			ofSetColor(colors["hovered"]);
		} else if (_checked) {
			ofSetColor(colors["checkedBackground"]);
		} else {
			ofSetColor(colors["background"]);
		}
		ofRect(shrinkRectangle(boundingBox, edgeWidth));

		ofSetColor(colors["edge"]);
		if (_checked) {
			switch (_checkMarkType) {
			case CheckTypes::X:
				ofLine(boundingBox.x, boundingBox.y, boundingBox.x + boundingBox.width, boundingBox.y + boundingBox.height);
				ofLine(boundingBox.x, boundingBox.y + boundingBox.height, boundingBox.x + boundingBox.width, boundingBox.y);
				break;
			case CheckTypes::NO_MARK:
				break;
			case CheckTypes::CHECK:
				ofLine(boundingBox.x + boundingBox.width / 8, boundingBox.y + boundingBox.height / 2,
					   boundingBox.x + boundingBox.width * 3 / 8, boundingBox.y + boundingBox.height * 7 / 8);
				ofLine(boundingBox.x + boundingBox.width * 3 / 8, boundingBox.y + boundingBox.height * 7 / 8,
					   boundingBox.x + boundingBox.width * 7 / 8, boundingBox.y + boundingBox.height / 8);
				break;
			}
		}

		ofSetColor(colors["text"]);
		this->drawString(_label, _labelLocation.x, _labelLocation.y);
	}

	CG::InteractionResult CheckBox::mouseButtonEvent(const MouseButtonEvent &ev) {

		CG::InteractionResult result;

		if ((ev.eventType == MouseButtonEvent::PRESSED) && (boundingBox.inside(ev.x, ev.y))) {
			_checked = !_checked;
			result.newDataAvailable = true;
			result.newControlState = _checked ? 1 : 0;

			notifyControlNeedsRedraw();
			notifyCallback();
		}

		return result;
	}

	void CheckBox::setLabel(string label) {
		_label = label;
		if (_usingRelativeLocation) {
			setLabelLocation(_relativeLabelLocation.x, _relativeLabelLocation.y);
		}
		notifyControlNeedsRedraw();
	}

	void CheckBox::setLabelLocation(ofPoint location) {
		_usingRelativeLocation = false;

		_labelLocation.x = floor(location.x);
		_labelLocation.y = floor(location.y);
	}

	void CheckBox::setLabelLocation(int relativeX, int relativeY) {
		_usingRelativeLocation = true;

		ofRectangle labelBox = this->getLabelBoundingBox();
		ofPoint newLabelLocation;

		_relativeLabelLocation = ofPoint(relativeX, relativeY);

		if (relativeX < 0) {
			newLabelLocation.x = boundingBox.x - labelBox.width - 5;
		} else if (relativeX > 0) {
			newLabelLocation.x = boundingBox.x + boundingBox.width + 5;
		} else {
			newLabelLocation.x = boundingBox.getCenter().x - labelBox.width / 2;
		}

		if (relativeY < 0) {
			newLabelLocation.y = boundingBox.y - 2;
		} else if (relativeY > 0) {
			newLabelLocation.y = boundingBox.y + boundingBox.height + labelBox.height + 2;
		} else {
			newLabelLocation.y = boundingBox.y + boundingBox.height / 2 + labelBox.height / 2;
		}

		_labelLocation.x = floor(newLabelLocation.x);
		_labelLocation.y = floor(newLabelLocation.y);
	}

	ofRectangle CheckBox::getLabelBoundingBox(void) {
		ofRectangle tempRect(_labelLocation.x, _labelLocation.y, 0, 0);
		if (_label != "") {
			tempRect = getStringBoundingBox(_label, _labelLocation.x, _labelLocation.y);
		}
		return tempRect;
	}

	ofRectangle CheckBox::getCompleteBoundingBox(void) {
		ofRectangle temp = getLabelBoundingBox();
		temp.growToInclude(boundingBox);
		return temp;
	}

	void CheckBox::setBoundingBox(ofRectangle boundingBox_) {
		boundingBox = boundingBox_;
		if (_usingRelativeLocation) {
			setLabelLocation(_relativeLabelLocation.x, _relativeLabelLocation.y);
		}
		notifyControlNeedsRedraw();
	}

	void CheckBox::setFont(ofTrueTypeFont *font) {
		textFont = font;
		if (_usingRelativeLocation) {
			setLabelLocation(_relativeLabelLocation.x, _relativeLabelLocation.y);
		}
		notifyControlNeedsRedraw();
	}

}