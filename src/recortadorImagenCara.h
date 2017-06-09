#ifndef RECORTADORIMAGENCARA_H
#define RECORTADORIMAGENCARA_H

#include "ofMain.h"
#include "ofxCv.h"
using namespace ofxCv;
using namespace cv;

#include "ofxFaceTracker.h"

#define FEATURE_COUNT 13
#define CURRENT_FEATURE_COUNT 1

class recortadorImagenCara
{
    public:
        recortadorImagenCara();
        virtual ~recortadorImagenCara();

        void setup(ofVideoGrabber *aCamPtr, ofxFaceTracker *aTrackerPtr,int aMinFaceWidth,int aMinBigFaceWidth);
        void update();

        ofImage* getFeatureImages();

        void resetIsUpToDate();
        bool getIsUpToDate();
        bool getBigEnoughFound();

        //int totalCapturas;

        string getFeatureDescription(ofxFaceTracker::Feature feature);
        string getFeatureDescriptionByIndex(int index);

        // Debug methods
        void setDebugMode(bool aDebugMode);
        float getDLastFeatureBoxWidth();

    protected:
        ofVideoGrabber *camPtr;
        ofxFaceTracker *trackerPtr;

        bool capturarFeature(int index);
        ofxFaceTracker::Feature featuresIndex[CURRENT_FEATURE_COUNT];
        ofImage currentFeatureImages[CURRENT_FEATURE_COUNT];
        string featureDescription[FEATURE_COUNT];
        bool isUpToDate;
        bool bigEnoughFound;

        int minFaceWidth;
        int minBigFaceWidth;

        // Debug vars
        bool debugMode;
        float dLastFeatureBoxWidth;

};

#endif // RECORTADORIMAGENCARA_H
