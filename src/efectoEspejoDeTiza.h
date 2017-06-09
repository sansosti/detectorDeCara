#ifndef EFECTOESPEJODETIZA_H
#define EFECTOESPEJODETIZA_H

#include "efectoBase.h"


class efectoEspejoDeTiza : public efectoBase
{
    public:
        efectoEspejoDeTiza(ofVideoGrabber *aCamPtr, ofxFaceTracker *aTrackerPtr, recortadorImagenCara *aRecortador);
        virtual ~efectoEspejoDeTiza();

        void setup();
        void draw(int indiceRotacion);
        string nombreEfecto();

    protected:
    private:
};

#endif // EFECTOESPEJODETIZA_H
