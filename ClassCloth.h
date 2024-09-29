#ifndef CLOTHES_CLASSCLOTH_H
#define CLOTHES_CLASSCLOTH_H

#include "ClassParticle.h"

class Cloth {
public:
    int         width;
    int         height;
    int         distance;
    float       gravity;
    Particle*   TABparticles[40][40];

    void updateAllAccelerations();
    void simulateVerlet(float);
    void JakobsenMethod();

    void suppParticle(sf::Vector2f);

    Cloth(int x, int y, int w, int h, float d, float grav, float frict): width(w), height(h), distance(d), gravity(grav) {
        int id = 0;
        for (int i = 0; i < h; i++) {
            for (int j = 0; j < w; j++) {
                id++;
                Particle* ptrNewP = new Particle(i*distance + x, j*distance + y, 5.f);
                // CReating initial velocity
                ptrNewP->prev_pos = ptrNewP->pos - sf::Vector2f {0.f, 0.f};
                TABparticles[i][j] = ptrNewP;
                ptrNewP->friction = frict;
                ptrNewP->id = id;
                if (j == 0) {
                    ptrNewP->moving = false;
                } else {
                    ptrNewP->moving = true;
                }
            }
        }

        for (int i = 0; i < h; i++) {
            for (int j = 0; j < w; j++) {
                Particle* ptrP = TABparticles[i][j];
                if (i != 0) {
                    ptrP->nearestParticles.push_back(TABparticles[i-1][j]);
                    //ptrP->nearParticle
                } if (i != h-1) {
                    ptrP->nearestParticles.push_back(TABparticles[i+1][j]);
                } if (j != 0) {
                    ptrP->nearestParticles.push_back(TABparticles[i][j-1]);
                } if (j != w-1) {
                    ptrP->nearestParticles.push_back(TABparticles[i][j+1]);
                }
            }
        }
    }
};

#endif //CLOTHES_CLASSCLOTH_H