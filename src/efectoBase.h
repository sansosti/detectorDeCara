#ifndef EFECTOBASE_H
#define EFECTOBASE_H

#include "ofxFaceTracker.h"
#include "recortadorImagenCara.h"

class efectoBase
{
    public:
        efectoBase(ofVideoGrabber *aCamPtr, ofxFaceTracker *aTrackerPtr, recortadorImagenCara *aRecortador);

		virtual void setup();
		virtual void update();
		virtual void draw(int indiceRotacion);
		virtual void debug();

		virtual void keyPressed(int key);

        virtual string nombreEfecto() = 0;

    protected:
        ofVideoGrabber *camPtr;
        ofxFaceTracker *trackerPtr;
        recortadorImagenCara *recortadorPtr;
    private:
};

#endif // EFECTOBASE_H
