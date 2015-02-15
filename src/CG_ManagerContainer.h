#pragma once

#include <vector>
#include <queue>

#include "CG_Typedefs.h"

#include "CG_BaseControl.h"
#include "CG_Pushbutton.h"
#include "CG_CheckBox.h"
#include "CG_Dropdown.h"
#include "CG_Framebuffer.h"
#include "CG_TextBox.h"
#include "CG_TextGraphic.h"
#include "CG_RectangleGraphic.h"
#include "CG_Console.h"
#include "CG_DragBox.h"
#include "CG_Canvas.h"
#include "CG_LineGraphic.h"
#include "CG_Slider.h"
#include "CG_Container.h"

using namespace std;

namespace CG {

	//You could add DONTFOLLOW versions of these. Or a "backtrack" function.
	enum class Dir {
		UP,
		DOWN,
		RIGHT,
		LEFT,
		USE_PREVIOUS_DIRECTION
	};

	class ManagerContainer : public BaseControl {
	public:
		ManagerContainer();
		~ManagerContainer();

		void setDelimiter(std::string delim);
		void setDefaultFont(string fontName, int fontSize, bool antialiased, bool fullCharSet, float simplifyAmount, int dpi);

		void pollEvents(void);
		int availableInteractions(void);
		CG::InteractionResult getNextInteraction(void);

		bool redrawRequestedSincelastCheck(void);
		void draw(void);

		template <typename T> T& createControl(ofRectangle boundingBox, string name, string groups = "");

		template <typename T> T& createControl(float spacing, Dir directionFromBB, ofRectangle BB,
											   std::string name, std::string groups, float width, float height);
		template <typename T> T& createControl(Dir directionFromBB, ofRectangle BB,
											   std::string name, std::string groups, float width, float height);
		template <typename T> T& createControl(Dir directionFromBB, ofRectangle BB,
											   std::string name, std::string groups);

		template <typename T> T& createControl(float spacing, Dir directionFromLast, std::string name, std::string groups,
											   float width, float height);
		template <typename T> T& createControl(float spacing, Dir directionFromLast, std::string name, std::string groups = "");
		template <typename T> T& createControl(float spacing, std::string name, std::string groups = "");

		template <typename T> T& createControl(std::string name, std::string groups = "");
		template <typename T> T& createControl(Dir directionFromLast, std::string name, std::string groups = "");
		template <typename T> T& createControl(Dir directionFromLast, std::string name, std::string groups, float width, float height);

		void deleteControlsInGroup(string groups);
		void deleteControl(string name, string groups);

		BaseControl& getLastControl(void);
		template <typename T> T& getControl(string name, string groups = "*");
		BaseControl& getControl(string name, string groups = "*");

		std::vector<BaseControl*> getControls(std::string groups);
		std::vector<BaseControl*> getControls(CG::ControlType type);

		void moveControls(int xChange, int yChange);
		void moveControlsInGroup(string group, int xChange, int yChange);

		void applyFunction(string groups, std::function<void(BaseControl&)> fn);
		void applyFunction(ControlType type, std::function<void(BaseControl&)> fn);
		void applyFunction(std::string groups, ControlType type, std::function<void(BaseControl&)> fn);

		void setGroupEdgeWidth(std::string groups, float edgeWidth);
		void setGroupZOrder(string groups, int zOrder);
		void setGroupVisibility(string groups, bool visible);
		void setGroupEnabledState(string groups, bool enabled);
		void setGroupWidth(string groups, float width);
		void setGroupHeight(string groups, float height);
		void setGroupFont(string groups, ofTrueTypeFont *font);
		void setGroupComponentColor(string groups, string controlComponent, ofColor color);

		ofRectangle getGroupBoundingBox(string groups);

		void mouseMovedEventHandler(ofMouseEventArgs &a);
		void mouseDraggedEventHandler(ofMouseEventArgs &a);

		void mouseClickedEventHandler(ofMouseEventArgs &a);
		void mouseReleasedEventHandler(ofMouseEventArgs &a);
		void mouseButtonEventHandler(ofMouseEventArgs &a);

		void keypressHandler(ofKeyEventArgs &a);
		void keyReleaseHandler(ofKeyEventArgs &a);

	private:

		std::string _groupDelimiter;

		void _deleteControlAtIndex(int index);

		queue <CG::InteractionResult> _controlInteractions;

		vector<BaseControl*> _controls;

		vector<unsigned int> _convertZOrderToDrawOrder(vector<int> zOrders);
		vector<int> _getMatchedGroupIndices(string groups);


		static vector<int> _matchControls(const vector<BaseControl*>& controls, string groups, string delim, bool matchAll);

		ofTrueTypeFont _defaultFont;
		ModifierKeyHeld _heldModifiers;

		float _lastControlSpacing;
		CG::Dir _lastControlDirection;
		bool _redrawRequested;
	};

	template <typename T>
	T& ManagerContainer::getControl(std::string name, std::string groups) {
		return dynamic_cast<T&>(this->getControl(name, groups));
	}

	template <typename T>
	T& ManagerContainer::createControl(ofRectangle boundingBox, string name, string groups) {
		_controls.push_back(new T());

		_controls.back()->setGroupDelimiter(this->_groupDelimiter);
		_controls.back()->setFont(&_defaultFont);
		_controls.back()->setBoundingBox(boundingBox);
		_controls.back()->name = name;
		_controls.back()->addGroups(groups);

		return dynamic_cast<T&>(*(_controls.back()));
	}

	template <typename T>
	T& ManagerContainer::createControl(float spacing, Dir directionFromBoundingBox, ofRectangle boundingBox,
							  std::string name, std::string groups, float width, float height) {

		_lastControlSpacing = spacing;
		if (directionFromBoundingBox != Dir::USE_PREVIOUS_DIRECTION) {
			_lastControlDirection = directionFromBoundingBox;
		}

		switch (_lastControlDirection) {
		case Dir::DOWN:
			boundingBox.y = boundingBox.y + boundingBox.height + spacing;
			break;
		case Dir::UP:
			boundingBox.y = boundingBox.y - height - spacing;
			break;
		case Dir::RIGHT:
			boundingBox.x = boundingBox.x + boundingBox.width + spacing;
			break;
		case Dir::LEFT:
			boundingBox.x = boundingBox.x - width - spacing;
			break;
		}

		boundingBox.width = width;
		boundingBox.height = height;

		return createControl<T>(boundingBox, name, groups);

	}

	template <typename T>
	T& ManagerContainer::createControl(Dir directionFromBB, ofRectangle BB, std::string name, std::string groups, float width, float height) {
		return createControl<T>(_lastControlSpacing, directionFromBB, BB, name, groups, width, height);
	}

	template <typename T>
	T& ManagerContainer::createControl(Dir directionFromBB, ofRectangle BB, std::string name, std::string groups) {
		ofRectangle lastBB = getLastControl().getBoundingBox();
		return createControl<T>(_lastControlSpacing, directionFromBB, BB, name, groups, lastBB.width, lastBB.height);
	}



	template <typename T>
	T& ManagerContainer::createControl(float spacing, Dir directionFromLast, std::string name, std::string groups, float width, float height) {
		ofRectangle lastBB = getLastControl().getBoundingBox();
		return createControl<T>(spacing, directionFromLast, lastBB, name, groups, width, height);
	}

	template <typename T>
	T& ManagerContainer::createControl(float spacing, Dir directionFromLast, std::string name, std::string groups) {
		ofRectangle lastBB = getLastControl().getBoundingBox();
		return createControl<T>(spacing, directionFromLast, lastBB, name, groups, lastBB.width, lastBB.height);
	}

	template <typename T>
	T& ManagerContainer::createControl(float spacing, std::string name, std::string groups) {
		return createControl<T>(spacing, Dir::USE_PREVIOUS_DIRECTION, name, groups);
	}

	template <typename T>
	T& ManagerContainer::createControl(std::string name, std::string groups) {
		return createControl<T>(Dir::USE_PREVIOUS_DIRECTION, name, groups);
	}

	template <typename T>
	T& ManagerContainer::createControl(Dir directionFromLast, std::string name, std::string groups) {
		ofRectangle lastBB = getLastControl().getBoundingBox();
		return createControl<T>(_lastControlSpacing, directionFromLast, lastBB, name, groups, lastBB.width, lastBB.height);
	}

	template <typename T>
	T& ManagerContainer::createControl(Dir directionFromLast, std::string name, std::string groups, float width, float height) {
		ofRectangle lastBB = getLastControl().getBoundingBox();
		return createControl<T>(_lastControlSpacing, directionFromLast, lastBB, name, groups, width, height);
	}

}