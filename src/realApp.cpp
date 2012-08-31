#pragma warning(disable:4819)
#include <cmath>
#include "realApp.h"
#include "formWidget.h"
#include "gateWidget.h"


//--------------------------------------------------------------
formWidget *gui;

ofxBox2dJoint j;
void realApp::setup()
{ 
	ofEnableSmoothing(); 
	ofBackgroundHex(0xfdefc2);
	ofSetLogLevel(OF_LOG_NOTICE);
	ofSetVerticalSync(true);
	
	// Box2d
	box2d.init();
	box2d.setGravity(0, 5);
	//box2d.createGround();
	box2d.createBounds();
	box2d.setFPS(24.0);

	ofxBox2dCircle c;
	c.setPhysics(0.3, 0.3, 0.1);
	c.setup(box2d.getWorld(), (ofGetWidth()/2)+ofRandom(-10, 10), 30, ofRandom(10, 20));
	circles.push_back(c);

	ofxBox2dCircle c2;
	c2.setPhysics(0.3, 0.3, 0.1);
	c2.setup(box2d.getWorld(), (ofGetWidth()/2)+ofRandom(-10, 10), 50, ofRandom(10, 20));
	circles.push_back(c2);

	j.setup(box2d.getWorld(), c.body, c2.body);
	j.setLength(100);
	j.setDamping(0.2);


	cout<<"hearing?" <<endl;
	//gui
	float xInit = OFX_UI_GLOBAL_WIDGET_SPACING; 
    float length = 255-xInit; 
	gui = new formWidget(length+xInit, (ofGetHeight()-200));
 	ofAddListener(gui->newGUIEvent,this,&realApp::guiEvent);
	gui->setup(box2d.getWorld(),500,40);
	gui->box.setVelocity(-10,5);
}


//--------------------------------------------------------------
void realApp::update() {
	float angle=fmod(gui->box.getRotation(),360);
	if(angle>180) angle-=360; else if(angle<-180) angle+=360;
	if(angle>20 || angle<-20) gui->box.body->ApplyTorque(-angle*30.0f);
	box2d.update();	
}

//--------------------------------------------------------------
void realApp::draw() {
	// some circles :)
	ofFill();
	ofSetHexColor(0xc0dd3b);
	int i;
	for(i=0;i<circles.size();++i)
	{
		circles[i].draw();
		circles[i].addAttractionPoint(ofVec2f(200,300),1);
	}
	j.draw();
	
	gui->setColorBack(0x7777cc);
	float angle=int(gui->box.getRotation()*10)/10.0f;
	ofVec2f pos=gui->box.getPosition()-ofVec2f(gui->box.getWidth(),gui->box.getHeight()).getRotated(angle);	
	ofTranslate(pos);
	ofRotate(angle);
	gui->draw();
	ofRotate(-angle);
	ofTranslate(-pos);

	// some debug information
	ofDrawBitmapString("Plug", 10, 15);
}


//--------------------------------------------------------------
void realApp::mouseMoved(int x, int y ) {
}

//--------------------------------------------------------------
void realApp::mouseDragged(int x, int y, int button) {
}

//--------------------------------------------------------------
void realApp::mousePressed(int x, int y, int button) {
}

//--------------------------------------------------------------
void realApp::mouseReleased(int x, int y, int button) {	
}

////--------------------------------------------------------------
void realApp::guiEvent(ofxUIEventArgs &e)
{
	string name = e.widget->getName(); 
	int kind = e.widget->getKind(); 
	cout << "got event from: " << name << endl; 	
	
	if(name == "TEXT INPUT")
    {
        ofxUITextInput *textinput = (ofxUITextInput *) e.widget; 
        if(textinput->getTriggerType() == OFX_UI_TEXTINPUT_ON_ENTER)
        {
            cout << "ON ENTER: "; 
//            ofUnregisterKeyEvents((realApp*)this); 
        }
        else if(textinput->getTriggerType() == OFX_UI_TEXTINPUT_ON_FOCUS)
        {
            cout << "ON FOCUS: "; 
        }
        else if(textinput->getTriggerType() == OFX_UI_TEXTINPUT_ON_UNFOCUS)
        {
            cout << "ON BLUR: "; 
//            ofRegisterKeyEvents(this);             
        }        
        string output = textinput->getTextString(); 
        cout << output << endl; 
    }
}
////--------------------------------------------------------------
void realApp::exit()
{
	delete gui; 
}
////--------------------------------------------------------------
void realApp::keyPressed(int key)
{
    if(gui->hasKeyboardFocus())
    {
        return;  
    }
	ofxBox2dCircle c;
	c.setPhysics(100, 0.3, 0.1);
	c.setup(box2d.getWorld(), (ofGetWidth()/2)+ofRandom(-50, 50), ofRandom(0,50), ofRandom(10, 20));
	c.setVelocity(ofRandom(-20,20),ofRandom(-20,20));
	circles.push_back(c);
	switch (key) 
	{			
		case 'f':
			ofToggleFullscreen(); 
			break;

		default:
			break;
	}
}

//--------------------------------------------------------------
void realApp::keyReleased(int key){

}

//--------------------------------------------------------------
void realApp::windowResized(int w, int h)
{
 

}

////--------------------------------------------------------------
void realApp::gotMessage(ofMessage msg){

}

////--------------------------------------------------------------
void realApp::dragEvent(ofDragInfo dragInfo){ 
	
}
