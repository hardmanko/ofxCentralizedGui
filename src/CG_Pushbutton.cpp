#include "CG_Pushbutton.h"

namespace CG {

	Pushbutton::Pushbutton(void) :
		_textVerticalOffset(0)
	{
	}

	void Pushbutton::draw(void) {
		ofSetColor(colors["edge"]);
		ofRect(boundingBox);

		if (isHovered()) {
			ofSetColor(colors["hovered"]);
		} else {
			ofSetColor(colors["background"]);
		}

		ofRect(shrinkRectangle(boundingBox, edgeWidth));

		ofSetColor(colors["text"]);
		ofRectangle labelBB = getStringBoundingBox(_label);
		this->drawString(_label, boundingBox.x + boundingBox.width / 2 - labelBB.width / 2,
						 boundingBox.y + boundingBox.height / 2 + labelBB.height / 2 + _textVerticalOffset);

	}

	CG::InteractionResult Pushbutton::mouseButtonEvent(const MouseButtonEvent &ev) {
		CG::InteractionResult result;

		if ((ev.eventType == MouseButtonEvent::PRESSED) && boundingBox.inside(ev.x, ev.y)) {
			result.newDataAvailable = true;
			result.newControlState = 0;
			notifyControlNeedsRedraw();
			notifyCallback();
		}

		return result;
	}

	void Pushbutton::setLabel(string label) {
		_label = label;
		notifyControlNeedsRedraw();
	};

	void Pushbutton::setTextVerticalOffset(float offset) {
		_textVerticalOffset = offset;
		notifyControlNeedsRedraw();
	};

}