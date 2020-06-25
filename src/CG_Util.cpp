#include "CG_Util.h"

#include "ofConstants.h"

#include "CG_BaseControl.h"


#ifdef TARGET_WIN32
#include "Windows.h"
#endif

namespace CG {
	/*
	Eventually expand this function to work on other OSs.
	*/
	void setCursor(UI_cursorIds cursorId) {

		switch (cursorId) {

		case UIC_HAND:
#ifdef TARGET_WIN32
			SetCursor(LoadCursor(NULL, IDC_HAND));
#endif
			break;
		case UIC_TEXT_SELECT:
#ifdef TARGET_WIN32
			SetCursor(LoadCursor(NULL, IDC_IBEAM));
#endif
			break;
		case UIC_HOURGLASS:
#ifdef TARGET_WIN32
			SetCursor(LoadCursor(NULL, IDC_WAIT));
#endif
			break;
		case UIC_STANDARD:
			break;

		};
	}

	ofRectangle shrinkRectangle(ofRectangle r, float amount) {
		return ofRectangle(r.x + amount, r.y + amount, r.width - 2 * amount, r.height - 2 * amount);
	}

	ofRectangle growRectangle(ofRectangle r, float amount) {
		return shrinkRectangle(r, -amount);
	}

	map<int, int>& getShiftedCharMap(void) {
		static map<int, int> charMap = []() -> map<int, int> {
			map<int, int> characterMap;
			characterMap['`'] = '~';
			characterMap['1'] = '!';
			characterMap['2'] = '@';
			characterMap['3'] = '#';
			characterMap['4'] = '$';
			characterMap['5'] = '%';
			characterMap['6'] = '^';
			characterMap['7'] = '&';
			characterMap['8'] = '*';
			characterMap['9'] = '(';
			characterMap['0'] = ')';
			characterMap['-'] = '_';
			characterMap['='] = '+';
			characterMap['['] = '{';
			characterMap[']'] = '}';
			characterMap['\\'] = '|';
			characterMap[';'] = ':';
			characterMap['\''] = '\"';
			characterMap[','] = '<';
			characterMap['.'] = '>';
			characterMap['/'] = '?';
			return characterMap;
		}();
		return charMap;
	}

	bool isShiftableChar(int c) {
		return getShiftedCharMap().find(c) != getShiftedCharMap().end();
	}

	int getShiftedChar(int c) {

		if (isShiftableChar(c)) {
			return getShiftedCharMap()[c];
		}

		return c;
	}

	bool isModifierKey(int c) {
		return (c & OF_KEY_CONTROL) == OF_KEY_CONTROL ||
			(c & OF_KEY_ALT) == OF_KEY_ALT ||
			(c & OF_KEY_SHIFT) == OF_KEY_SHIFT ||
			(c & OF_KEY_SUPER) == OF_KEY_SUPER;
	}


	ofRectangle getGroupBoundingBox(const std::vector<BaseControl*>& controls) {
		if (controls.size() == 0) {
			return ofRectangle();
		}

		std::vector<BaseControl*>::const_iterator it = controls.begin();
		ofRectangle inclusiveBoundingBox = (*it)->getBoundingBox();
		it++;

		for (/* */; it != controls.end(); it++) {
			inclusiveBoundingBox.growToInclude((*it)->getBoundingBox());
		}

		return inclusiveBoundingBox;
	}

	std::vector<std::string> splitGroupString(std::string groupNames, std::string delimiter) {
		return ofSplitString(groupNames, delimiter, true, true);
	}

}