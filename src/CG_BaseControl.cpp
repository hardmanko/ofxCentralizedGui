#include "CG_BaseControl.h"

#include "CG_Manager.h"

namespace CG {

	BaseControl::BaseControl(void) :
		edgeWidth(1),
		zOrder(0),
		_controlActive(false),
		_controlEnabled(true),
		_controlVisible(true),
		_controlHovered(false),
		textFont(nullptr),
		redrawRequested(true),
		_groupDelimiter(",")
	{
		colors["text"] = ofColor(0);
		colors["edge"] = ofColor(50);
		colors["background"] = ofColor(255);
		colors["hovered"] = ofColor(200);
		colors["disabled"] = ofColor(127);
		//notifyControlNeedsRedraw();
	}

	void BaseControl::setGroupDelimiter(std::string delim) {
		_groupDelimiter = delim;
	}

	//Basic implementations of virtual functions.
	void BaseControl::draw(void) {
		return;
	}

	CG::InteractionResult BaseControl::mouseButtonEvent(const MouseButtonEvent &ev) {
		return CG::InteractionResult();
	}

	CG::InteractionResult BaseControl::mouseMovedEvent(int x, int y) {
		setHoveredState(this->inside(x, y));
		return CG::InteractionResult();
	}

	CG::InteractionResult BaseControl::keyEvent(KeyEvent ev) {
		return CG::InteractionResult();
	}


	void BaseControl::setFont(ofTrueTypeFont *font) {
		textFont = font;
		notifyControlNeedsRedraw();
	}

	void BaseControl::drawString(std::string s, int x, int y) {
		if (textFont != nullptr) {
			textFont->drawStringAsShapes(s, x, y);
		} else {
			cout << "Error: font pointer is NULL" << endl;
		}
	}

	ofRectangle BaseControl::getStringBoundingBox(std::string s, int x, int y) {
		if (textFont != NULL) {
			return textFont->getStringBoundingBox(s, x, y);
		}
		cout << "Error: font pointer is NULL" << endl;
		return ofRectangle();
	}

	bool BaseControl::nameContains(string s) {
		return name.find(s) != string::npos;
	}

	void BaseControl::addGroups(std::string groupNames) {
		std::vector<std::string> newGroups = splitGroupString(groupNames, _groupDelimiter);

		groups.insert(newGroups.begin(), newGroups.end());
	}

	void BaseControl::removeGroup(std::string groupName) {
		if (groupName == "*") {
			//groups.clear();
			return;
		}

		groups.erase(groupName);
	}

	void BaseControl::clearGroups(void) {
		groups.clear();
	}

	std::string BaseControl::getGroupString(void) const {
		std::string rval = "";
		for (std::string g : groups) {
			rval += g + ",";
		}
		if (rval != "") {
			rval.pop_back(); //pop off the last comma
		}
		return rval;
	}

	bool BaseControl::hasGroup(std::string groupName) const {
		if (groupName == "*") {
			return true;
		}

		if (groups.find(groupName) != groups.end()) {
			return true;
		}

		return false;
	}

	bool BaseControl::hasGroups(std::string groupNames) const {
		return hasGroups(splitGroupString(groupNames, _groupDelimiter));
	}

	bool BaseControl::hasGroups(std::vector<std::string> groupNames) const {
		bool hasAll = true;
		for (std::string& g : groupNames) {
			hasAll = hasAll && hasGroup(g);
		}
		return hasAll;
	}


	void BaseControl::setActiveState(bool active) {
		if (_controlActive != active) {
			_controlActive = active;
			notifyControlNeedsRedraw();
		}
	}

	void BaseControl::setEnabledState(bool enabled) {
		if (_controlEnabled != enabled) {
			_controlEnabled = enabled;
			notifyControlNeedsRedraw();
		}
	}

	void BaseControl::setHoveredState(bool hovered) {
		if (_controlHovered != hovered) {
			if (hovered) { // if now hovered,
				_wasEntered = true; // it was entered
			} else {
				_wasExited = true; // if not hovered, it was exited
			}

			_controlHovered = hovered;
			notifyControlNeedsRedraw();
		}
	}

	void BaseControl::setVisibleState(bool visible) {
		if (_controlVisible != visible) {
			_controlVisible = visible;
			notifyControlNeedsRedraw();
		}
	}

	bool BaseControl::isActive(void) const {
		return _controlActive;
	}

	bool BaseControl::isEnabled(void) const {
		return _controlEnabled;
	}

	bool BaseControl::isHovered(void) const {
		return _controlHovered;
	}

	bool BaseControl::isVisible(void) const {
		return _controlVisible;
	}

	bool BaseControl::wasEntered(bool reset) {
		bool rval = _wasEntered;
		if (reset) {
			_wasEntered = false;
		}
		return rval;
	}

	bool BaseControl::wasExited(bool reset) {
		bool rval = _wasExited;
		if (reset) {
			_wasExited = false;
		}
		return rval;
	}

	void BaseControl::notifyControlNeedsRedraw(void) {
		redrawRequested = true;
	}

	void BaseControl::setBoundingBox(ofRectangle bb) {
		boundingBox = bb;
		notifyControlNeedsRedraw();
	}

	void BaseControl::setLocation(float x, float y) {
		ofRectangle tempBB = boundingBox;
		tempBB.x = x;
		tempBB.y = y;
		setBoundingBox(tempBB);
	}

	void BaseControl::setX(float x) {
		ofRectangle tempBB = boundingBox;
		tempBB.x = x;
		setBoundingBox(tempBB);
	}

	void BaseControl::setY(float y) {
		ofRectangle tempBB = boundingBox;
		tempBB.y = y;
		setBoundingBox(tempBB);
	}

	void BaseControl::setWidth(float width) {
		ofRectangle tempBB = boundingBox;
		tempBB.width = width;
		setBoundingBox(tempBB);
	}

	void BaseControl::setHeight(float height) {
		ofRectangle tempBB = boundingBox;
		tempBB.height = height;
		setBoundingBox(tempBB);
	}

	void BaseControl::setCallback(NewDataCallbackType cb) {
		_callback = cb;
	}

	ofTrueTypeFont* BaseControl::getFont(void) {
		return this->textFont;
	}

	////////////////////
	// LabeledControl //
	////////////////////
	void LabeledControl::setLabel(std::string label) {
		_label = label;
		notifyControlNeedsRedraw();
	}

	const std::string& LabeledControl::getLabel(void) const {
		return _label;
	}

}