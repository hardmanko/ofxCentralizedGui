#include "CG_LineGraphic.h"

namespace CG {

	void LineGraphic::draw(void) {
		ofEnableAlphaBlending();
		ofSetLineWidth(_thickness);
		ofSetColor(colors["background"]);
		ofLine(_x1, _y1, _x2, _y2);
		ofDisableAlphaBlending();
	}

	void LineGraphic::setEndpoints(float x1, float y1, float x2, float y2) {
		_x1 = x1;
		_y1 = y1;
		_x2 = x2;
		_y2 = y2;

		ofRectangle bb;
		bb.x = min(_x1, _x2);
		bb.y = min(_y1, _y2);
		bb.width = abs(_x1 - _x2);
		bb.height = abs(_y1 - _y2);
		setBoundingBox(bb);

		notifyControlNeedsRedraw();
	}

	void LineGraphic::setStart(float x, float y) {
		setEndpoints(x, y, _x2, _y2);
	}

	void LineGraphic::setEnd(float x, float y) {
		setEndpoints(_x1, _y1, x, y);
	}

}