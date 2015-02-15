#pragma once

#include "ofRectangle.h"

typedef enum {
	UIC_STANDARD,
	UIC_HAND,
	UIC_TEXT_SELECT,
	UIC_HOURGLASS
} UI_cursorIds;

namespace CG {

	class BaseControl;

	void setCursor (UI_cursorIds cursorId);
	ofRectangle shrinkRectangle(ofRectangle r, float amount);
	ofRectangle growRectangle(ofRectangle r, float amount);

	template <typename T>
	T* convertControlType(BaseControl* base) {
		T* p = dynamic_cast<T*>(base);
		if (!p) {
			ofLogError("CG") << "Control type conversion failed.";
		}
		return p;
	}

	int getShiftedChar(int c);
	bool isShiftableChar(int c);

	vector<string> splitGroupString(string groupNames, string delimiter);

	//ofRectangle getGroupBoundingBox(const std::vector<BaseControl*>& controls);
}