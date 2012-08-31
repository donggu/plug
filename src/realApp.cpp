#pragma warning(disable:4819)
#include "realApp.h"
#include "formWidget.cpp"


//--------------------------------------------------------------
ofxUICanvas *gui;

ofxBox2dJoint j;
void realApp::setup()
{ 
	ofEnableSmoothing(); 
	ofBackgroundHex(0xfdefc2);
	ofSetLogLevel(OF_LOG_NOTICE);
	ofSetVerticalSync(true);

	//gui
	float xInit = OFX_UI_GLOBAL_WIDGET_SPACING; 
    float length = 255-xInit; 

    gui = new formWidget(length+xInit+2, 100, length+xInit, ofGetHeight()-200);
 	ofAddListener(gui->newGUIEvent,this,&realApp::guiEvent);

    gui->setDrawBack(true);
    //gui->setDrawPadding(true);

	
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
//	widgets.push_back(c);

	ofxBox2dCircle c2;
	c2.setPhysics(0.3, 0.3, 0.1);
	c2.setup(box2d.getWorld(), (ofGetWidth()/2)+ofRandom(-10, 10), 50, ofRandom(10, 20));
	circles.push_back(c2);
//	widgets.push_back(c2);

	ofxBox2dRect b;
	b.setPhysics(0.3,0.6,0.1);
	b.setup(box2d.getWorld(), length+xInit+2,100,length+xInit,ofGetHeight()-200);
	rects.push_back(b);

	j.setup(box2d.getWorld(), c.body, c2.body);
	j.setLength(100);
	j.setDamping(0.2);
}


//--------------------------------------------------------------
void realApp::update() {
	
	box2d.update();	
}

//--------------------------------------------------------------
void realApp::draw() {

	circles[0].draw();
	circles[1].draw();

	// some circles :)
	for (int i=0; i<rects.size(); i++) {
		ofFill();
		ofSetHexColor(0xc0dd3b);
		rects[i].draw();

		ofVec2f pos=rects[i].getPosition();
		float angle=rects[i].getRotation();
		ofTranslate(pos);
		ofRotate(angle);
		gui->drawAtOrigin();
		ofRotate(-angle);
		ofTranslate(-pos);
	}

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
