#include "CG_RectangleGraphic.h"

namespace CG {

	RectangleGraphic::RectangleGraphic(void)
	{}

	void RectangleGraphic::draw(void) {
		if (edgeWidth != 0) {
			ofSetColor(colors["edge"]);
			ofRect(boundingBox);

			ofSetColor(colors["background"]);
			ofRect(boundingBox.x + edgeWidth, boundingBox.y + edgeWidth, boundingBox.width - edgeWidth * 2, boundingBox.height - edgeWidth * 2);
		} else {
			ofSetColor(colors["background"]);
			ofRect(boundingBox);
		}
	}

}