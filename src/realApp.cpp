#pragma warning(disable:4819)
#include "realApp.h"


//--------------------------------------------------------------


void realApp::setup()
{ 
	ofEnableSmoothing(); 
	ofBackgroundHex(0xfdefc2);
	ofSetLogLevel(OF_LOG_NOTICE);
	ofSetVerticalSync(true);

	setGUI2(); 
    gui2->setDrawBack(true);
    gui2->setDrawPadding(true);

	
	// Box2d
	box2d.init();
	box2d.setGravity(0, 10);
	//box2d.createGround();
	box2d.createBounds();
	box2d.setFPS(24.0);

	// load the shape we saved...
	vector <ofVec2f> pts = loadPoints("shape.dat");
	ofxBox2dPolygon poly;
	
	// loop and add vertex points
	for (int i=0; i<pts.size(); i++) {
		poly.addVertex(pts[i]);
	}
	poly.setAsEdge(false);
	poly.triangulate(15);
	poly.setPhysics(1.0, 0.3, 0.3);
	poly.create(box2d.getWorld());
	triangles.push_back(poly);
}

vector <ofVec2f> realApp::loadPoints(string file) {
	ifstream f;
	vector <ofVec2f> pts;
	f.exceptions ( ifstream::failbit | ifstream::badbit );
	try {
		f.open(ofToDataPath(file).c_str());
		string strLines;
		while (!f.eof()) {
			string ptStr;
			getline(f, ptStr);
			strLines+=ptStr;
		}
		f.close();
		
		vector <string>  ptsStr = ofSplitString(strLines, ",");
		for (int i=0; i<ptsStr.size(); i+=2) {
			float x = ofToFloat(ptsStr[i]);
			float y = ofToFloat(ptsStr[i+1]);
			pts.push_back(ofVec2f(x, y));
		}
	}
	catch (ifstream::failure e) {
		printf("no file to load...\n");
	}
	return pts;
}


//--------------------------------------------------------------
void realApp::update() {
	
	// add some circles every so often
	if((int)ofRandom(0, 10) == 0) {
		ofxBox2dCircle c;
		c.setPhysics(0.3, 0.5, 0.1);
		c.setup(box2d.getWorld(), (ofGetWidth()/2)+ofRandom(-20, 20), 30, ofRandom(10, 20));
		circles.push_back(c);		
	}
	
	box2d.update();	
}

//--------------------------------------------------------------
void realApp::draw() {
	// some circles :)
	for (int i=0; i<circles.size(); i++) {
		ofFill();
		ofSetHexColor(0xc0dd3b);
		circles[i].draw();
	}

	ofSetHexColor(0x444342);
	ofFill();
	shape.draw();

	ofSetHexColor(0x444342);
	ofFill(); // <- OF not working here... 
	for (int i=0; i<triangles.size(); i++) {
		triangles[i].draw();
	}	
	// some debug information
	string info = "Draw a shape with the mouse\n";
	ofSetHexColor(0x444342);
	ofDrawBitmapString(info, 10, 15);

	////////////
}


//--------------------------------------------------------------
void realApp::mouseMoved(int x, int y ) {
}

//--------------------------------------------------------------
void realApp::mouseDragged(int x, int y, int button) {
	shape.addVertex(x, y);
}

//--------------------------------------------------------------
void realApp::mousePressed(int x, int y, int button) {
	shape.clear();
	shape.addVertex(x, y);
}

//--------------------------------------------------------------
void realApp::mouseReleased(int x, int y, int button) {
	
	// This is the manuel way to triangulate the shape
	// you can then add many little triangles
	
	// first simplify the shape
	shape.simplify();
	
	// save the outline of the shape
	ofPolyline outline = shape;
	
	// resample shape
	ofPolyline resampled = shape.getResampledBySpacing(25);
	
	// trangleate the shape, return am array of traingles
	vector <TriangleShape> tris = triangulatePolygonWithOutline(resampled, outline);
	
	// add some random points inside
	addRandomPointsInside(shape, 255);
	
	// now loop through all the trainles and make a box2d triangle
	for (int i=0; i<tris.size(); i++) {
		ofxBox2dPolygon p;
		p.addTriangle(tris[i].a, tris[i].b, tris[i].c);
		p.setPhysics(1.0, 0.3, 0.3);
		p.setAsEdge(false);
		if(p.isGoodShape()) {
			p.create(box2d.getWorld());
			triangles.push_back(p);
		}
	}
	
	// done with shape clear it now
	shape.clear();
}

////--------------------------------------------------------------
void realApp::guiEvent(ofxUIEventArgs &e)
{
	string name = e.widget->getName(); 
	int kind = e.widget->getKind(); 
	cout << "got event from: " << name << endl; 	
	
	if(name == "RED")
	{
		ofxUISlider *slider = (ofxUISlider *) e.widget; 
		cout << "RED " << slider->getScaledValue() << endl; 
		red = slider->getScaledValue(); 
	}
    else if(name == "TEXT INPUT")
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
	delete gui2; 
	delete[] buffer; 
    delete img; 
}
////--------------------------------------------------------------
void realApp::keyPressed(int key)
{
    if(gui2->hasKeyboardFocus())
    {
        return;  
    }
	switch (key) 
	{			
		case 'f':
			ofToggleFullscreen(); 
			break;

		case 'h':
            gui2->toggleVisible();      
			break;
        case '2':
            gui2->toggleVisible(); 
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

void realApp::setGUI2()
{	
	float dim = 16; 
	float xInit = OFX_UI_GLOBAL_WIDGET_SPACING; 
    float length = 255-xInit; 
    
    img = new ofImage(); 
    img->loadImage("nerd_me.png"); 
    buffer = new float[256];     
    for(int i = 0; i < 256; i++) { buffer[i] = ofNoise(i/100.0); }

    gui2 = new ofxUICanvas(length+xInit+2, 100, length+xInit, ofGetHeight()/2.);
	gui2->addWidgetDown(new ofxUILabel("PANEL 2: ADVANCED", OFX_UI_FONT_LARGE)); 

	gui2->addSpacer(length-xInit, 2);
    gui2->addWidgetDown(new ofxUILabel("DROP DOWN", OFX_UI_FONT_MEDIUM)); 				
        
    vector<string> items; 
    items.push_back("FIRST ITEM"); 
    items.push_back("SECOND ITEM"); 
    items.push_back("THIRD ITEM WHATS UP DOG"); 
    items.push_back("FOURTH ITEM"); 
    items.push_back("FIFTH ITEM"); 
    items.push_back("SIXTH ITEM");     
    
    gui2->addWidgetDown(new ofxUIDropDownList(200, "DROP DOWN LIST", items, OFX_UI_FONT_MEDIUM)); 

 	ofAddListener(gui2->newGUIEvent,this,&realApp::guiEvent);
}
