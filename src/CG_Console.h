#pragma once

#include "CG_BaseControl.h"

namespace CG {

	class Console : public BaseControl {
	public:
		Console(void);

		void draw(void) override;

		//ofRectangle getBoundingBox (void);
		void setBoundingBox(ofRectangle newBoundingBox) override;

		//void setFont (string fontName, int fontSize);
		void setFont(ofTrueTypeFont *font) override;

		void setMaxVisibleLines(int lines);
		void setMaxLineLength(int characters);

		void handleChar(char c);
		void handleString(string s);
		void handleString(char *s);

		template <typename T>
		void printObject(const T& t) {
			this->handleString(ofToString<T>(t));
		}

		void clear(void);

		CG::ControlType getType(void) const override { return CG::ControlType::CONSOLE; };

	private:

		bool _currentLineBlank;

		char _lastChar;
		deque<string> _lineStrings;

		int _maxVisibleLines;
		int _maxLineLength;

	};

}