#include "CG_ManagerContainer.h"

#include "GLFW/glfw3.h"

namespace CG {

	ManagerContainer::ManagerContainer(void) :
		_redrawRequested(true),
		_groupDelimiter(","),
		_lastControlSpacing(3),
		_lastControlDirection(Dir::DOWN)
	{
		setDefaultFont(OF_TTF_SANS, 10, true, false, .3, 96);

		ofAddListener(ofEvents().mousePressed, this, &ManagerContainer::mouseClickedEventHandler);
		ofAddListener(ofEvents().mouseReleased, this, &ManagerContainer::mouseReleasedEventHandler);

		ofAddListener(ofEvents().mouseMoved, this, &ManagerContainer::mouseMovedEventHandler);
		ofAddListener(ofEvents().mouseDragged, this, &ManagerContainer::mouseDraggedEventHandler);

		ofAddListener(ofEvents().keyPressed, this, &ManagerContainer::keypressHandler);
		ofAddListener(ofEvents().keyReleased, this, &ManagerContainer::keyReleaseHandler);
	}

	ManagerContainer::~ManagerContainer(void) {
		ofRemoveListener(ofEvents().mousePressed, this, &ManagerContainer::mouseClickedEventHandler);
		ofRemoveListener(ofEvents().mouseReleased, this, &ManagerContainer::mouseReleasedEventHandler);

		ofRemoveListener(ofEvents().mouseMoved, this, &ManagerContainer::mouseMovedEventHandler);
		ofRemoveListener(ofEvents().mouseDragged, this, &ManagerContainer::mouseDraggedEventHandler);

		ofRemoveListener(ofEvents().keyPressed, this, &ManagerContainer::keypressHandler);
		ofRemoveListener(ofEvents().keyReleased, this, &ManagerContainer::keyReleaseHandler);

		//Free the memory allocated on the pointers in _controls
		for (unsigned int i = 0; i < _controls.size(); i++) {
			delete _controls[i];
		}
	}

	//The default font is assigned to all new created controls.
	void ManagerContainer::setDefaultFont(string fontName, int fontSize, bool antialiased, bool fullCharSet, float simplifyAmount, int dpi) {
		_defaultFont.loadFont(OF_TTF_SANS, 10, antialiased, fullCharSet, true, simplifyAmount, dpi);
	}

	void ManagerContainer::setDelimiter(std::string delim) {
		_groupDelimiter = delim;
	}

	void ManagerContainer::pollEvents(void) {
		glfwPollEvents();
	}

	int ManagerContainer::availableInteractions(void) {
		return _controlInteractions.size();
	}

	CG::InteractionResult ManagerContainer::getNextInteraction(void) {
		CG::InteractionResult temp = _controlInteractions.front();
		_controlInteractions.pop();
		return temp;
	}

	void ManagerContainer::draw(void) {
		vector<int> controlZOrders(_controls.size());
		for (int i = 0; i < _controls.size(); i++) {
			//Put any active controls on the top of the z-order.
			controlZOrders[i] = _controls[i]->isActive() ? numeric_limits<int>::max() : _controls[i]->zOrder;
		}

		vector<unsigned int> controlDrawOrder = _convertZOrderToDrawOrder(controlZOrders);

		for (int i = 0; i < controlDrawOrder.size(); i++) {
			if (_controls[controlDrawOrder[i]]->isVisible()) {
				_controls[controlDrawOrder[i]]->draw();
			}
		}
	}

	void ManagerContainer::moveControls(int xChange, int yChange) {
		moveControlsInGroup("*", xChange, yChange);
	}

	void ManagerContainer::moveControlsInGroup(string group, int xChange, int yChange) {
		vector<int> matchedControls = _getMatchedGroupIndices(group);

		for (int i = 0; i < matchedControls.size(); i++) {
			ofRectangle currentBB = _controls.at(matchedControls[i])->getBoundingBox();
			currentBB.x += xChange;
			currentBB.y += yChange;
			_controls.at(matchedControls[i])->setBoundingBox(currentBB);
		}
	}

	void ManagerContainer::setGroupEdgeWidth(std::string groups, float edgeWidth) {
		this->applyFunction(groups, [&edgeWidth](BaseControl& c) { c.edgeWidth = edgeWidth; });
	}

	void ManagerContainer::setGroupZOrder(string groups, int zOrder) {
		this->applyFunction(groups, [&zOrder](BaseControl& c) { c.zOrder = zOrder; });
	}

	void ManagerContainer::setGroupVisibility(string groups, bool visible) {
		this->applyFunction(groups, [&visible](BaseControl& c) { c.setVisibleState(visible); });
	}

	void ManagerContainer::setGroupEnabledState(string groups, bool enabled) {
		this->applyFunction(groups, [&enabled](BaseControl& c) { c.setEnabledState(enabled); });
	}

	void ManagerContainer::setGroupWidth(string groups, float width) {
		this->applyFunction(groups, [&width](BaseControl& c) { c.setWidth(width); });
	}

	void ManagerContainer::setGroupHeight(string groups, float height) {
		this->applyFunction(groups, [&height](BaseControl& c) { c.setHeight(height); });
	}

	void ManagerContainer::setGroupFont(string groups, ofTrueTypeFont *font) {
		/*
		vector<int> matchedControls = _getMatchedGroupIndices(controlGroup);

		for (int i = 0; i < matchedControls.size(); i++) {
		_controls.at( matchedControls[i] )->setFont(font);
		}
		*/
		this->applyFunction(groups, [font](BaseControl& c) { c.setFont(font); });
	}


	/*! For the specified control group, sets the color component to the given color.
	\param controlGroup The name of the control group.
	\param controlComponent The component of the controls to set the color to. The basic components that many
	controls use are "text", "edge", "background", "hovered", and "disabled". Some control types may use other
	components.
	\param color The color to set the component to. */
	void ManagerContainer::setGroupComponentColor(string groups, string controlComponent, ofColor color) {
		auto applyColor = [&controlComponent, &color](BaseControl& c) {
			c.colors[controlComponent] = color;
		};
		this->applyFunction(groups, applyColor);
	}

	/*! Get a bounding box that is the smallest rectangle that contains all members of the named control group. */
	ofRectangle ManagerContainer::getGroupBoundingBox(string controlGroup) {
		vector<int> matchedControls = _getMatchedGroupIndices(controlGroup);

		ofRectangle inclusiveBoundingBox;
		if (matchedControls.size() >= 1) {
			inclusiveBoundingBox = _controls.at(matchedControls[0])->getBoundingBox();

			for (int i = 1; i < matchedControls.size(); i++) {
				CG::BaseControl* con = _controls.at(matchedControls[i]);

				ofRectangle tbb = con->getBoundingBox();

				switch (con->getType()) {
				case CG::ControlType::CHECK_BOX:
					tbb = con->typecast<CG::CheckBox>()->getCompleteBoundingBox();
					break;
				}

				inclusiveBoundingBox.growToInclude(tbb);
			}
		}

		return inclusiveBoundingBox;
	}

	void ManagerContainer::deleteControlsInGroup(string controlGroups) {
		vector<int> matchedControls = _getMatchedGroupIndices(controlGroups);

		for (int i = matchedControls.size() - 1; i >= 0; i--) {
			_deleteControlAtIndex(matchedControls[i]);
		}
	}

	void ManagerContainer::deleteControl(string controlName, string controlGroups) {

		vector<int> matchedControls = _getMatchedGroupIndices(controlGroups);
		for (int ci : matchedControls) {
			if (_controls[ci]->name == controlName) {
				_deleteControlAtIndex(ci);
				return;
			}
		}

		/*
		vector<string> groups = BaseControl::splitGroupString(controlGroups, delim);

		for (int i = 0; i < _controls.size(); i++) {
		if (_controls[i]->name == controlName) {

		//This is an optimization: hasGroups() should work the same.
		bool hasAllGroups = true;
		for (auto g : groups) {
		hasAllGroups = hasAllGroups && _controls[i]->hasGroup(g);
		}

		if (hasAllGroups) {
		_deleteControlAtIndex(i);
		return;
		}

		}
		}
		*/
	}

	void ManagerContainer::_deleteControlAtIndex(int index) {
		try {
			delete _controls.at(index); //Free the memory allocated on the pointer
			_controls.erase(_controls.begin() + index); //Then erase the pointer from the vector

			_redrawRequested = true;
		}
		catch (std::exception e) {
			cout << e.what() << endl;
		}
	}

	BaseControl& ManagerContainer::getLastControl(void) {
		return *(_controls.back());
	}

	/*! Returns a reference to a control with the given set of control purposes and control groups.
	If no matching control is found, a std::out_of_range exception is thrown. */
	BaseControl& ManagerContainer::getControl(string controlName, string controlGroups) {

		vector<int> matchedControls = _getMatchedGroupIndices(controlGroups);
		for (int ci : matchedControls) {
			if (_controls[ci]->name == controlName) {
				return *(_controls[ci]);
			}
		}

		string s = "Control not found. Purposes: " + controlName + ". Groups: " + controlGroups;
		throw std::out_of_range(s.c_str());
	}

	std::vector<BaseControl*> ManagerContainer::getControls(std::string controlGroups) {
		vector<int> matchedControls = _getMatchedGroupIndices(controlGroups);
		vector<BaseControl*> rval(matchedControls.size());
		for (int i = 0; i < matchedControls.size(); i++) {
			rval[i] = _controls[matchedControls[i]];
		}
		return rval;
	}

	std::vector<BaseControl*> ManagerContainer::getControls(CG::ControlType type) {
		std::vector<BaseControl*> rval;
		for (BaseControl* c : _controls) {
			if (c->getType() == type) {
				rval.push_back(c);
			}
		}
		return rval;
	}

	vector<unsigned int> ManagerContainer::_convertZOrderToDrawOrder(vector<int> zOrders) {

		auto pairFirstLessThan = [](pair<int, unsigned int> p1, pair<int, unsigned int> p2) -> bool {
			return (p1.first < p2.first);
		};

		vector < pair<int, unsigned int> > orderPairs(zOrders.size());
		for (int i = 0; i < orderPairs.size(); i++) {
			orderPairs.at(i).first = zOrders.at(i);
			orderPairs.at(i).second = i;
		}

		sort(orderPairs.begin(), orderPairs.end(), pairFirstLessThan);

		vector<unsigned int> drawOrder(zOrders.size());
		for (int i = 0; i < orderPairs.size(); i++) {
			drawOrder.at(i) = orderPairs.at(i).second;
		}

		return drawOrder;
	}

	bool ManagerContainer::redrawRequestedSincelastCheck(void) {
		bool redrawRequested = false;
		for (unsigned int i = 0; i < _controls.size(); i++) {
			if (_controls[i]->redrawRequested) {
				redrawRequested = true;
				_controls[i]->redrawRequested = false;
				//Don't break, unset all redraws that are requested (or maybe unset them once they are actually drawn)
			}
		}
		if (_redrawRequested) {
			redrawRequested = true;
			_redrawRequested = false;
		}

		return redrawRequested;

		/*
		bool requestedSinceLastCheck = false;
		if (_redrawRequested) {
		requestedSinceLastCheck = true;
		_redrawRequested = false;
		}
		return requestedSinceLastCheck;
		*/
	}

	void ManagerContainer::mouseDraggedEventHandler(ofMouseEventArgs &a) {
		a.type = ofMouseEventArgs::Dragged;
		mouseMovedEventHandler(a);
	}

	void ManagerContainer::mouseMovedEventHandler(ofMouseEventArgs &a) {

		int maxZOrder = numeric_limits<int>::min();
		int maxZOrderIndex = -1;
		bool anyControlActive = false;

		for (int i = 0; i < _controls.size(); i++) {
			if (_controls[i]->isEnabled()) {
				//Anything that is active or already hovered always get the event
				if (_controls[i]->isActive() || _controls[i]->isHovered()) {
					CG::InteractionResult result = _controls[i]->mouseMovedEvent(a.x, a.y);
					result.control = _controls[i];
					_controlInteractions.push(result);

					if (_controls[i]->isActive()) {
						anyControlActive = true;
					}
				}

				if (_controls[i]->inside(a.x, a.y)) {
					//Regardless of whether it is already active or hovered, things on top capture hovers.
					if (_controls[i]->zOrder > maxZOrder) {
						maxZOrder = _controls[i]->zOrder;
						maxZOrderIndex = i;
					}
				}
			}
		}

		if (!anyControlActive && (maxZOrderIndex != -1)) {
			CG::InteractionResult result = _controls[maxZOrderIndex]->mouseMovedEvent(a.x, a.y);
			result.control = _controls[maxZOrderIndex];
			_controlInteractions.push(result);
		}

	}


	void ManagerContainer::mouseClickedEventHandler(ofMouseEventArgs &a) {
		a.type = ofMouseEventArgs::Pressed;
		mouseButtonEventHandler(a);
	}

	void ManagerContainer::mouseReleasedEventHandler(ofMouseEventArgs &a) {
		a.type = ofMouseEventArgs::Released;
		mouseButtonEventHandler(a);
	}


	void ManagerContainer::mouseButtonEventHandler(ofMouseEventArgs &a) {
		MouseButtonEvent ev;

		ev.button = a.button;
		ev.x = a.x;
		ev.y = a.y;
		if (a.type == ofMouseEventArgs::Released) {
			ev.eventType = MouseButtonEvent::RELEASED;
		} else if (a.type == ofMouseEventArgs::Pressed) {
			ev.eventType = MouseButtonEvent::PRESSED;
		} else {
			//This function should not be getting this event.
			return;
		}


		int maxZOrder = numeric_limits<int>::min();
		int maxZOrderIndex = -1;
		bool aControlWasActive = false;

		for (int i = 0; i < _controls.size(); i++) {
			if (_controls[i]->isEnabled()) {
				if (_controls[i]->isActive())	{
					//Any active controls are given the mouse button event.
					CG::InteractionResult result = _controls[i]->mouseButtonEvent(ev);
					result.control = _controls[i];
					_controlInteractions.push(result);

					aControlWasActive = true;
				} else if (_controls[i]->inside(ev.x, ev.y)) {
					//Any controls that are not active but are clicked on are given the mouse click event if they have the highest z-order.
					if (_controls[i]->zOrder > maxZOrder) {
						maxZOrder = _controls[i]->zOrder;
						maxZOrderIndex = i;
					}
				}
			}
		}

		if (!aControlWasActive && (maxZOrderIndex != -1)) {
			CG::InteractionResult result = _controls[maxZOrderIndex]->mouseButtonEvent(ev);
			result.control = _controls[maxZOrderIndex];
			_controlInteractions.push(result);
		}
	}

	void ManagerContainer::keypressHandler(ofKeyEventArgs &a) {

		if ((a.key & OF_KEY_CONTROL) == OF_KEY_CONTROL) {
			_heldModifiers.ctrl = true;
		} else if ((a.key & OF_KEY_ALT) == OF_KEY_ALT) {
			_heldModifiers.alt = true;
		} else if ((a.key & OF_KEY_SHIFT) == OF_KEY_SHIFT) {
			_heldModifiers.shift = true;
		} else if ((a.key & OF_KEY_SUPER) == OF_KEY_SUPER) {
			_heldModifiers.meta = true;
		}

		KeyEvent ev;
		ev.eventType = KeyEventType::PRESSED; //Deal with key repeats
		ev.key = a.key;
		ev.heldModifiers = _heldModifiers;

		for (int i = 0; i < _controls.size(); i++) {
			if (_controls[i]->isEnabled() && _controls[i]->isActive()) {
				if ((_controls[i]->getType() == CG::ControlType::TEXT_BOX) || (_controls[i]->getType() == CG::ControlType::CONTAINER)) {
					CG::InteractionResult result = _controls[i]->keyEvent(ev);
					if (result.control == nullptr) {
						result.control = _controls[i];
					}
					_controlInteractions.push(result);

					return; //Only ever route keypresses to one source.
				}
			}
		}

		//Keyboard shortcuts!
		CG::InteractionResult res;
		res.interactionCategory = InteractionCategory::KEYBOARD_SHORTCUT;
		res.newDataAvailable = true;
		res.newControlState = a.key;
		_controlInteractions.push(res);
	}

	void ManagerContainer::keyReleaseHandler(ofKeyEventArgs &a) {

		if ((a.key & OF_KEY_CONTROL) == OF_KEY_CONTROL) {
			_heldModifiers.ctrl = false;
		} else if ((a.key & OF_KEY_ALT) == OF_KEY_ALT) {
			_heldModifiers.alt = false;
		} else if ((a.key & OF_KEY_SHIFT) == OF_KEY_SHIFT) {
			_heldModifiers.shift = false;
		} else if ((a.key & OF_KEY_SUPER) == OF_KEY_SUPER) {
			_heldModifiers.meta = false;
		}

		//It seems like something else might happen here...
	}


	vector<int> ManagerContainer::_matchControls(const vector<BaseControl*>& controls, string controlGroups, string delim, bool matchAll) {

		vector<int> rval;

		vector<string> groupsStrings = splitGroupString(controlGroups, delim);


		for (int i = 0; i < controls.size(); i++) {
			BaseControl* c = controls[i];

			bool hasAllGroups = true;
			bool hasAnyGroup = false;
			for (string g : groupsStrings) {
				if (c->hasGroup(g)) {
					hasAnyGroup = true;
				} else {
					hasAllGroups = false;
				}
			}

			if (matchAll) {
				if (hasAllGroups) {
					rval.push_back(i);
				}
			} else {
				if (hasAnyGroup) {
					rval.push_back(i);
				}
			}
		}

		return rval;
	}

	vector<int> ManagerContainer::_getMatchedGroupIndices(string controlGroups) {
		vector<int> matchedControls;
		vector<string> groupNames = splitGroupString(controlGroups, _groupDelimiter);

		for (int i = 0; i < _controls.size(); i++) {
			if (_controls[i]->hasGroups(groupNames)) {
				matchedControls.push_back(i);
			}
		}

		return matchedControls;
	}

	void ManagerContainer::applyFunction(std::string groups, std::function<void(BaseControl&)> fn) {
		vector<BaseControl*> gcs = this->getControls(groups);
		for (BaseControl* c : gcs) {
			fn(*c);
		}
	}

	void ManagerContainer::applyFunction(ControlType type, std::function<void(BaseControl&)> fn) {
		for (BaseControl* c : _controls) {
			if (c->getType() == type) {
				fn(*c);
			}
		}
	}

	void ManagerContainer::applyFunction(std::string groups, ControlType type, std::function<void(BaseControl&)> fn) {
		vector<BaseControl*> gcs = this->getControls(groups);
		for (BaseControl* c : gcs) {
			if (c->getType() == type) {
				fn(*c);
			}
		}
	}

}