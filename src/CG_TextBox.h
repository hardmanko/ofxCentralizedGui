#pragma once

#include "CG_BaseControl.h"

namespace CG {

	class TextBox : public BaseControl {
	public:
		TextBox(void);
		~TextBox(void);

		void draw(void) override;

		CG::InteractionResult mouseButtonEvent(const MouseButtonEvent &ev) override;
		CG::InteractionResult mouseMovedEvent(int x, int y) override;
		CG::InteractionResult keyEvent(KeyEvent ev) override;

		void setFont(ofTrueTypeFont *font) override;
		void setLabel(string label); //This should be renamed to setEntry
		string getEntry(void) { return _currentEntry; };

		CG::ControlType getType(void) const override { return CG::ControlType::TEXT_BOX; };

		void setClearFieldOnActivation(bool clearFieldOnActivation) { _clearFieldOnActivation = clearFieldOnActivation; };

	private:

		bool _clearFieldOnActivation;

		int _cursorDisplayedFrames;
		bool _cursorVisible;
		int _cursorXOffset;
		int _cursorCharacterIndex;

		string _currentEntry; //The current entry is not displayed, per se, because it may be too big to fit in the field.
		string _lastEntry;

		struct {
			int offset;
			int length;
		} _displayInfo;

		struct {
			int offset;
			int length;
		} _highlightedChars;


		int _lineHeight;
		int _linesOfText;

		void _setDisplayedString(string s);
		void _calculateCursorOffset(void);
		void _setCursorFromClick(ofPoint clickLocation);

		float _calculateCharX(int offset);
		void _highlightChars(int offset, int length);
		void _clearHighlight(void);

		void _submitEntry(void);
		void _cancelEntry(void);
		void _activate(void);
		void _deactivate(void);

	};

}
