#include "controlGroups.h"

using namespace CG;

vector<BaseControl*> lastSelectedGroup;
ofColor triangleColor(0);

//--------------------------------------------------------------
void testApp::setup(){

	//We start by making some controls. The first two controls are both in the group "green".
	UI.createControl<Pushbutton>(ofRectangle(50, 50, 150, 30), "button1", "green").setLabel("Button1:green");
	UI.createControl<Pushbutton>(2, Dir::DOWN, "button2", "green").setLabel("Button2:green");

	//Now we have a control in the "red" group.
	UI.createControl<Pushbutton>(Dir::DOWN, "button3", "red").setLabel("Button3:red");

	//You can give a control multiple control groups. You give the names in a string, delimited by a 
	//comma (or other delimiter if you use CG::Manager::setDelimiter() to change the default).
	UI.createControl<Pushbutton>(Dir::DOWN, "button4", "red, green").setLabel("Button4:red, green");
	UI.createControl<Pushbutton>(Dir::DOWN, "button5", "blue, green").setLabel("Button5:blue, green");

	//This complex control creation should be read as: Create this control 100 pixels to the RIGHT of the bounding box of the control named 
	//"button1" with the group "green". It should have the name "console" and the group "consoleGroup". It should be 400 by 200 pixels.
	UI.createControl<Console>(100, Dir::RIGHT, UI.getControl("button1", "green").getBoundingBox(), "console", "consoleGroup", 400, 200);

	//Now that we have some groups, we can do things to them as a function of which groups they belong to.
	UI.setGroupComponentColor("green", "background", ofColor::green); //"green" controls get a green background.
	UI.setGroupComponentColor("red", "edge", ofColor::red); //"red" controls get a red edge.
	UI.setGroupComponentColor("blue", "text", ofColor::blue); //"blue" controls get blue text.
	UI.setGroupComponentColor("green, red", "hovered", ofColor::yellow); //You can select combinations of groups, in this case only 
		//controls that have both the green and red groups (note that group name order is ignored: it was originally specified as "red, green").

	UI.setGroupEdgeWidth("*", 2); // Asterisk (*) is a wildcard: It matches all controls.

	//For anything that you can't do with built-in functions of a CG::Manager, you can do with applyFunction().
	//The user-provided function is called for every control that matches the given control groups.
	UI.applyFunction("red", [](BaseControl& c) { c.setWidth(c.getBoundingBox().width - 10); });
}

//--------------------------------------------------------------
void testApp::update(){
	while (UI.availableInteractions()) {
		InteractionResult res = UI.getNextInteraction();

		if (res.newDataAvailable) {
			//CG::Manager::getControls() returns a vector of CG::BaseControl*, so that you can iterate 
			//over controls or store them for use elsewhere.
			lastSelectedGroup = UI.getControls(res.control->getGroupString());

			if (res.control->hasGroups("blue, green")) {
				triangleColor = ofColor::cyan;
			} else if (res.control->hasGroups("red, green")) {
				triangleColor = ofColor::yellow;
			} else if (res.control->hasGroups("green")) {
				triangleColor = ofColor::green;
			} else if (res.control->hasGroups("red")) {
				triangleColor = ofColor::red;
			} else if (res.control->hasGroups("blue")) {
				triangleColor = ofColor::blue;
			}

			string s = "Control named " + res.control->name + " with groups " + res.control->getGroupString() + " selected.\n";
			UI.getControl<Console>("console", "consoleGroup").handleString(s);

		}
	}
}

//--------------------------------------------------------------
void testApp::draw(){
	ofBackground(60);
	UI.draw();

	//Draw triangles pointing at controls that share groups with the last clicked control.
	ofSetColor(triangleColor);
	for (BaseControl* c : lastSelectedGroup) {
		ofRectangle bb = c->getBoundingBox();
		ofTriangle(bb.getRight() + 30, bb.getCenter().y, bb.getRight() + 50, bb.getTop(), bb.getRight() + 50, bb.getBottom());
	}
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}