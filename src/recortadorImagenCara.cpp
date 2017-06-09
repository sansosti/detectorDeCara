#include "recortadorImagenCara.h"

//--------------------------------------------------------------
recortadorImagenCara::recortadorImagenCara()
{
    isUpToDate = false;
    //totalCapturas = 0;
    featuresIndex[0] = ofxFaceTracker::FACE_OUTLINE;
//    featuresIndex[1] = ofxFaceTracker::LEFT_EYE;
//    featuresIndex[2] = ofxFaceTracker::RIGHT_EYE;
//    featuresIndex[3] = ofxFaceTracker::OUTER_MOUTH;


    featureDescription[ofxFaceTracker::LEFT_EYEBROW] = "ceja-izq";
    featureDescription[ofxFaceTracker::RIGHT_EYEBROW] = "ceja-der";
    featureDescription[ofxFaceTracker::LEFT_EYE] = "ojo-izq";
    featureDescription[ofxFaceTracker::RIGHT_EYE] = "ojo-der";
    featureDescription[ofxFaceTracker::LEFT_JAW] = "mandib-izq";
    featureDescription[ofxFaceTracker::RIGHT_JAW] = "mandib-der";
    featureDescription[ofxFaceTracker::JAW] = "mandib";
    featureDescription[ofxFaceTracker::OUTER_MOUTH] = "boca-ext";
    featureDescription[ofxFaceTracker::INNER_MOUTH] = "boca-int";
    featureDescription[ofxFaceTracker::NOSE_BRIDGE] = "nariz-puente";
    featureDescription[ofxFaceTracker::NOSE_BASE] = "nariz-base";
    featureDescription[ofxFaceTracker::FACE_OUTLINE] = "cara";
    featureDescription[ofxFaceTracker::ALL_FEATURES] = "todo";

    dLastFeatureBoxWidth = 0;
}
//--------------------------------------------------------------
recortadorImagenCara::~recortadorImagenCara()
{
    //dtor
}
//--------------------------------------------------------------
void recortadorImagenCara::setDebugMode(bool aDebugMode)
{
    debugMode = aDebugMode;
}
//--------------------------------------------------------------
void recortadorImagenCara::setup(ofVideoGrabber *aCamPtr, ofxFaceTracker *aTrackerPtr,int aMinFaceWidth,int aMinBigFaceWidth)
{
    camPtr = aCamPtr;
    trackerPtr = aTrackerPtr;
    minFaceWidth = aMinFaceWidth;
    minBigFaceWidth = aMinBigFaceWidth;
}
//--------------------------------------------------------------
void recortadorImagenCara::update()
{
    if(trackerPtr->getFound()) {
        if ((!isUpToDate) || (!bigEnoughFound)){
            for (int i=0;i<CURRENT_FEATURE_COUNT;i++) {
                if (!capturarFeature(i)) {
                    return;
                }
            };
            isUpToDate = true;
            //totalCapturas++;
        }
    } else {
        dLastFeatureBoxWidth = 0;
        isUpToDate = false;
        bigEnoughFound = false;
    }
}
//--------------------------------------------------------------
bool recortadorImagenCara::capturarFeature(int index)
{
//    ofLog() << ofGetTimestampString() + " capturarFeature: " + ofToString(index);
    ofPolyline featurePoints = trackerPtr->getImageFeature(featuresIndex[index]); //getting the Polyline for the selected feature

    ofRectangle featureBox = featurePoints.getBoundingBox(); //Bounding box of the face

    dLastFeatureBoxWidth = featureBox.width;

    if (featureBox.width < minFaceWidth) {
        return false;
    }

    if (featureBox.width >= minBigFaceWidth) {
        bigEnoughFound = true;
    }
    /*if (debugMode) {
        ofLogNotice() << "Feature box width: " + ofToString(featureBox.width);
    }*/

    ofPixels pixels;
    camPtr->getTextureReference().readToPixels(pixels); //copy cam image to ofPixels

    pixels.setImageType(OF_IMAGE_COLOR_ALPHA);

    pixels.crop(featureBox.x,featureBox.y,featureBox.width,featureBox.height);//Crop them to the bounding box of the recognized face

    int totalPixels = pixels.getWidth()*pixels.getHeight();
    for (int x = 0; x < pixels.getWidth(); x++){
        for (int y = 0; y < pixels.getHeight(); y++){
            //make a point to check whether it is inside the face, but include the x and y of the bounding box
            ofPoint checkpoint = ofPoint(x+featureBox.x,y+featureBox.y);

            if(featurePoints.inside(checkpoint)){ //if inside, do nothing
            } else {
                ofColor b = ofColor(0,255,0,0);// verde, transparente
                pixels.setColor(x,y,b); //Make current pixel black
            }

        }
    }
    currentFeatureImages[index].setFromPixels(pixels); //Finally, create an ofImage out of the set of pixels

    return true;
}
//--------------------------------------------------------------
ofImage* recortadorImagenCara::getFeatureImages()
{
    return currentFeatureImages;
}
//--------------------------------------------------------------
string recortadorImagenCara::getFeatureDescription(ofxFaceTracker::Feature feature)
{
        return featureDescription[feature];
}
//--------------------------------------------------------------
string recortadorImagenCara::getFeatureDescriptionByIndex(int index)
{
        return getFeatureDescription(featuresIndex[index]);
}
//--------------------------------------------------------------
void recortadorImagenCara::resetIsUpToDate()
{
    isUpToDate = false;
}
//--------------------------------------------------------------
bool recortadorImagenCara::getIsUpToDate()
{
    return isUpToDate;
}
//--------------------------------------------------------------
bool recortadorImagenCara::getBigEnoughFound()
{
    return (isUpToDate && bigEnoughFound);
}
//--------------------------------------------------------------
float recortadorImagenCara::getDLastFeatureBoxWidth()
{
    return dLastFeatureBoxWidth;
}
//--------------------------------------------------------------
