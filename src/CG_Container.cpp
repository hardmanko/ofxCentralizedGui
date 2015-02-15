#include "CG_Container.h"

namespace CG {

	void Container_experimental::draw(void) {

		//draw inactive controls, then active controls.
		for (BaseControl* c : _registeredControls) {
			if (!c->isActive()) {
				c->draw();
			}
		}

		for (BaseControl* c : _registeredControls) {
			if (c->isActive()) {
				c->draw();
			}
		}
	}

	void Container_experimental::registerControl(BaseControl* control) {

		if (control->_container) {
			control->_container->unregisterControl(control);
		}

		control->_container = this;

		_registeredControls.insert(control);
	}

	void Container_experimental::unregisterControl(BaseControl* control) {
		_registeredControls.erase(control);
		control->_container = nullptr;
	}

	ofRectangle Container_experimental::getBoundingBox(void) const {
		ofRectangle inclusiveBoundingBox;

		std::set<BaseControl*>::iterator it = _registeredControls.begin();
		inclusiveBoundingBox = (*it)->getBoundingBox();
		it++;

		for (/* */; it != _registeredControls.end(); it++) {
			inclusiveBoundingBox.growToInclude((*it)->getBoundingBox());
		}

		return inclusiveBoundingBox;
	}

	bool Container_experimental::inside(float x, float y) const {
		for (auto c : _registeredControls) {
			if (c->inside(x, y)) {
				return true;
			}
		}
		return false;
	}

	void Container_experimental::setFont(ofTrueTypeFont *font) {
		for (auto c : _registeredControls) {
			c->setFont(font);
		}
	}


	CG::InteractionResult Container_experimental::mouseButtonEvent(const MouseButtonEvent &ev) {

		int maxZOrder = numeric_limits<int>::min();
		BaseControl* maxZControl = nullptr;
		bool anyControlWasActive = false;

		CG::InteractionResult result;

		for (auto c : _registeredControls) {
			if (c->isEnabled()) {
				if (c->isActive())	{
					//Any active controls are given the mouse button event.
					result = c->mouseButtonEvent(ev);
					//result.control = c;

					anyControlWasActive = true;
				} else if (c->inside(ev.x, ev.y)) {
					//Any controls that are not active but are clicked on are given the mouse click event if they have the highest z-order.
					if (c->zOrder > maxZOrder) {
						maxZOrder = c->zOrder;
						maxZControl = c;
					}
				}
			}
		}

		if (!anyControlWasActive && (maxZControl != nullptr)) {
			result = maxZControl->mouseButtonEvent(ev);
		}

		return result;
	}

	CG::InteractionResult Container_experimental::mouseMovedEvent(int x, int y) {

		int maxZOrder = numeric_limits<int>::min();
		BaseControl* maxZControl = nullptr;
		bool anyControlActive = false;

		CG::InteractionResult result;

		for (auto c : _registeredControls) {
			if (c->isEnabled()) {
				//Anything that is active or already hovered always get the event
				if (c->isActive() || c->isHovered()) {
					result = c->mouseMovedEvent(x, y);
					//result.control = c;

					if (c->isActive()) {
						anyControlActive = true;
					}
				}

				if (c->inside(x, y)) {
					//Regardless of whether it is already active or hovered, things on top capture hovers.
					if (c->zOrder > maxZOrder) {
						maxZOrder = c->zOrder;
						maxZControl = c;
					}
				}
			}
		}

		if (!anyControlActive && (maxZControl != nullptr)) {
			result = maxZControl->mouseMovedEvent(x, y);
			//result.control = maxZControl;
		}
		return result;
	}

	CG::InteractionResult Container_experimental::keyEvent(KeyEvent ev) {
		for (auto c : _registeredControls) {
			if (c->isEnabled()) {
				if ((c->getType() == CG::ControlType::TEXT_BOX) && c->isActive()) {
					CG::InteractionResult result = c->keyEvent(ev);
					//result.control = c;
					return result;
				}
			}
		}
		return CG::InteractionResult();
	}

	bool Container_experimental::isActive(void) const {
		for (auto c : _registeredControls) {
			if (c->isActive()) {
				return true;
			}
		}
		return false;
	}

	bool Container_experimental::isVisible(void) const {
		for (auto c : _registeredControls) {
			if (c->isVisible()) {
				return true;
			}
		}
		return false;
	}

	bool Container_experimental::isHovered(void) const {
		for (auto c : _registeredControls) {
			if (c->isHovered()) {
				return true;
			}
		}
		return false;
	}

	bool Container_experimental::isEnabled(void) const {
		for (auto c : _registeredControls) {
			if (c->isEnabled()) {
				return true;
			}
		}
		return false;
	}

}