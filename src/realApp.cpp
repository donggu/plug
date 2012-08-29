#pragma warning(disable:4819)
#include "realApp.h"


//--------------------------------------------------------------


void realApp::setup()
{ 
	ofEnableSmoothing(); 
	ofBackgroundHex(0xfdefc2);
	ofSetLogLevel(OF_LOG_NOTICE);
	ofSetVerticalSync(true);

//	setGUI1(); 
//	setGUI2(); 
//    setGUI3(); 
    setGUI4();     
//
//    gui1->setDrawBack(false);
//    gui2->setDrawBack(false);
//    gui3->setDrawBack(false);
    gui4->setDrawBack(false);    
//
////    gui1->setDrawBack(true);
////    gui2->setDrawBack(true);
////    gui3->setDrawBack(true);
    gui4->setDrawBack(true);    
//    
////    gui1->setDrawPadding(false);
////    gui2->setDrawPadding(false);
////    gui3->setDrawPadding(false);
////    
//    ofBackground(red, green, blue); 

	
	// Box2d
	box2d.init();
	box2d.setGravity(0, 20);
	box2d.createGround();
	box2d.setFPS(30.0);
	
	
	
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
		c.setup(box2d.getWorld(), (ofGetWidth()/2)+ofRandom(-20, 20), -20, ofRandom(10, 20));
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
	info += "Press 1 to add some circles\n";
	info += "Press c to clear everything\n";
	
	ofSetHexColor(0x444342);
	ofDrawBitmapString(info, 10, 15);
}


//--------------------------------------------------------------
void realApp::keyPressed(int key) {
	
	if(key == '1') {
		ofxBox2dCircle c;
		c.setPhysics(1, 0.5, 0.5);
		c.setup(box2d.getWorld(), mouseX, mouseY, 10);
		circles.push_back(c);
	}
	
	if(key == 'c') {
		shape.clear();
		for (int i=0; i<triangles.size(); i++) {
			triangles[i].destroy();
		}
	}
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

//
//
////--------------------------------------------------------------
//void realApp::update()
//{
//    //mg->addPoint(ofGetFrameRate());
//		if(toggle.hasChanged()){
//		handle.setGrabbing(!toggle.isActive());
//		handle.fillMe = toggle.isActive();
//		toggle.resetChanged();
//	}
//}
//
//
////--------------------------------------------------------------
//void realApp::draw()
//{    
//	ofSetColor(255,255,255);
//	bezier.draw();
//	//ofBackground(red, green, blue, 255); 
//	//
//	//ofPushStyle(); 
//	//ofEnableBlendMode(OF_BLENDMODE_ALPHA); 
//
//
//	//if(bdrawGrid)
//	//{
//	//	ofSetColor(255, 255, 255, 25);	
//	//	drawGrid(8,8); 
//	//}
// //   
//	//ofPopStyle(); 
// //   
// //   ofSetRectMode(OF_RECTMODE_CENTER); 
//
//}
////--------------------------------------------------------------
//void realApp::guiEvent(ofxUIEventArgs &e)
//{
//	string name = e.widget->getName(); 
//	int kind = e.widget->getKind(); 
//	cout << "got event from: " << name << endl; 	
//	
//	if(name == "RED")
//	{
//		ofxUISlider *slider = (ofxUISlider *) e.widget; 
//		cout << "RED " << slider->getScaledValue() << endl; 
//		red = slider->getScaledValue(); 
//	}
//	else if(name == "GREEN")
//	{
//		ofxUISlider *slider = (ofxUISlider *) e.widget; 
//		cout << "GREEN " << slider->getScaledValue() << endl; 
//		green = slider->getScaledValue(); 
//	}
//	
//	else if(name == "BLUE")
//	{
//		ofxUISlider *slider = (ofxUISlider *) e.widget; 
//		cout << "BLUE " << slider->getScaledValue() << endl; 
//		blue = slider->getScaledValue(); 		
//	}
//	else if(name == "DRAW GRID")
//	{
//		ofxUIButton *button = (ofxUIButton *) e.widget; 
//		bdrawGrid = button->getValue(); 
//	}
//	else if(name == "D_GRID")
//	{
//		ofxUIToggle *toggle = (ofxUIToggle *) e.widget; 
//		bdrawGrid = toggle->getValue(); 
//	}
//    else if(name == "TEXT INPUT")
//    {
//        ofxUITextInput *textinput = (ofxUITextInput *) e.widget; 
//        if(textinput->getTriggerType() == OFX_UI_TEXTINPUT_ON_ENTER)
//        {
//            cout << "ON ENTER: "; 
////            ofUnregisterKeyEvents((realApp*)this); 
//        }
//        else if(textinput->getTriggerType() == OFX_UI_TEXTINPUT_ON_FOCUS)
//        {
//            cout << "ON FOCUS: "; 
//        }
//        else if(textinput->getTriggerType() == OFX_UI_TEXTINPUT_ON_UNFOCUS)
//        {
//            cout << "ON BLUR: "; 
////            ofRegisterKeyEvents(this);             
//        }        
//        string output = textinput->getTextString(); 
//        cout << output << endl; 
//    }
//	
//	
//	
//}
////--------------------------------------------------------------
void realApp::exit()
{
	delete gui1; 
	delete gui2; 
    delete gui3; 
    delete gui4; 
	delete[] buffer; 
    delete img; 
}
////--------------------------------------------------------------
//void realApp::keyPressed(int key)
//{
//	if(key == 'd'){
//		bezier.toggleDrawing();
//	}
//	if(key == 'c'){
//		bezier.clear();
//	}
//	return;
//
//    if(gui2->hasKeyboardFocus())
//    {
//        return;  
//    }
//	switch (key) 
//	{			
//		case '`':
//		{
//			string hr;         
//			ofImage img; 
//			img.grabScreen(0,0,ofGetWidth(), ofGetHeight()); 
//			if(ofGetHours() < 12)
//			{
//				hr = " AM"; 
//			}
//			else
//			{
//				hr = " PM"; 
//			}
//			img.saveImage("snapshots/OFXUI "+ofToString(ofGetYear())+"-"+ofToString(ofGetMonth())+"-"+ofToString(ofGetDay())+" at "+ofToString(ofGetHours(),2)+"."+ofToString(ofGetMinutes(),2)+"."+ofToString(ofGetSeconds(),2) +hr +".png");
//		}						
//			break; 
//			
//		case 'f':
//			ofToggleFullscreen(); 
//			break;
//
//		case 'h':
//            gui1->toggleVisible(); 
//            gui2->toggleVisible(); 
//            gui3->toggleVisible(); 
//            gui4->toggleVisible();             
//			break;
//
//		case 'p':
//			bdrawPadding = !bdrawPadding; 
//			gui1->setDrawWidgetPaddingOutline(bdrawPadding); 			
//			gui2->setDrawWidgetPaddingOutline(bdrawPadding); 			
//			gui3->setDrawWidgetPaddingOutline(bdrawPadding); 			            
//			gui4->setDrawWidgetPaddingOutline(bdrawPadding); 			                        
//			break;			
//
//		case '[':
//			gui1->setDrawWidgetPadding(false); 			
//			gui2->setDrawWidgetPadding(false);
//			gui3->setDrawWidgetPadding(false);
//			gui4->setDrawWidgetPadding(false);            
//			break;			
//
//		case ']':
//			gui1->setDrawWidgetPadding(true); 			
//			gui2->setDrawWidgetPadding(true);
//			gui3->setDrawWidgetPadding(true);
//			gui4->setDrawWidgetPadding(true);            
//			break;			
//			
//        case '1':
//            gui1->toggleVisible(); 
//            break;
//
//        case '2':
//            gui2->toggleVisible(); 
//            break;
//
//        case '3':
//            gui3->toggleVisible(); 
//            break;
//
//        case '4':
//            gui4->toggleVisible(); 
//            break;
//            
//		default:
//			break;
//	}
//}
//
//--------------------------------------------------------------
void realApp::keyReleased(int key){

}

//--------------------------------------------------------------
//void realApp::mouseMoved(int x, int y )
//{ 
//	
//}
//
////--------------------------------------------------------------
//void realApp::mouseDragged(int x, int y, int button)
//{
// 
//}

////--------------------------------------------------------------
//void realApp::mousePressed(int x, int y, int button)
//{
//}
//
////--------------------------------------------------------------
//void realApp::mouseReleased(int x, int y, int button)
//{
//
//}

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
//
//void realApp::drawGrid(float x, float y)
//{
//	return;
//
//    float w = ofGetWidth(); 
//    float h = ofGetHeight(); 
//    
//    for(int i = 0; i < h; i+=y)
//    {
//        ofLine(0,i,w,i); 
//    }
//    
//    for(int j = 0; j < w; j+=x)
//    {
//        ofLine(j,0,j,h); 
//    }    
//}
//
//void realApp::setGUI1()
//{
//	red = 233; blue = 52; green = 27; 
//	
//	float dim = 16; 
//	float xInit = OFX_UI_GLOBAL_WIDGET_SPACING; 
//    float length = 255-xInit; 
//	hideGUI = false; 
//	
//    vector<string> names; 
//	names.push_back("RAD1");
//	names.push_back("RAD2");
//	names.push_back("RAD3");	
//	
//	gui1 = new ofxUICanvas(0, 0, length+xInit, ofGetHeight()); 
//	gui1->addWidgetDown(new ofxUILabel("PANEL 1: BASICS", OFX_UI_FONT_LARGE)); 
//    gui1->addWidgetDown(new ofxUILabel("Press 'h' to Hide GUIs", OFX_UI_FONT_LARGE)); 
//
//    gui1->addSpacer(length-xInit, 2);
//	gui1->addWidgetDown(new ofxUILabel("H SLIDERS", OFX_UI_FONT_MEDIUM)); 
//	gui1->addSlider("RED", 0.0, 255.0, red, length-xInit, dim);
//	gui1->addSlider("GREEN", 0.0, 255.0, green, length-xInit,dim);
//	gui1->addSlider("BLUE", 0.0, 255.0, blue, length-xInit,dim);
//
//    gui1->addSpacer(length-xInit, 2); 
//    gui1->addWidgetDown(new ofxUILabel("V SLIDERS", OFX_UI_FONT_MEDIUM)); 
//	gui1->addSlider("0", 0.0, 255.0, 150, dim, 160);
//	gui1->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
//	gui1->addSlider("1", 0.0, 255.0, 150, dim, 160);
//	gui1->addSlider("2", 0.0, 255.0, 150, dim, 160);
//	gui1->addSlider("3", 0.0, 255.0, 150, dim, 160);
//	gui1->addSlider("4", 0.0, 255.0, 150, dim, 160);
//	gui1->addSlider("5", 0.0, 255.0, 150, dim, 160);
//	gui1->addSlider("6", 0.0, 255.0, 150, dim, 160);
//	gui1->addSlider("7", 0.0, 255.0, 150, dim, 160);
//	gui1->addSlider("8", 0.0, 255.0, 150, dim, 160);
//	gui1->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
//    
//    gui1->addSpacer(length-xInit, 2);
//	gui1->addRadio("RADIO HORIZONTAL", names, OFX_UI_ORIENTATION_HORIZONTAL, dim, dim); 
//	gui1->addRadio("RADIO VERTICAL", names, OFX_UI_ORIENTATION_VERTICAL, dim, dim); 
//
//    gui1->addSpacer(length-xInit, 2);
//	gui1->addWidgetDown(new ofxUILabel("BUTTONS", OFX_UI_FONT_MEDIUM)); 
//	gui1->addButton("DRAW GRID", false, dim, dim);
//	gui1->addWidgetDown(new ofxUILabel("TOGGLES", OFX_UI_FONT_MEDIUM)); 
//	gui1->addToggle( "D_GRID", false, dim, dim);
//    
//    gui1->addSpacer(length-xInit, 2);
//    gui1->addWidgetDown(new ofxUILabel("RANGE SLIDER", OFX_UI_FONT_MEDIUM)); 
//	gui1->addRangeSlider("RSLIDER", 0.0, 255.0, 50.0, 100.0, length-xInit,dim);
//
//    gui1->addSpacer(length-xInit, 2);
//	gui1->addWidgetDown(new ofxUILabel("2D PAD", OFX_UI_FONT_MEDIUM)); 
//	gui1->add2DPad("PAD", ofPoint(0,length-xInit), ofPoint(0,120), ofPoint((length-xInit)*.5,120*.5), length-xInit,120);
//
//
//    
//    bdrawGrid = false; 
//	bdrawPadding = false; 	
//	ofAddListener(gui1->newGUIEvent,this,&realApp::guiEvent);
//}
//
//void realApp::setGUI2()
//{	
//	float dim = 16; 
//	float xInit = OFX_UI_GLOBAL_WIDGET_SPACING; 
//    float length = 255-xInit; 
//    
//    img = new ofImage(); 
//    img->loadImage("nerd_me.png"); 
//    buffer = new float[256];     
//    for(int i = 0; i < 256; i++) { buffer[i] = ofNoise(i/100.0); }
//
//    gui2 = new ofxUICanvas(length+xInit+2, 0, length+xInit, ofGetHeight());     
//	gui2->addWidgetDown(new ofxUILabel("PANEL 2: ADVANCED", OFX_UI_FONT_LARGE)); 
//
//    gui2->addSpacer(length-xInit, 2);
//	gui2->addWidgetDown(new ofxUILabel("TEXT INPUT", OFX_UI_FONT_MEDIUM));
//	gui2->setWidgetFontSize(OFX_UI_FONT_LARGE);
//	gui2->addTextInput("TEXT INPUT", "Input Text", length-xInit);
//    gui2->addWidgetDown(new ofxUILabel("AUTO CLEAR DISABLED", OFX_UI_FONT_MEDIUM));
//    gui2->addTextInput("TEXT INPUT2", "Input Text", length-xInit)->setAutoClear(false);
//	gui2->setWidgetFontSize(OFX_UI_FONT_MEDIUM);
//
//    gui2->addSpacer(length-xInit, 2);
//    gui2->addWidgetDown(new ofxUILabel("WAVEFORM DISPLAY", OFX_UI_FONT_MEDIUM));     
//	gui2->addWidgetDown(new ofxUIWaveform(length-xInit, 64, buffer, 256, 0.0, 1.0, "WAVEFORM")); 
//    gui2->addWidgetDown(new ofxUILabel("SPECTRUM DISPLAY", OFX_UI_FONT_MEDIUM));    
//    gui2->addWidgetDown(new ofxUISpectrum(length-xInit, 64, buffer, 256, 0.0, 1.0, "SPECTRUM")); 
//
//    gui2->addSpacer(length-xInit, 2);
//    gui2->addWidgetDown(new ofxUILabel("IMAGE DISPLAY", OFX_UI_FONT_MEDIUM));
//	gui2->addWidgetDown(new ofxUIImage(img->width*.5, img->height*.5, img, "IMAGE CAPTION")); 
//
//    gui2->addSpacer(length-xInit, 2);
//    gui2->addWidgetDown(new ofxUILabel("FPS LABEL", OFX_UI_FONT_MEDIUM));
//    gui2->addWidgetDown(new ofxUIFPS(OFX_UI_FONT_MEDIUM)); 
//
//    gui2->addSpacer(length-xInit, 2);
//    gui2->addWidgetDown(new ofxUILabel("NUMBER DIALER", OFX_UI_FONT_MEDIUM));
//    gui2->addWidgetDown(new ofxUINumberDialer(-10000, 10000, 5000, 1, "NUMBER DIALER", OFX_UI_FONT_MEDIUM)); 
//
//    gui2->addSpacer(length-xInit, 2);
//    gui2->addWidgetDown(new ofxUILabel("LABEL BUTTON", OFX_UI_FONT_MEDIUM));
//    gui2->addWidgetDown(new ofxUILabelButton(length-xInit, false, "LABEL BUTTON", OFX_UI_FONT_MEDIUM));     
//    gui2->addWidgetDown(new ofxUILabelButton(false, "AUTOSIZE BUTTON", OFX_UI_FONT_MEDIUM));     
//    gui2->addWidgetDown(new ofxUILabelButton(false, "MEDIUM BUTTON", OFX_UI_FONT_MEDIUM));     
//    
//    gui2->addSpacer(length-xInit, 2);
//    gui2->addWidgetDown(new ofxUILabel("LABEL TOGGLES", OFX_UI_FONT_MEDIUM));	
//    gui2->addWidgetDown(new ofxUILabelToggle(length-xInit, false, "LABEL TOGGLE", OFX_UI_FONT_MEDIUM));     
//    gui2->addWidgetDown(new ofxUILabelToggle(false, "AUTOSIZE TOGGLE", OFX_UI_FONT_MEDIUM));     
//    gui2->addWidgetDown(new ofxUILabelToggle(false, "MEDIUM TOGGLE", OFX_UI_FONT_MEDIUM));     
//    
//	ofAddListener(gui2->newGUIEvent,this,&realApp::guiEvent);
//}
//
//void realApp::setGUI3()
//{	
//	float dim = 16; 
//	float xInit = OFX_UI_GLOBAL_WIDGET_SPACING; 
//    float length = 255-xInit; 
//	gui3 = new ofxUICanvas(length*2+xInit*2+4, 0, length+xInit, ofGetHeight());     
//    gui3->addWidgetDown(new ofxUILabel("PANEL 3: ADVANCED", OFX_UI_FONT_LARGE)); 	
//
//    gui3->addSpacer(length-xInit, 2);
//    gui3->addWidgetDown(new ofxUILabel("MATRIX", OFX_UI_FONT_MEDIUM)); 				
//    gui3->addWidgetDown(new ofxUIToggleMatrix(dim, dim, 4, 4, "MATRIX1")); 
//    gui3->addWidgetDown(new ofxUIToggleMatrix(dim*2.0, dim*2.0, 3, 6, "MATRIX2")); 
//    gui3->addWidgetDown(new ofxUIToggleMatrix(dim*3.0, dim*3.0, 1, 4, "MATRIX3")); 
//
//    gui3->addSpacer(length-xInit, 2);
//    gui3->addWidgetDown(new ofxUILabel("IMAGE BUTTON", OFX_UI_FONT_MEDIUM)); 				    
//    gui3->addWidgetDown(new ofxUIImageButton(dim*4.0, dim*4.0, false, "GUI/images/App.png","IMAGEBTN")); 
//    gui3->addWidgetDown(new ofxUILabel("IMAGE TOGGLE", OFX_UI_FONT_MEDIUM)); 				        
//    gui3->addWidgetDown(new ofxUIImageToggle(dim*4.0, dim*4.0, false, "GUI/images/Preview.png","IMAGETGL")); 
//
//    gui3->addSpacer(length-xInit, 2);
//    gui3->addWidgetDown(new ofxUILabel("ROTARY SLIDER", OFX_UI_FONT_MEDIUM)); 			
//    gui3->addWidgetDown(new ofxUIRotarySlider(dim*4, 0, 100, 50, "R2SLIDER")); 			
//    gui3->addSpacer(length-xInit, 2);
//    gui3->addWidgetDown(new ofxUILabel("DROP DOWN", OFX_UI_FONT_MEDIUM)); 				
//        
//    vector<string> items; 
//    items.push_back("FIRST ITEM"); 
//    items.push_back("SECOND ITEM"); 
//    items.push_back("THIRD ITEM WHATS UP DOG"); 
//    items.push_back("FOURTH ITEM"); 
//    items.push_back("FIFTH ITEM"); 
//    items.push_back("SIXTH ITEM");     
//    
//    gui3->addWidgetDown(new ofxUIDropDownList(200, "DROP DOWN LIST", items, OFX_UI_FONT_MEDIUM)); 
//
//	ofAddListener(gui3->newGUIEvent,this,&realApp::guiEvent);
//}
//
//void realApp::setGUI4()
//{	
//	float dim = 16; 
//	float xInit = OFX_UI_GLOBAL_WIDGET_SPACING; 
//    float length = 255-xInit; 
//	gui4 = new ofxUIScrollableCanvas(length*3+xInit*3+6, 0, length+xInit, ofGetHeight());     
//    gui4->addWidgetDown(new ofxUILabel("PANEL 4: SCROLLABLE", OFX_UI_FONT_LARGE)); 	
//    
//    gui4->addSpacer(length-xInit, 2);
//
//    gui4->addWidgetDown(new ofxUILabel("BILABEL SLIDER", OFX_UI_FONT_MEDIUM)); 				
//    gui4->addWidgetDown(new ofxUIBiLabelSlider(length-xInit, 0, 100, 50, "BILABEL", "HOT", "COLD", OFX_UI_FONT_MEDIUM));
//
//    gui4->addWidgetDown(new ofxUILabel("MINIMAL SLIDER", OFX_UI_FONT_MEDIUM)); 				
//    gui4->addWidgetDown(new ofxUIMinimalSlider(length-xInit, dim, 0, 100, 50.0, "MINIMAL",OFX_UI_FONT_MEDIUM));
//
//    gui4->addSpacer(length-xInit, 2);
//    
//    gui4->addWidgetDown(new ofxUILabel("CIRCLE SLIDER", OFX_UI_FONT_MEDIUM)); 				
//    gui4->addWidgetDown(new ofxUICircleSlider((length-xInit)*.5, 0, 100, 50.0, "NORTH SOUTH", OFX_UI_FONT_MEDIUM));    
//    
//    gui4->addSpacer(length-xInit, 2);
//    gui4->addWidgetDown(new ofxUILabel("FPS SLIDER", OFX_UI_FONT_MEDIUM)); 				
//    gui4->addFPSSlider("FPS SLIDER", length-xInit, dim);
//
//    vector<float> buffer; 
//    for(int i = 0; i < 256; i++)
//    {
//        buffer.push_back(0.0);
//    }
//
//    gui4->addWidgetDown(new ofxUILabel("MOVING GRAPH", OFX_UI_FONT_MEDIUM)); 				    
//    mg = (ofxUIMovingGraph *) gui4->addWidgetDown(new ofxUIMovingGraph(length-xInit, 120, buffer, 256, 0, 400, "MOVING GRAPH"));
//    
//    gui4->addSpacer(length-xInit, 2);
//    gui4->addWidgetDown(new ofxUILabel("IMAGE SAMPLER", OFX_UI_FONT_MEDIUM)); 				
//    gui4->addWidgetDown(new ofxUIImageSampler(img->getWidth(), img->getHeight(), img, "SAMPLER"));
//    gui4->addWidgetDown(new ofxUIMultiImageButton(dim*2, dim*2, false, "GUI/toggle.png", "IMAGE BUTTON"));
//    gui4->addWidgetDown(new ofxUIMultiImageToggle(dim*2, dim*2, false, "GUI/toggle.png", "IMAGE BUTTON"));
//    
//    
//	ofAddListener(gui4->newGUIEvent,this,&realApp::guiEvent);
//	}