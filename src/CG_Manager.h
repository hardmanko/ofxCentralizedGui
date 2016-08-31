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


namespace CG {

	//You could add DONTFOLLOW versions of these. Or a "backtrack" function.
	enum class Dir {
		UP,
		DOWN,
		RIGHT,
		LEFT,
		USE_PREVIOUS_DIRECTION
	};

	class Manager {
	public:
		Manager();
		~Manager();

		void setDelimiter(std::string delim);
		std::string getDelimiter(void);

		void setDefaultFont(std::string fontName, int fontSize, bool antialiased, bool fullCharSet, float simplifyAmount, int dpi);
		ofTrueTypeFont& getDefaultFont(void);

		void pollEvents(void);
		int availableInteractions(void);
		CG::InteractionResult getNextInteraction(void);
		std::queue<CG::InteractionResult> copyAllInteractions(void) const;

		bool redrawRequestedSincelastCheck(void);
		void draw(void);

		template <typename T> T& createControl(ofRectangle thisBB, std::string name, std::string groups = "");

		template <typename T> T& createControl(float spacing, Dir directionFromBB, ofRectangle otherBB, std::string name, std::string groups, float width, float height);
		template <typename T> T& createControl(float spacing, Dir directionFromBB, ofRectangle otherBB, std::string name, std::string groups);
		template <typename T> T& createControl(Dir directionFromBB, ofRectangle otherBB, std::string name, std::string groups, float width, float height);
		template <typename T> T& createControl(Dir directionFromBB, ofRectangle otherBB, std::string name, std::string groups);

		template <typename T> T& createControl(float spacing, Dir directionFromLast, std::string name, std::string groups, float width, float height);
		template <typename T> T& createControl(float spacing, Dir directionFromLast, std::string name, std::string groups = "");
		template <typename T> T& createControl(float spacing, std::string name, std::string groups = "");

		template <typename T> T& createControl(std::string name, std::string groups = "");
		template <typename T> T& createControl(std::string name, std::string groups, float width, float height);

		template <typename T> T& createControl(Dir directionFromLast, std::string name, std::string groups = "");
		template <typename T> T& createControl(Dir directionFromLast, std::string name, std::string groups, float width, float height);

		void deleteControlsInGroup(std::string groups);
		void deleteControl(std::string name, std::string groups);

		BaseControl& getLastControl(void);
		template <typename T> T& getControl(std::string name, std::string groups = "*");
		BaseControl& getControl(std::string name, std::string groups = "*");

		std::vector<BaseControl*> getControls(std::string groups);
		std::vector<BaseControl*> getControls(CG::ControlType type);

		void moveControls(int xChange, int yChange);
		void moveControlsInGroup(std::string group, int xChange, int yChange);

		void applyFunction(std::string groups, std::function<void(BaseControl&)> fn);
		void applyFunction(ControlType type, std::function<void(BaseControl&)> fn);
		void applyFunction(std::string groups, ControlType type, std::function<void(BaseControl&)> fn);

		void setGroupEdgeWidth(std::string groups, float edgeWidth);
		void setGroupZOrder(std::string groups, int zOrder);
		void setGroupVisibility(std::string groups, bool visible);
		void setGroupEnabledState(std::string groups, bool enabled);
		void setGroupWidth(std::string groups, float width);
		void setGroupHeight(std::string groups, float height);
		void setGroupFont(std::string groups, ofTrueTypeFont *font);
		void setGroupComponentColor(std::string groups, string controlComponent, ofColor color);

		ofRectangle getGroupBoundingBox(std::string groups);

		void enableKeyboardShortcuts(bool enable);


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

		std::queue <CG::InteractionResult> _controlInteractions;

		std::vector<BaseControl*> _controls;

		std::vector<unsigned int> _convertZOrderToDrawOrder(std::vector<int> zOrders);
		std::vector<int> _getMatchedGroupIndices(std::string groups);


		static std::vector<int> _matchControls(const std::vector<BaseControl*>& controls, std::string groups, std::string delim, bool matchAll);

		ofTrueTypeFont _defaultFont;
		ModifierKeyHeld _heldModifiers;

		float _lastControlSpacing;
		CG::Dir _lastControlDirection;
		bool _redrawRequested;

		bool _keyboardShortcutsEnabled;
	};

	template <typename T>
	T& Manager::getControl(std::string name, std::string groups) {
		return dynamic_cast<T&>(this->getControl(name, groups));
	}

	template <typename T>
	T& Manager::createControl(ofRectangle thisBB, std::string name, std::string groups) {
		_controls.push_back(new T());

		_controls.back()->setGroupDelimiter(this->_groupDelimiter);
		_controls.back()->setFont(&_defaultFont);
		_controls.back()->setBoundingBox(thisBB);
		_controls.back()->name = name;
		_controls.back()->addGroups(groups);

		return dynamic_cast<T&>(*(_controls.back()));
	}

	template <typename T> 
	T& Manager::createControl(float spacing, Dir directionFromBB, ofRectangle otherBB,
							  std::string name, std::string groups, float width, float height) {

		_lastControlSpacing = spacing;
		if (directionFromBB != Dir::USE_PREVIOUS_DIRECTION) {
			_lastControlDirection = directionFromBB;
		}

		switch (_lastControlDirection) {
		case Dir::DOWN:
			otherBB.y = otherBB.y + otherBB.height + spacing;
			break;
		case Dir::UP:
			otherBB.y = otherBB.y - height - spacing;
			break;
		case Dir::RIGHT:
			otherBB.x = otherBB.x + otherBB.width + spacing;
			break;
		case Dir::LEFT:
			otherBB.x = otherBB.x - width - spacing;
			break;
		}

		otherBB.width = width;
		otherBB.height = height;

		return createControl<T>(otherBB, name, groups);

	}

	template <typename T> 
	T& Manager::createControl(float spacing, Dir directionFromBB, ofRectangle otherBB, std::string name, std::string groups) {
		ofRectangle lastBB = getLastControl().getBoundingBox();
		return createControl<T>(spacing, directionFromBB, otherBB, name, groups, lastBB.width, lastBB.height);
	}

	template <typename T>
	T& Manager::createControl(Dir directionFromBB, ofRectangle otherBB, std::string name, std::string groups, float width, float height) {
		return createControl<T>(_lastControlSpacing, directionFromBB, otherBB, name, groups, width, height);
	}

	template <typename T>
	T& Manager::createControl(Dir directionFromBB, ofRectangle otherBB, std::string name, std::string groups) {
		ofRectangle lastBB = getLastControl().getBoundingBox();
		return createControl<T>(_lastControlSpacing, directionFromBB, otherBB, name, groups, lastBB.width, lastBB.height);
	}

	template <typename T> 
	T& Manager::createControl(float spacing, Dir directionFromLast, std::string name, std::string groups, float width, float height) {
		ofRectangle lastBB = getLastControl().getBoundingBox();
		return createControl<T>(spacing, directionFromLast, lastBB, name, groups, width, height);
	}
	
	template <typename T> 
	T& Manager::createControl(float spacing, Dir directionFromLast, std::string name, std::string groups) {
		ofRectangle lastBB = getLastControl().getBoundingBox();
		return createControl<T>(spacing, directionFromLast, lastBB, name, groups, lastBB.width, lastBB.height);
	}

	template <typename T> 
	T& Manager::createControl(float spacing, std::string name, std::string groups) {
		return createControl<T>(spacing, Dir::USE_PREVIOUS_DIRECTION, name, groups);
	}

	template <typename T> 
	T& Manager::createControl(std::string name, std::string groups) {
		return createControl<T>(Dir::USE_PREVIOUS_DIRECTION, name, groups);
	}

	template <typename T> 
	T& Manager::createControl(std::string name, std::string groups, float width, float height) {
		return createControl<T>(Dir::USE_PREVIOUS_DIRECTION, name, groups, width, height);
	}

	template <typename T> 
	T& Manager::createControl(Dir directionFromLast, std::string name, std::string groups) {
		ofRectangle lastBB = getLastControl().getBoundingBox();
		return createControl<T>(_lastControlSpacing, directionFromLast, lastBB, name, groups, lastBB.width, lastBB.height);
	}

	template <typename T> 
	T& Manager::createControl(Dir directionFromLast, std::string name, std::string groups, float width, float height) {
		ofRectangle lastBB = getLastControl().getBoundingBox();
		return createControl<T>(_lastControlSpacing, directionFromLast, lastBB, name, groups, width, height);
	}

}