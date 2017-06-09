#include "efectoBase.h"

//--------------------------------------------------------------
efectoBase::efectoBase(ofVideoGrabber *aCamPtr, ofxFaceTracker *aTrackerPtr, recortadorImagenCara *aRecortador)
{
    camPtr = aCamPtr;
    trackerPtr = aTrackerPtr;
    recortadorPtr = aRecortador;
}
//--------------------------------------------------------------
void efectoBase::setup()
{
    ofBackground(0,0,0);
	ofSetVerticalSync(true);
	ofSetDrawBitmapMode(OF_BITMAPMODE_MODEL_BILLBOARD);
	ofSetOrientation(OF_ORIENTATION_DEFAULT);
}
//--------------------------------------------------------------
void efectoBase::update()
{

}
//--------------------------------------------------------------
void efectoBase::draw(int indiceRotacion)
{

}
//--------------------------------------------------------------
void efectoBase::debug()
{

}
//--------------------------------------------------------------
void efectoBase::keyPressed(int key)
{

}
//--------------------------------------------------------------
string efectoBase::nombreEfecto()
{

}
//--------------------------------------------------------------
