#pragma once

#include "CG_BaseControl.h"

namespace CG {

	class CheckBox : public BaseControl {
	public:

		enum class CheckTypes {
			NO_MARK,
			X,
			CHECK
		};

		CheckBox();

		void draw(void) override;

		void setLabel(string label);
		void setLabelLocation(ofPoint location);
		void setLabelLocation(int relativeX, int relativeY);

		void setFont(ofTrueTypeFont *font) override;

		void setBoundingBox(ofRectangle boundingBox) override;

		ofRectangle getLabelBoundingBox(void);
		ofRectangle getCompleteBoundingBox(void);

		CG::InteractionResult mouseButtonEvent(const MouseButtonEvent &ev) override;

		CG::ControlType getType(void) const override { return CG::ControlType::CHECK_BOX; };


		bool isChecked(void) { return _checked; };
		void setCheckedState(bool checked) { _checked = checked; };

		void setCheckMarkType(CheckTypes type) { _checkMarkType = type; };
	private:

		bool _checked;

		string _label;

		ofPoint _labelLocation;

		bool _usingRelativeLocation;
		ofPoint _relativeLabelLocation;
		CheckTypes _checkMarkType;

	};

}