#pragma once

#include "CG_BaseControl.h"

namespace CG {

	class RectangleGraphic : public BaseControl {
	public:
		RectangleGraphic();

		void draw(void) override;

		CG::ControlType getType(void) const override { return CG::ControlType::RECTANGLE; };

	};

}