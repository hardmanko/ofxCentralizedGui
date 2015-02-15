#include "CG_TextGraphic.h"

namespace CG {

	TextGraphic::TextGraphic(void) :
		_textClickable(false),
		_textHeightAboveBaseLine(0)
	{
	}

	void TextGraphic::draw(void) {
		if (_textClickable && isHovered()) {
			ofSetColor(colors["hovered"]);
		} else {
			ofSetColor(colors["text"]);
		}
		this->drawString(_text, boundingBox.x, boundingBox.y + _textHeightAboveBaseLine);
	}

	void TextGraphic::setLabel(string text) {
		_text = text;

		ofRectangle temp = getStringBoundingBox(_text, 0, 0);
		setWidth(temp.width);
		setHeight(temp.height);

		_textHeightAboveBaseLine = -temp.y;
	}

	void TextGraphic::setFont(ofTrueTypeFont *font) {
		textFont = font;
		setLabel(_text);
	}

	bool TextGraphic::inside(float x, float y) const {
		ofRectangle tempBox = boundingBox;
		tempBox.y -= tempBox.height;
		return tempBox.inside(x, y);
	}

	ofRectangle TextGraphic::getBoundingBox(void) const {
		if (_text == "") {
			return boundingBox;
		}
		ofRectangle stringBB = textFont->getStringBoundingBox(_text, boundingBox.x, boundingBox.y + _textHeightAboveBaseLine);
		return stringBB;

	}


	CG::InteractionResult TextGraphic::mouseMovedEvent(int x, int y) {
		if (this->inside(x, y)) {
			setHoveredState(true);
			if (_textClickable) {
				setCursor(UIC_HAND);
			}
		} else {
			setHoveredState(false);
		}
		return CG::InteractionResult();
	}

	//Plan on doing something with this.
	CG::InteractionResult TextGraphic::mouseButtonEvent(const MouseButtonEvent &ev) {
		return CG::InteractionResult();
	}

}