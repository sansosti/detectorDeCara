#include "efectoCabezaRebotante.h"

#define SPEED_X 10
#define SPEED_Y 10
//--------------------------------------------------------------
efectoCabezaRebotante::efectoCabezaRebotante(ofVideoGrabber *aCamPtr, ofxFaceTracker *aTrackerPtr, recortadorImagenCara *aRecortador):efectoBase(aCamPtr,aTrackerPtr,aRecortador)
{
    //ctor
}
//--------------------------------------------------------------
efectoCabezaRebotante::~efectoCabezaRebotante()
{
    //dtor
}
//--------------------------------------------------------------
void efectoCabezaRebotante::setup()
{
    efectoBase::setup();

    posX = ((float)rand()/RAND_MAX)*(ofGetWindowWidth());
    posY = ((float)rand()/RAND_MAX)*(ofGetWindowHeight());

    speedX = SPEED_X;
    speedY = SPEED_Y;
    incX = speedX;
    incY = speedY;
}
//--------------------------------------------------------------
void efectoCabezaRebotante::update()
{
//    if(trackerPtr->getFound()) {
        recortadorPtr->update();
        /*
        ofPolyline facePoints = trackerPtr->getImageFeature(ofxFaceTracker::FACE_OUTLINE); //getting the Polyline for the whole face

        ofRectangle faceBox = facePoints.getBoundingBox(); //Bounding box of the face
        ofPoint faceCenter = faceBox.getCenter();

        ofPixels pixels;
        camPtr->getTextureReference().readToPixels(pixels); //copy cam image to ofPixels
        pixels.crop(faceBox.x,faceBox.y,faceBox.width,faceBox.height);//Crop them to the bounding box of the recognized face

        int totalPixels = pixels.getWidth()*pixels.getHeight();
        for (int x = 0; x < pixels.getWidth(); x++){
            for (int y = 0; y < pixels.getHeight(); y++){
                //make a point to check whether it is inside the face, but include the x and y of the bounding box
                ofPoint checkpoint = ofPoint(x+faceBox.x,y+faceBox.y);

                if(facePoints.inside(checkpoint)){ //if inside, do nothing
                } else {
                    ofColor b = ofColor(0);//black
                    pixels.setColor(x,y,b); //Make current pixel black
                }

            }
        }
        videoImage.setFromPixels(pixels); //Finally, create an ofImage out of the set of pixels
        */
//    }
}
//--------------------------------------------------------------
void efectoCabezaRebotante::draw(int indiceRotacion)
{
//    speedX = sin(ofGetElapsedTimef()*5)*10;

	//if(trackerPtr->getFound()) {
    if (recortadorPtr->getIsUpToDate()) {
        //ofImage currentCara = recortadorPtr->getImage();
        ofImage* imagesList = recortadorPtr->getFeatureImages();
        ofImage currentCara = imagesList[0];
        if (posX < 0) {
            incX = speedX;
        };
        if (posX > (ofGetWindowWidth()-currentCara.width)) {
            incX = -speedX;
        };
        if (posY < 0) {
            incY = speedY;
        };
        if (posY > (ofGetWindowHeight()-currentCara.height)) {
            incY = -speedY;
        };
        posX += incX;
        posY += incY;

        currentCara.draw(posX,posY);
	}
}
//--------------------------------------------------------------
string efectoCabezaRebotante::nombreEfecto()
{
    return "efectoCabezaRebotante";
}
//--------------------------------------------------------------
