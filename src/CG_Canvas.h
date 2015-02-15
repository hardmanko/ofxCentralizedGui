#pragma once

#include "ofTexture.h"

#include "CG_BaseControl.h"

namespace CG {

	class Canvas : public BaseControl {
	public:

		Canvas(void);

		void draw(void) override;

		void setBoundingBox(ofRectangle bb) override;

		CG::ControlType getType(void) const override { return CG::ControlType::CANVAS; };

		//Add ability to draw directly to internal fbo.

		void clear(void);
		void clear(ofColor col);
		void clear(unsigned char r, unsigned char g, unsigned char b, unsigned char alpha);

		void refresh(void);
		void moveTexture(int xMovement, int yMovement);

		ofTexture &getTexture(void) { return _texture; };

		void setPixel(int xPos, int yPos, ofColor col);
		void setPixel(int xPos, int yPos, unsigned char r, unsigned char g, unsigned char b, unsigned char alpha);
		void setPixelAbsolute(int xPos, int yPos, ofColor col);
		void setPixelAbsolute(int xPos, int yPos, unsigned char r, unsigned char g, unsigned char b, unsigned char alpha);

		ofColor getPixel(int xPos, int yPos);

	private:

		void _allocate(int width, int height);
		bool _dataAllocated;
		unsigned char *_data;

		ofTexture _texture;

	};

}