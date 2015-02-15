#pragma once

#include <string>
#include <vector>
#include <functional>

namespace CG {

	typedef long ControlState;

	class BaseControl; //Forward declaration for use in UI_interactionResultStruct

	enum class ControlType {
		NO_TYPE,
		BUTTON,
		CANVAS,
		CHECK_BOX,
		CONSOLE,
		CONTAINER,
		DRAG_BOX,
		DROPDOWN,
		FRAMEBUFFER,
		LINE,
		RECTANGLE,
		SLIDER,
		TEXT,
		TEXT_BOX
	};

	enum class InteractionCategory {
		STANDARD,
		KEYBOARD_SHORTCUT
	};

	struct MouseButtonEvent {
		int button;
		int x;
		int y;
		enum {
			PRESSED,
			RELEASED
		} eventType;
	};

	struct ModifierKeyHeld {
		ModifierKeyHeld() :
			ctrl(false),
			alt(false),
			shift(false),
			meta(false)
		{};
		bool ctrl;
		bool alt;
		bool shift;
		bool meta;
	};

	enum class KeyEventType {
		PRESSED,
		RELEASED,
		REPEAT
	};

	struct KeyEvent {
		KeyEvent() :
			key(0)
		{};

		ModifierKeyHeld heldModifiers;

		int key;
		KeyEventType eventType;

	};


	struct InteractionResult {

		InteractionResult() :
			interactionCategory(InteractionCategory::STANDARD),
			control(nullptr),
			contolStateChanged(false),
			newDataAvailable(false),
			newControlState(0)

		{};

		InteractionCategory interactionCategory;

		BaseControl *control;

		bool contolStateChanged;

		bool newDataAvailable;
		ControlState newControlState;
		std::string newStateLabel;
	};

	typedef std::function<void(BaseControl*)> NewDataCallbackType;

}