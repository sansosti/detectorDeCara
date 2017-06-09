#include "ofApp.h"

#include "recortadorImagenCara.h"
#include "efectoEspejoDeTiza.h"
#include "efectoCabezaRebotante.h"

using namespace ofxCv;

#define DEFAULT_CAPTURA_ANCHO 640
#define DEFAULT_CAPTURA_ALTO 480
#define DEFAULT_CAPTURA_ROTACION 0
#define DEFAULT_DEVICE_ID 0

#define DEFAULT_DEST_PATH "caras"

#define DEFAULT_FONT_FILENAME "arial.ttf"
#define DEFAULT_FONT_SIZE 8

#define DEFAULT_MIN_FACE_WIDTH 100
#define DEFAULT_MIN_BIG_FACE_WIDTH 260
#define DEFAULT_ROTACION_FACE 0

#define DEFAULT_MENSAJE_TEXTO "Mensaje vacío"
#define DEFAULT_MENSAJE_LARGO_PORCION 20
#define DEFAULT_MENSAJE_PERIODO_PORCION 1

const string ConfigFile = "config.xml";

//--------------------------------------------------------------
void ofApp::setup() {
/*
    ofBackground(0,0,0);
	ofSetVerticalSync(true);
	ofSetDrawBitmapMode(OF_BITMAPMODE_MODEL_BILLBOARD);
*/

    loadConfig();

    myFont.loadFont(fontFileName,fontSize);

	cam.setVerbose(true);
	cam.setDeviceID(deviceId);
	cam.initGrabber(capturaAncho, capturaAlto);

	ofLogNotice() << "Listado de Dispositivos";
    cam.listDevices();

	ofLogNotice() << "Captura, tamaño real: " + ofToString(cam.width) + ":" + ofToString(cam.height);

	tracker.setup();

    recortador.setup(&cam, &tracker,minFaceWidth,minBigFaceWidth);
    recortador.setDebugMode(debugMode);

    pictureTaken = false;
    bigPictureTaken = false;

	elegirProximoEfecto(EFECTO_ESPEJO_DE_TIZA);

    extraerPorcionMensaje(-1);
}
//--------------------------------------------------------------
void ofApp::loadConfig() {
    if( XML.loadFile(ConfigFile) ){
		ofLogNotice() << "Cargando configuracion desde '" + ConfigFile + "'";
	} else {
		ofLogNotice() << "Archivo de configuracion '" + ConfigFile + "' no encontrado";
		//return;
	}


	debugMode        = (XML.getValue("ROBACARAS:DEBUG_MODE",0) == 1);
    deviceId         = XML.getValue("ROBACARAS:CAMARA:DISPOSITIVO",DEFAULT_DEVICE_ID);
	destPath        = XML.getValue("ROBACARAS:DESTINO:PATH",DEFAULT_DEST_PATH);
	capturaAncho    = XML.getValue("ROBACARAS:CAPTURA:ANCHO",DEFAULT_CAPTURA_ANCHO);
	capturaAlto     = XML.getValue("ROBACARAS:CAPTURA:ALTO",DEFAULT_CAPTURA_ALTO);
	// capturaRotacion: n = clockwise right angle times
	capturaRotacion  = XML.getValue("ROBACARAS:CAPTURA:ROTACION",DEFAULT_CAPTURA_ROTACION);
    fontFileName     = XML.getValue("ROBACARAS:FONT:FILENAME",DEFAULT_FONT_FILENAME);
    fontSize        = XML.getValue("ROBACARAS:FONT:SIZE",DEFAULT_FONT_SIZE);
    minFaceWidth    = XML.getValue("ROBACARAS:CARA:MIN_WIDTH",DEFAULT_MIN_FACE_WIDTH);
    minBigFaceWidth    = XML.getValue("ROBACARAS:CARA:MIN_BIG_WIDTH",DEFAULT_MIN_BIG_FACE_WIDTH);
    // rotacionFace: n = clockwise right angle times
    rotacionFace    = XML.getValue("ROBACARAS:CARA:ROTACION",DEFAULT_ROTACION_FACE);
    mensaje         = XML.getValue("ROBACARAS:MENSAJE:TEXTO",DEFAULT_MENSAJE_TEXTO);
    largoPorcion    = XML.getValue("ROBACARAS:MENSAJE:LARGO_PORCION",DEFAULT_MENSAJE_LARGO_PORCION);
    periodoPorcion  = XML.getValue("ROBACARAS:MENSAJE:PERIODO_PORCION",DEFAULT_MENSAJE_PERIODO_PORCION);

    ofLogNotice() << "dispositivo: " + ofToString(deviceId);
    ofLogNotice() << "destPath: " + destPath;
    ofLogNotice() << "capturaAncho: " + ofToString(capturaAncho);
    ofLogNotice() << "capturaAlto: " + ofToString(capturaAlto);
    ofLogNotice() << "fontFileName: " + fontFileName;
    ofLogNotice() << "fontSize: " + ofToString(fontSize);
    ofLogNotice() << "minFaceWidth: " + ofToString(minFaceWidth);
    ofLogNotice() << "minBigFaceWidth: " + ofToString(minBigFaceWidth);
    ofLogNotice() << "largoPorcion: " + ofToString(largoPorcion);
    ofLogNotice() << "periodoPorcion: " + ofToString(periodoPorcion);
}
//--------------------------------------------------------------
void ofApp::elegirProximoEfecto(TEfecto nuevoEfectoId) {

    if (efectoActual) {
        delete efectoActual;
    }

    efectoIdActual = nuevoEfectoId;

    switch (efectoIdActual)
    {
        case EFECTO_ESPEJO_DE_TIZA:
            efectoActual = new efectoEspejoDeTiza(&cam, &tracker, &recortador);
            break;
        case EFECTO_CABEZA_REBOTANTE:
            efectoActual = new efectoCabezaRebotante(&cam, &tracker, &recortador);
            break;
    }

    efectoActual->setup();

    ofLog() << ofGetTimestampString() + ",EFECTO," + efectoActual->nombreEfecto();
}
//--------------------------------------------------------------
void ofApp::update() {
	cam.update();
	if(cam.isFrameNew()) {
        if (capturaRotacion == 0) {
            tracker.update(toCv(cam));
        } else {
            cam.getTextureReference().readToPixels(camPixels);
            camPixels.rotate90(capturaRotacion);
            tracker.update(toCv(camPixels));
        }

		if (!tracker.getFound()) {
            pictureTaken = false;
            bigPictureTaken = false;
		}

		recortador.update();
        efectoActual->update();
	}

    if ((recortador.getIsUpToDate() && isTimeToTakePicture()) ||
        (recortador.getBigEnoughFound() && isTimeToTakeBigPicture())) {

        if (recortador.getIsUpToDate() && isTimeToTakePicture()) {
            if (debugMode) {
                ofLogNotice() << "Voy a tomar foto inicial";
            }
            pictureTaken = true;
        }
        if (recortador.getBigEnoughFound() && isTimeToTakeBigPicture()) {
            if (debugMode) {
                ofLogNotice() << "Voy a tomar big picture";
            }
            bigPictureTaken = true;
        }
        ofImage* imagesList = recortador.getFeatureImages();
        for (int i=0;i<CURRENT_FEATURE_COUNT;i++) {
            imagesList[i].saveImage(destPath+"/"+recortador.getFeatureDescriptionByIndex(i)+ "-"+ofGetTimestampString("%Y_%m_%d_%H_%M_%s")+".png");
        }
    }
}
//--------------------------------------------------------------
void ofApp::draw() {
	ofSetColor(255);
	ofSetOrientation(OF_ORIENTATION_DEFAULT);
	ofScale(1,1,1);
	//cam.draw(0, 0);
//	ofDrawBitmapString(ofToString((int) ofGetFrameRate()), 100, 20);

    if (recortador.getIsUpToDate()) {
        /*
        if (isTimeToTakePicture()) {
            pictureTaken = true;
            ofImage* imagesList = recortador.getFeatureImages();
            for (int i=0;i<CURRENT_FEATURE_COUNT;i++) {
                imagesList[i].saveImage(destPath+"/"+recortador.getFeatureDescriptionByIndex(i)+ "-"+ofGetTimestampString("%Y_%m_%d_%H_%M_%s")+".png");
            }
        }
        */
        efectoActual->draw(rotacionFace);
    } else {
        int refWidth,refHeight;
        switch (rotacionFace)
        {
        case -1:
            ofSetOrientation(OF_ORIENTATION_90_RIGHT);
            break;
        case 1:
            ofSetOrientation(OF_ORIENTATION_90_LEFT);
            break;
        default:
            break;
        }
        if (rotacionFace == 0) {
            refWidth = ofGetWindowWidth();
            refHeight = ofGetWindowHeight();
        } else {
            refWidth = ofGetWindowHeight();
            refHeight = ofGetWindowWidth();
        }

        if (debugMode) {
            if (capturaRotacion == 0) {
                cam.draw(0,0);
            } else {
                ofImage videoImage;
                videoImage.setFromPixels(camPixels);
                videoImage.draw(0,0);
            }
        }
        float tiempoTranscurrido = ofGetElapsedTimef() - porcionTimestamp;
        if (tiempoTranscurrido > periodoPorcion) {
            extraerPorcionMensaje(finPorcion);
        }
        ofRectangle stringRect = myFont.getStringBoundingBox(porcionMensaje,0,0);
        //myFont.drawString(porcionMensaje, (ofGetWindowWidth()-stringRect.width)/2, ofGetWindowHeight()/4);
        myFont.drawString(porcionMensaje, (refWidth-stringRect.width)/2, refHeight/4);
    }

    if (debugMode) {
        //ofSetOrientation(OF_ORIENTATION_DEFAULT,false);
        ofDrawBitmapString("Last Feature Box width: " + ofToString(recortador.getDLastFeatureBoxWidth()), 200, 100);
    }
 //   efectoActual->draw();

//    ofLog() << ofGetTimestampString() + " TotalCapturas: " + ofToString(recortador.totalCapturas);

//    ofSetOrientation(OF_ORIENTATION_DEFAULT,false);

/*
	if(tracker.getFound()) {
		ofSetLineWidth(1);
ofSetOrientation(OF_ORIENTATION_180,false);
		tracker.draw();
*/
/*
        int x = ((ofGetWindowWidth()-videoImage.width)/2)  + sin(ofGetElapsedTimef())*((ofGetWindowWidth()-videoImage.width)/2);
        int y = ((ofGetWindowHeight()-videoImage.height)/2)  + sin(ofGetElapsedTimef())*((ofGetWindowHeight()-videoImage.height)/2);
		videoImage.draw(x,y);
*/
/*
		float jawOpenness = tracker.getGesture(ofxFaceTracker::JAW_OPENNESS);
		ofDrawBitmapString("jawOpenness : " + ofToString(jawOpenness), 10, 40);
		float mouthHeight = tracker.getGesture(ofxFaceTracker::MOUTH_HEIGHT);
		ofDrawBitmapString("mouthHeight : " + ofToString(mouthHeight), 10, 60);


		//ofSetupScreenOrtho(640, 480, OF_ORIENTATION_UNKNOWN, true, -1000, 1000);
        ofSetOrientation(OF_ORIENTATION_UNKNOWN, true);
        ofSetupScreenOrtho(640, 480, -1000, 1000);

		ofTranslate(640 / 2, 480 / 2);
		applyMatrix(rotationMatrix);
		ofScale(5,5,5);
		ofDrawAxis(scale);
		tracker.getObjectMesh().drawWireframe();
*/
/*
	}
*/
}
//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
    switch (key)
    {
    case 'r':
    case 'R':
        tracker.reset();
        break;
    case '1':
        elegirProximoEfecto(EFECTO_ESPEJO_DE_TIZA);
        break;
    case '2':
        elegirProximoEfecto(EFECTO_CABEZA_REBOTANTE);
        break;
    default:
        break;
    }

}
//--------------------------------------------------------------
bool ofApp::isTimeToTakePicture()
{
//    return (ofGetFrameNum()%(5*20) == 0);
    return !pictureTaken;
}
//--------------------------------------------------------------
bool ofApp::isTimeToTakeBigPicture()
{
//    return (ofGetFrameNum()%(5*20) == 0);
    return !bigPictureTaken;
}
//--------------------------------------------------------------
void ofApp::extraerPorcionMensaje(int ultimoFinPorcion)
{
    inicioPorcion = ultimoFinPorcion+1;
    if (inicioPorcion >= mensaje.length()) {
        inicioPorcion = 0;
    }
    finPorcion = inicioPorcion + largoPorcion;
    if (finPorcion >= mensaje.length()) {
        finPorcion = mensaje.length() - 1;
    }
    finPorcion = mensaje.find_first_of(' ',finPorcion);

    //ofLogNotice() << ofToString(inicioPorcion) + ":" + ofToString(finPorcion);

    porcionMensaje = string(mensaje.c_str(),inicioPorcion,finPorcion-inicioPorcion);

    porcionTimestamp = ofGetElapsedTimef();
}
//--------------------------------------------------------------
