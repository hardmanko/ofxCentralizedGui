#include "CG_BaseControl.h"

#include "ofFbo.h"

namespace CG {

	class Framebuffer : public BaseControl {
	public:

		Framebuffer(void) :
			_msaaSamples(4),
			_imageFormat(GL_RGBA)
		{}

		void setup(int msaaSamples, int format = GL_RGBA) {
			_msaaSamples = msaaSamples;
			_imageFormat = format;
		}

		void setBoundingBox(ofRectangle bb) override {
			//Only reallocate if the size has changed
			if ((boundingBox.width != bb.width) || (boundingBox.height != bb.height)) {
				_fbo.allocate(bb.width, bb.height, _imageFormat, _msaaSamples);
			}

			boundingBox = bb;
		}

		void draw(void) override {
			ofSetColor(255);
			_fbo.draw(this->getBoundingBox().x, this->getBoundingBox().y);
		}

		void beginRendering(void) {
			_fbo.begin();
		}

		void endRendering(void) {
			_fbo.end();
		}

		CG::ControlType getType(void) const override { return CG::ControlType::FRAMEBUFFER; };

	private:
		ofFbo _fbo;

		int _msaaSamples;
		int _imageFormat;
	};

}