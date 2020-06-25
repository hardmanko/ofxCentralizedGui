#include "CG_BaseControl.h"

namespace CG {

	class Container_experimental : public BaseControl {
	public:

		void draw(void) override;

		//This should really override a ton of stuff...
		//do the controls in the container all have the same zOrder? no, I think not.

		void registerControl(BaseControl* control);
		void unregisterControl(BaseControl* control);

		ofRectangle getBoundingBox(void) const override;

		bool inside(float x, float y) const override;

		void setFont(ofTrueTypeFont *font) override;

		CG::ControlType getType(void) const override { return CG::ControlType::CONTAINER; };

		CG::InteractionResult mouseButtonEvent(const MouseButtonEvent &ev) override;
		CG::InteractionResult mouseMovedEvent(int x, int y) override;
		CG::InteractionResult keyEvent(KeyEvent ev) override;

		bool isActive(void) const override;
		bool isVisible(void) const override;
		bool isHovered(void) const override;
		bool isEnabled(void) const override;

	private:

		std::set<BaseControl*> _registeredControls;

	};


	// nest within one another
	struct SharedContainer : public BaseControl, public std::enable_shared_from_this<SharedContainer> 
	{

		CG::ControlType getType(void) const override { return CG::ControlType::CONTAINER; };

		template <typename Ctrl>
		std::shared_ptr<Ctrl> newCtrl();

	protected:

		std::vector<std::shared_ptr<SharedContainer>> _children;

	};

}