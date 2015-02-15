#include "CG_DragBox.h"

namespace CG {

	DragBox::DragBox(void) :
		_targetMouseButton(0),
		_mouseButtonHeld(false)
	{
	}

	ofPoint DragBox::getLastMovement(void) {
		ofPoint rval;
		if (_dragCoordinates.size() >= 2) {
			//rval.x = _dragCoordinates.back().x - _dragCoordinates.at( _dragCoordinates.size() - 2 ).x;

			rval = _dragCoordinates.at(_dragCoordinates.size() - 1) - _dragCoordinates.at(_dragCoordinates.size() - 2);
		}
		return rval;
	}

	CG::InteractionResult DragBox::mouseButtonEvent(const MouseButtonEvent &ev) {
		CG::InteractionResult result;
		if (ev.button != _targetMouseButton) {
			return result;
		}

		if (ev.eventType == MouseButtonEvent::PRESSED) {
			if (this->inside(ev.x, ev.y)) {
				this->setActiveState(true);
				_mouseButtonHeld = true;
				result.newDataAvailable = true;

				_dragCoordinates.clear();
				//Start the drag here.
				mouseMovedEvent(ev.x, ev.y);

				notifyCallback();
			}
		} else if (ev.eventType == MouseButtonEvent::RELEASED) {
			this->setActiveState(false);
			_mouseButtonHeld = false;
			result.newDataAvailable = true;

			notifyCallback();
		}

		return result;
	}

	CG::InteractionResult DragBox::mouseMovedEvent(int x, int y) {
		CG::InteractionResult result;

		if (_mouseButtonHeld) {

			//The drag cannot go outside of the bounding box. If it does, clamp it to the box.
			if (x < boundingBox.x) {
				x = boundingBox.x;
			} else if (x > boundingBox.getRight()) {
				x = boundingBox.getRight();
			}

			if (y < boundingBox.y) {
				y = boundingBox.y;
			} else if (y > boundingBox.getBottom()) {
				y = boundingBox.getBottom();
			}

			_dragCoordinates.push_back(ofPoint(x, y));

			result.newDataAvailable = true;

			notifyCallback();
		}

		return result;
	}

}