#ifndef EFECTOCABEZAREBOTANTE_H
#define EFECTOCABEZAREBOTANTE_H

#include "efectoBase.h"


class efectoCabezaRebotante : public efectoBase
{
    public:
        efectoCabezaRebotante(ofVideoGrabber *aCamPtr, ofxFaceTracker *aTrackerPtr, recortadorImagenCara *aRecortador);
        virtual ~efectoCabezaRebotante();

        void setup();
        void update();
        void draw(int indiceRotacion);
        string nombreEfecto();

    protected:
        //ofImage videoImage;

        int posX,posY;
        int incX,incY;
        int speedX,speedY;
    private:
};

#endif // EFECTOCABEZAREBOTANTE_H
