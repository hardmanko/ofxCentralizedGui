#include "CG_Slider.h"

namespace CG {

	Slider::Slider(void) :
		_sliderPos(0),
		_sliderThickness(10),
		_targetMouseButton(0),
		_mouseButtonHeld(false)
	{}

	void Slider::draw(void) {
		ofRectangle bb = this->getBoundingBox();

		ofSetColor(colors["edge"]);
		ofRect(bb);

		ofRectangle innerRect = shrinkRectangle(bb, edgeWidth);
		ofSetColor(colors["background"]);
		ofRect(innerRect);

		ofRectangle sliderBB;
		if (this->_isHorizontal()) {
			sliderBB = ofRectangle(innerRect.x + _sliderPos, innerRect.y + 1, _sliderThickness, innerRect.height - 2);
		} else {
			sliderBB = ofRectangle(innerRect.x + 1, innerRect.y + _sliderPos, innerRect.width - 2, _sliderThickness);
		}
		ofSetColor(colors["edge"]);
		ofRect(sliderBB);

		if (isHovered() || isActive()) {
			ofSetColor(colors["hovered"]);
		} else {
			ofSetColor(colors["background"]);
		}
		ofRect(shrinkRectangle(sliderBB, 1));

	}

	float Slider::getProportion(void) {
		return (float)_sliderPos / _getUsefulLength();
	}

	void Slider::setProportion(float p) {
		_sliderPos = _getUsefulLength() * p;
		this->notifyControlNeedsRedraw();
	}

	CG::InteractionResult Slider::mouseButtonEvent(const MouseButtonEvent &ev) {
		CG::InteractionResult result;
		if (ev.button != _targetMouseButton) {
			return result;
		}

		if (ev.eventType == MouseButtonEvent::PRESSED) {
			if (this->inside(ev.x, ev.y)) {

				_sliderPos = _calculatePositionFromMouse(ev.x, ev.y);

				_mouseButtonHeld = true;
				result.newDataAvailable = true;
				result.newControlState = _sliderPos;

				this->setActiveState(true);

				notifyCallback();
			}
		} else if (ev.eventType == MouseButtonEvent::RELEASED) {
			this->setActiveState(false);
			_mouseButtonHeld = false;
			//result.newDataAvailable = true;
			//result.newControlState = _sliderPos;

			//notifyCallback();
		}

		return result;
	}

	CG::InteractionResult Slider::mouseMovedEvent(int x, int y) {
		CG::InteractionResult result;

		setHoveredState(this->inside(x, y));

		if (_mouseButtonHeld) {
			int tempPos = _calculatePositionFromMouse(x, y);

			if (tempPos != _sliderPos) {
				result.newDataAvailable = true;
				result.newControlState = tempPos;
				_sliderPos = tempPos;

				notifyCallback();
			}
		}
		return result;
	}

	bool Slider::_isHorizontal(void) const {
		return boundingBox.width > boundingBox.height;
	}
	/*
	int Slider::_constrainSliderPos(int pos) const {
	if (pos > _getUsefulLength()) {
	pos = _getUsefulLength();
	} else if (pos < 0) {
	pos = 0;
	}
	return pos;
	}
	*/
	int Slider::_getUsefulLength(void) const {
		int boxLength;
		if (this->_isHorizontal()) {
			boxLength = boundingBox.width;
		} else {
			boxLength = boundingBox.height;
		}

		return boxLength - _sliderThickness - edgeWidth - 1;
	}

	int Slider::_calculatePositionFromMouse(int x, int y) const {
		int pos;
		if (this->_isHorizontal()) {
			pos = x - (boundingBox.x + _sliderThickness / 2);
		} else {
			pos = y - (boundingBox.y + _sliderThickness / 2);
		}

		if (pos > _getUsefulLength()) {
			pos = _getUsefulLength();
		} else if (pos < 0) {
			pos = 0;
		}
		return pos;
	}

}