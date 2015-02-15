#pragma once

#include <limits>

#include "CG_BaseControl.h"

namespace CG {

	class Dropdown : public BaseControl {
	public:

		struct Selection {
			int id;
			std::string label;
		};

		Dropdown();

		void draw(void) override;

		void addSelection(int selectionID, std::string label);
		void addSelection(std::string label);
		void clearSelectionOptions(void);

		void setSelectedOption(int index);
		void setSelectedOptionById(int id);
		void setSelectedOption(std::string selectionLabel);
		void setToNothingSelected(void);
		void setNothingSelectedText(std::string text);

		void setDisplayedText(std::string text);

		std::string getSelectionLabel(int id);
		int getCurrentSelectionId(void);
		std::string getCurrentSelectionLabel(void);

		//Virtual overloads
		CG::ControlType getType(void) const override { return CG::ControlType::DROPDOWN; };

		bool inside(float x, float y) const override;
		void setBoundingBox(ofRectangle newBounds) override;
		void setFont(ofTrueTypeFont *font) override;

		//CG::InteractionResult mouseClickedEvent (int mouseButton, int x, int y);
		CG::InteractionResult mouseButtonEvent(const MouseButtonEvent &ev) override;
		CG::InteractionResult mouseMovedEvent(int x, int y) override;

	private:

		int _getSelectionLineAtLocation(int mouseX, int mouseY);
		void _recalculateDropdownBoxSize(void);

		std::string _displayedText;
		std::string _noSelectionText;

		int _currentSelectionIndex;
		int _highlightedSelectionIndex;
		float _selectionLineHeight;

		ofRectangle _dropdownBoxPosition;
		ofRectangle _dropdownArrowPosition;

		std::vector<Selection> _selectionOptions;

	};

}