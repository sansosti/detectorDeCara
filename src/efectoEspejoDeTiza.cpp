#include "efectoEspejoDeTiza.h"

#define ESPEJO_LINE_WIDTH 5
#define ESPEJO_SCALE_X 3
#define ESPEJO_SCALE_Y 3
//--------------------------------------------------------------
efectoEspejoDeTiza::efectoEspejoDeTiza(ofVideoGrabber *aCamPtr, ofxFaceTracker *aTrackerPtr, recortadorImagenCara *aRecortador):efectoBase(aCamPtr,aTrackerPtr,aRecortador)
{
    //ctor
}
//--------------------------------------------------------------
efectoEspejoDeTiza::~efectoEspejoDeTiza()
{
    //dtor
}
//--------------------------------------------------------------
string efectoEspejoDeTiza::nombreEfecto()
{
    return "efectoEspejoDeTiza";
}
//--------------------------------------------------------------
void efectoEspejoDeTiza::setup()
{
    efectoBase::setup();

    ofSetColor(255);
}
//--------------------------------------------------------------
void efectoEspejoDeTiza::draw(int indiceRotacion)
{
    if(trackerPtr->getFound()) {
        ofSetLineWidth(ESPEJO_LINE_WIDTH);
        //ofSetLineWidth(ESPEJO_LINE_WIDTH*abs(sin(ofGetElapsedTimef()*2)));
        int scaleX = ESPEJO_SCALE_X;
        int scaleY = ESPEJO_SCALE_Y;
        /*
        ofTranslate((ofGetWindowWidth()-(camPtr->getWidth()*scaleX)) / 2, (ofGetWindowHeight()-(camPtr->getHeight()*scaleY)) / 2);
        ofScale(scaleX,scaleY,0);
        ofSetOrientation(OF_ORIENTATION_180,false);
        */
        ofOrientation orientacion;
        int actualWidth = (ofGetWindowWidth()-(camPtr->getWidth()*scaleX)) / 2;
        int actualHeight = (ofGetWindowHeight()-(camPtr->getHeight()*scaleY)) / 2;
        int translationWidth,translationHeight;

        if (indiceRotacion == 0) {
            orientacion = OF_ORIENTATION_180;
            translationWidth = actualWidth;
            translationHeight = actualHeight;
        } else {
            translationWidth = actualHeight;
            translationHeight = actualWidth;
            if (indiceRotacion == 1) {
                orientacion = OF_ORIENTATION_90_RIGHT;
            } else {
                orientacion = OF_ORIENTATION_90_LEFT;
            }
        }

        ofTranslate(translationWidth,translationHeight);
        ofScale(scaleX,scaleY,0);
        ofSetOrientation(orientacion,false);

        trackerPtr->draw();
    }
}
//--------------------------------------------------------------
