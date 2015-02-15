#include "basicForm.h"

using namespace CG;

//--------------------------------------------------------------
void basicFormApp::setup(){
	
	int controlHeight = 20;

	//For the first control, you need to give a bounding box for the control as an ofRectangle.
	//createControl() returns a reference to the newly created control, which is useful for 
	//configuring most types of control.
	Dropdown& ddn = UI.createControl<Dropdown>(ofRectangle(20, 20, 160, controlHeight), "gender");
	//For a dropdown, you need to add in some possible selections.
	ddn.addSelection("Female");
	ddn.addSelection("Male");
	ddn.addSelection("Other");
	
	ddn.setNothingSelectedText("Select gender"); //Set the text that will be displayed when none of the options are selected.
	ddn.setToNothingSelected(); //And set the selection to nothing selected.


	//For controls past the first, you can specify control location in other ways. In this case,
	//the new control will be 3 pixels down from the last control. We can leave out width and height
	//and those values from the previous control will be used.
	UI.createControl<TextBox>(3, Dir::DOWN, "firstName").setLabel("First name");
	//Notice that at the end we use the fact that createControl returns a reference to tag on a function
	//call to the setLabel function for the check box.

	//Here we leave out the spacing between controls. The most recently used spacing will be used.
	//Also note that we went to the right instead of down.
	UI.createControl<TextBox>(Dir::RIGHT, "lastName").setLabel("Last name");

	//Here we do not want to place the control relative to the last control, so we give a bounding 
	//box relative to which this control will be placed. We pick the bounding box of the firstName
	//control. Also, we want to specify a narrow width because it is a check box.
	UI.createControl<CheckBox>(Dir::DOWN, UI.getControl("firstName").getBoundingBox(), "isCommuter", "", 20, controlHeight).setLabel("Commuter?");

	UI.createControl<TextBox>(Dir::DOWN, "age", "", 100, controlHeight).setLabel("Age");

	//You can even sometimes omit the direction that you are moving in from the previous control.
	UI.createControl<Pushbutton>("finishButton").setLabel("Finish");

	UI.createControl<TextGraphic>(10, "infoText").setLabel("See the console for output from the form.");

	//applyFunction is a convenient way to do specific behaviors not otherwise provided by built-in functions of the Manager.
	//In this case, we are selecting all contols that are text boxes and telling them to clear their contents when they are
	//clicked on.
	UI.applyFunction(ControlType::TEXT_BOX, [](BaseControl& c) { c.typecast<TextBox>()->setClearFieldOnActivation(true); });
}

//--------------------------------------------------------------
void basicFormApp::update(){
	while (UI.availableInteractions() > 0) {
		CG::InteractionResult act = UI.getNextInteraction();
		if (act.newDataAvailable) {
			if (act.control->name == "finishButton") {
				//Process the form. Finding out about the current state of different controls is handled differently
				//depending on what type of control it is.

				string gender = UI.getControl<Dropdown>("gender").getCurrentSelectionLabel();

				string name = UI.getControl<TextBox>("firstName").getEntry() + " " + UI.getControl<TextBox>("lastName").getEntry();
				string age = UI.getControl<TextBox>("age").getEntry();

				bool isCommuter = UI.getControl<CheckBox>("isCommuter").isChecked();

				cout << name << endl <<
					"Gender: " << gender << endl <<
					(isCommuter ? "Commuter" : "Not a commuter") << endl <<
					"Age: " << age << endl;

			}
		}
	}
}

//--------------------------------------------------------------
void basicFormApp::draw(){
	UI.draw();
}

//--------------------------------------------------------------
void basicFormApp::keyPressed(int key){

}

//--------------------------------------------------------------
void basicFormApp::keyReleased(int key){

}

//--------------------------------------------------------------
void basicFormApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void basicFormApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void basicFormApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void basicFormApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void basicFormApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void basicFormApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void basicFormApp::dragEvent(ofDragInfo dragInfo){ 

}