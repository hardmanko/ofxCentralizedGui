#pragma once

#include "CG_BaseControl.h"

namespace CG {

	class LineGraphic : public BaseControl {
	public:

		void draw(void) override;

		ControlType getType(void) const override { return ControlType::LINE; };

		void setEndpoints(float x1, float y1, float x2, float y2);

		void setStart(float x, float y);
		void setEnd(float x, float y);

	private:
		float _thickness;

		float _x1;
		float _y1;
		float _x2;
		float _y2;
	};

}