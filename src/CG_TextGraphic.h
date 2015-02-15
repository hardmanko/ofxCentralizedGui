#pragma once

#include "CG_BaseControl.h"

namespace CG {

	class TextGraphic : public BaseControl {
	public:
		TextGraphic(void);

		void draw(void) override;

		void setLabel(std::string text);
		void setFont(ofTrueTypeFont *font) override;

		bool inside(float x, float y) const override;

		ofRectangle getBoundingBox(void) const override;

		CG::ControlType getType(void) const override { return CG::ControlType::TEXT; };

		CG::InteractionResult mouseButtonEvent(const MouseButtonEvent &ev) override;
		CG::InteractionResult mouseMovedEvent(int x, int y) override;

	private:
		string _text;

		int _textHeightAboveBaseLine;

		//TODO: Create some kind of resource linking ability.
		bool _textClickable;
	};

}