#include "CG_BaseControl.h"

namespace CG {

	class Slider : public BaseControl {
	public:

		Slider(void);

		void draw(void) override;

		float getProportion(void); //Proportion of the way from beginning to end.
		void setProportion(float p);

		CG::ControlType getType(void) const override{ return CG::ControlType::SLIDER; };

		CG::InteractionResult mouseMovedEvent(int x, int y) override;
		CG::InteractionResult mouseButtonEvent(const MouseButtonEvent &ev) override;
		//Consider mouse scrolled event handler.

		//void setEndpointLabels(string ep1, string ep2);
		//void setName(string n);

	private:

		int _sliderPos;
		unsigned int _sliderThickness;

		//std::string _beginningLabel;
		//std::string _endLabel;

		int _targetMouseButton;
		bool _mouseButtonHeld;

		bool _isHorizontal(void) const;
		int _getUsefulLength(void) const;
		int _calculatePositionFromMouse(int x, int y) const;

	};

}