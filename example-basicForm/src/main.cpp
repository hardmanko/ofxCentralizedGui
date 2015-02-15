#include "ofMain.h"
#include "basicForm.h"

//========================================================================
int main( ){
	ofSetupOpenGL(1024,768,OF_WINDOW);

	ofRunApp(new basicFormApp());
}
