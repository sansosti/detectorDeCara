#pragma once

#include "ofMain.h"
#include "ofxCv.h"
using namespace ofxCv;
using namespace cv;

#include "ofxXmlSettings.h"
#include "ofxFaceTracker.h"
#include "efectoBase.h"

#include "recortadorImagenCara.h"

class ofApp : public ofBaseApp {
public:
    enum TEfecto {
            EFECTO_ESPEJO_DE_TIZA,
            EFECTO_CABEZA_REBOTANTE
    };

	void setup();
	void update();
	void draw();
	void keyPressed(int key);

    void elegirProximoEfecto(TEfecto nuevoEfectoId);

    int capturaAncho;
    int capturaAlto;
	ofVideoGrabber cam;
	unsigned int        deviceId;
	ofxFaceTracker tracker;
	ofVec2f position;
	float scale;
	ofVec3f orientation;
	ofMatrix4x4 rotationMatrix;

	Mat translation, rotation;
	ofMatrix4x4 pose;

    string destPath;

    //ofImage videoImage;
    recortadorImagenCara recortador;

    TEfecto         efectoIdActual;
    efectoBase*     efectoActual;

protected:
    bool pictureTaken;
    bool bigPictureTaken;
    bool isTimeToTakePicture();
    bool isTimeToTakeBigPicture();

    ofxXmlSettings XML;
    void loadConfig();

    ofTrueTypeFont myFont;
    string fontFileName;
    int fontSize;

    int minFaceWidth;
    int minBigFaceWidth;

    string mensaje, porcionMensaje;
    int inicioPorcion,finPorcion,largoPorcion;
    int periodoPorcion;
    float porcionTimestamp;
    void extraerPorcionMensaje(int ultimoFinPorcion);

    int capturaRotacion;
    int rotacionFace;
    ofPixels camPixels;

    bool debugMode;
};
