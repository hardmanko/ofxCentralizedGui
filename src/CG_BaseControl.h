#pragma once

#include "ofMain.h"
#include "ofColor.h"
#include "ofRectangle.h"
#include "ofTrueTypeFont.h"
#include "ofGraphics.h"
#include "ofEvents.h"

#include <string>
#include <map>
#include <set>
#include <memory>

#include "CG_Typedefs.h"

#include "CG_Util.h"

namespace CG {

	class Manager;
	class ManagerContainer;
	class Container_experimental;

	class BaseControl {
	public:
		BaseControl();

		void setGroupDelimiter(std::string delim);
		void setCallback(NewDataCallbackType cb);

		template <typename T> T* typecast(void) {
			return convertControlType<T>(this);
		}

		virtual void draw(void);

		virtual CG::ControlType getType(void) const { return CG::ControlType::NO_TYPE; };

		//Should these be private, only seen by Manager due to friendship?
		virtual CG::InteractionResult mouseButtonEvent(const MouseButtonEvent &ev);
		virtual CG::InteractionResult mouseMovedEvent(int x, int y);
		virtual CG::InteractionResult keyEvent(KeyEvent ev);

		virtual ofRectangle getBoundingBox(void) const { return this->boundingBox; };
		virtual bool inside(float x, float y) const { return boundingBox.inside(x, y); };

		virtual void setBoundingBox(ofRectangle newBounds);
		virtual void setLocation(float x, float y);
		virtual void setX(float x);
		virtual void setY(float y);
		virtual void setWidth(float width);
		virtual void setHeight(float height);

		virtual void setFont(ofTrueTypeFont *font);


		std::string name;
		bool nameContains(std::string s);

		std::set<std::string> groups;
		std::string getGroupString(void) const;
		bool hasGroup(std::string groupName) const;
		bool hasGroups(std::string groupNames) const;
		bool hasGroups(std::vector<std::string> groupNames) const;
		void addGroups(std::string groupNames);
		void removeGroup(std::string groupName);
		void clearGroups(void);

		virtual void setActiveState(bool active);
		virtual void setEnabledState(bool enabled);
		virtual void setHoveredState(bool hovered);
		virtual void setVisibleState(bool visible);

		virtual bool isActive(void) const;
		virtual bool isEnabled(void) const;
		virtual bool isHovered(void) const;
		virtual bool isVisible(void) const;

		int zOrder;
		float edgeWidth;

		std::map<std::string, ofColor> colors;

		void notifyControlNeedsRedraw(void);

		ofTrueTypeFont* getFont(void);

	protected:

		ofRectangle boundingBox;

		ofTrueTypeFont *textFont;
		void drawString(std::string s, int x, int y);
		ofRectangle getStringBoundingBox(std::string s, int x = 0, int y = 0);



		void notifyCallback(void) {
			if (_callback) {
				_callback(this);
			}
		}

	private:
		friend class CG::Manager;
		friend class CG::ManagerContainer;

		friend class CG::Container_experimental;
		CG::Container_experimental* _container;

		bool redrawRequested;
		

		std::string _groupDelimiter;
		NewDataCallbackType _callback;

		//Shouldn't these be private, given that there are now getters and setters for them?
		bool _controlActive;
		bool _controlEnabled;
		bool _controlHovered;
		bool _controlVisible;
	};

}







