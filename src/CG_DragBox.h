#pragma once

#include "CG_BaseControl.h"

namespace CG {

	class DragBox : public BaseControl {
	public:

		DragBox();

		CG::InteractionResult mouseButtonEvent(const MouseButtonEvent &ev) override;
		CG::InteractionResult mouseMovedEvent(int x, int y) override;

		bool dragStarted(void) { return _dragCoordinates.size() > 1; };
		bool dragEnded(void) { return (_dragCoordinates.size() > 1) && !_mouseButtonHeld; };
		ofPoint getDragStart(void) { return _dragCoordinates.front(); };
		ofPoint getDragEnd(void) { return _dragCoordinates.back(); };
		ofPoint getLastMovement(void);

		CG::ControlType getType(void) const override { return CG::ControlType::DRAG_BOX; };

	private:

		int _targetMouseButton;
		bool _mouseButtonHeld;
		vector<ofPoint> _dragCoordinates;

	};

}