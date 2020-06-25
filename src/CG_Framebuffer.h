#include "CG_BaseControl.h"

#include "ofFbo.h"

namespace CG {

	class Framebuffer : public BaseControl {
	public:

		Framebuffer(void) :
			_msaaSamples(4),
			_imageFormat(GL_RGBA)
		{}

		void setup(int msaaSamples, int format = GL_RGBA);

		virtual void setBoundingBox(ofRectangle bb) override;

		virtual void draw(void) override;

		void beginRendering(void);
		// render your stuff
		void endRendering(void);

		CG::ControlType getType(void) const override { return CG::ControlType::FRAMEBUFFER; };

	protected:
		ofFbo _fbo;

		int _msaaSamples;
		int _imageFormat;
	};

}