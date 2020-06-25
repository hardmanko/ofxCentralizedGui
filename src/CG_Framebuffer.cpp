#include "CG_Framebuffer.h"

namespace CG {

	void Framebuffer::setup(int msaaSamples, int format) {
		_msaaSamples = msaaSamples;
		_imageFormat = format;
	}

	void Framebuffer::setBoundingBox(ofRectangle bb) {
		//Only reallocate if the size has changed
		if ((boundingBox.width != bb.width) || (boundingBox.height != bb.height)) {
			_fbo.allocate(bb.width, bb.height, _imageFormat, _msaaSamples);
		}

		boundingBox = bb;
	}

	void Framebuffer::draw(void) {
		ofSetColor(255);
		_fbo.draw(this->getBoundingBox().x, this->getBoundingBox().y);
	}

	void Framebuffer::beginRendering(void) {
		_fbo.begin();
	}

	void Framebuffer::endRendering(void) {
		_fbo.end();
	}

} // namespace CG