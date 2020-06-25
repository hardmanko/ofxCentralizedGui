#pragma once

#include "CG_BaseControl.h"

namespace CG {

	class Pushbutton : public LabeledControl {
	public:
		Pushbutton();

		void draw(void) override;

		void setTextVerticalOffset(float offset);

		CG::InteractionResult mouseButtonEvent(const MouseButtonEvent &ev) override;

		CG::ControlType getType(void) const override { return CG::ControlType::BUTTON; };

	private:

		float _textVerticalOffset;
	};

}
