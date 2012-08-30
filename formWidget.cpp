#include "ofMain.h"
#include "ofxUI.h"
#include "ofxBox2d.h"


class formWidget : public ofxUIScrollableCanvas { //ofxUICanvas
public:
	formWidget()
	{
		float dim = 16; 
		float xInit = OFX_UI_GLOBAL_WIDGET_SPACING; 
		float length = 255-xInit; 
		//gui4 = new ofxUIScrollableCanvas(length*3+xInit*3+6, 0, length+xInit, ofGetHeight());     

		addWidgetDown(new ofxUILabel("Survey Form", OFX_UI_FONT_LARGE)); 	

		addSpacer(length-xInit, 2);
		setWidgetFontSize(OFX_UI_FONT_LARGE);
		addTextInput("Your name?", "is ..", length-xInit);
		setWidgetFontSize(OFX_UI_FONT_MEDIUM);

		vector<string> names; 
		names.push_back("MALE");
		names.push_back("FEMALE");
		names.push_back("ETC");
		addSpacer(length-xInit, 2);
		addRadio("GENDER", names, OFX_UI_ORIENTATION_HORIZONTAL, dim, dim); 

		addWidgetDown(new ofxUILabel("H SLIDERS", OFX_UI_FONT_MEDIUM)); 
		addSlider("HEIGHT (cm)", 10.0, 250.0, 170, length-xInit, dim);
		addSlider("WEIGHT (kg)", 2.0, 250.0, 60, length-xInit,dim);

		addSpacer(length-xInit, 2);
		addWidgetDown(new ofxUILabel("What do you feel?", OFX_UI_FONT_MEDIUM)); 				
	    addWidgetDown(new ofxUIBiLabelSlider(length-xInit, 0, 100, 50, "BILABEL", "HOT", "COLD", OFX_UI_FONT_MEDIUM));

		addSpacer(length-xInit, 2);
		addWidgetDown(new ofxUILabel("Your ring size?", OFX_UI_FONT_MEDIUM));
		addWidgetDown(new ofxUICircleSlider((length-xInit)*.5, 0, 100, 50.0, "Drag!", OFX_UI_FONT_MEDIUM));    

		addSpacer(length-xInit, 2);
		addWidgetDown(new ofxUILabel("Your Favorite Fruit?", OFX_UI_FONT_MEDIUM));
		addWidgetDown(new ofxUIMultiImageButton(dim*2, dim*2, false, "GUI/toggle.png", "Click to clear"));


		//ofAddListener(newGUIEvent,this,&realApp::guiEvent);
		setDrawBack(false);
	}
};
