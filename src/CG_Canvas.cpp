#include "CG_Canvas.h"

namespace CG {

	Canvas::Canvas(void) :
		_dataAllocated(false),
		_data(NULL)
	{
	}

	void Canvas::draw(void) {
		ofSetColor(255, 255, 255); //Annoying thing you have to do to get tectures to show up correctly.
		//ofEnableAlphaBlending(); //In case there is any alpha stuff in the texture.
		_texture.draw(boundingBox.x, boundingBox.y);
		//ofDisableAlphaBlending();
	}

	void Canvas::setBoundingBox(ofRectangle bb) {
		this->boundingBox = bb;

		this->_allocate(bb.width, bb.height);
	}

	void Canvas::_allocate(int width, int height) {
		try {

			if (_dataAllocated) {
				delete[] _data;
				_data = NULL;
				_dataAllocated = false;
			}

		}
		catch (...) {
			_dataAllocated = false;
		}

		bool allocationSuccessful = false;
		try {

			_data = new unsigned char[width * height * 4];
			memset(_data, 0, width * height * 4);
			_texture.allocate(width, height, GL_RGBA);
			_texture.loadData(_data, width, height, GL_RGBA);
			allocationSuccessful = true;

			boundingBox.width = width;
			boundingBox.height = height;

		}
		catch (...) {
			allocationSuccessful = false;
		}

		if (!allocationSuccessful) {
			boundingBox.width = 0;
			boundingBox.height = 0;
		}

		_dataAllocated = allocationSuccessful;
	}

	void Canvas::setPixel(int xPos, int yPos, unsigned char r, unsigned char g, unsigned char b, unsigned char alpha) {
		if (xPos < 0 || xPos >= boundingBox.width || yPos < 0 || yPos >= boundingBox.height) {
			return;
		}

		int startPos = (yPos * boundingBox.width + xPos) * 4;
		_data[startPos + 0] = r;
		_data[startPos + 1] = g;
		_data[startPos + 2] = b;
		_data[startPos + 3] = alpha;
	}

	void Canvas::setPixel(int xPos, int yPos, ofColor col) {
		setPixel(xPos, yPos, col.r, col.g, col.b, col.a);
	}

	void Canvas::setPixelAbsolute(int xPos, int yPos, unsigned char r, unsigned char g, unsigned char b, unsigned char alpha) {
		setPixel(xPos - boundingBox.x, yPos - boundingBox.y, r, g, b, alpha);
	}

	void Canvas::setPixelAbsolute(int xPos, int yPos, ofColor col) {
		setPixelAbsolute(xPos, yPos, col.r, col.g, col.b, col.a);
	}

	void Canvas::clear(void) {
		memset(_data, 0, boundingBox.width * boundingBox.height * 4);
		refresh();
	}

	void Canvas::clear(ofColor col) {
		clear(col.r, col.g, col.b, col.a);
	}

	void Canvas::clear(unsigned char r, unsigned char g, unsigned char b, unsigned char alpha) {
		for (int i = 0; i < getBoundingBox().width * getBoundingBox().height; i++) {
			_data[i + 0] = r;
			_data[i + 1] = g;
			_data[i + 2] = b;
			_data[i + 3] = alpha;
		}
	}



	void Canvas::moveTexture(int xMovement, int yMovement) {
		int charsRight = xMovement * 4;
		int charsDown = yMovement * boundingBox.width * 4;

		if ((abs(charsDown) >= (boundingBox.width * boundingBox.height * 4)) || abs(charsRight) >= (boundingBox.width * boundingBox.height * 4)) {
			memset(_data, 0, (boundingBox.width * boundingBox.height * 4));
			return;
		}

		if (charsRight < 0) {
			charsRight = -charsRight;

			memcpy(_data, _data + charsRight, (boundingBox.width * boundingBox.height * 4) - charsRight);

			for (int row = 0; row < boundingBox.height; ++row) {
				memset(_data + ((row + 1) * (int)boundingBox.width * 4) - charsRight, 0, charsRight);
			}
		} else if (charsRight > 0) {
			//There is something wrong with my understanding of this logically, but it works. 
			//I think it should use memmove, then zero the data. But that doesn't work.
			for (int row = 0; row < boundingBox.height; ++row) {
				memset(_data + ((row + 1) * (int)boundingBox.width * 4) - charsRight, 0, charsRight);
			}

			memcpy(_data + charsRight, _data, (boundingBox.width * boundingBox.height * 4) - charsRight);
		}

		if (charsDown > 0) {
			memcpy(_data + charsDown, _data, (boundingBox.width * boundingBox.height * 4) - charsDown);
			memset(_data, 0, charsDown);
		} else if (charsDown < 0) {
			charsDown = -charsDown;
			memcpy(_data, _data + charsDown, (boundingBox.width * boundingBox.height * 4) - charsDown);
			memset(_data + (int)(boundingBox.width * boundingBox.height * 4) - charsDown, 0, charsDown);
		}
	}

	void Canvas::refresh(void) {
		_texture.loadData(_data, boundingBox.width, boundingBox.height, GL_RGBA);
		notifyControlNeedsRedraw();
	}

}