#include "ClassCloth.h"

void Cloth::updateAllAccelerations() {
    for (int i = 0; i < this->height; i++) {
        for (int j = 0; j < this->width; j++) {
            Particle* ptrP = this->TABparticles[i][j];
            if (ptrP != nullptr) {
                ptrP->updateAcceleration(this->gravity);
            }
        }
        return;
    }
}

void Cloth::simulateVerlet(float dt) {
    for (int i = 0; i < this->height; i++) {
        for (int j = 0; j < this->width; j++) {
            Particle *ptrP = this->TABparticles[i][j];
            if (ptrP != nullptr) {
                ptrP->updateVelocity(dt);
                ptrP->updateAcceleration(this->gravity);
                ptrP->updateVerlet(dt);
            }
        }
    }
    return;
}

//
void Cloth::JakobsenMethod() {
    for (int i = 0; i < this->height; i++) {
        for (int j = 0; j < this->width; j++) {
            Particle* ptrP = this->TABparticles[i][j];
            if (ptrP != nullptr) {
                for (auto nearestP: ptrP->nearestParticles) {
                    if (nearestP != nullptr) {
                        sf::Vector2f delta_pos = nearestP->pos - ptrP->pos;

                        float current_distance = sqrt(delta_pos.x * delta_pos.x + delta_pos.y * delta_pos.y);

                        sf::Vector2f direction = delta_pos / current_distance;

                        float deltaDistance = current_distance - this->distance;

                        if ((deltaDistance > 0) && (deltaDistance >= this->distance*0.75)) {
                            ptrP->cutTwoParticles(nearestP);
                            continue;
                        }

                        sf::Vector2f correction = direction * (deltaDistance / 2.0f);

                        if (!ptrP->moving && nearestP->moving) {
                            nearestP->pos -= correction * 2.0f;
                        }
                        if (ptrP->moving && !nearestP->moving) {
                            ptrP->pos += correction * 2.0f;
                        }
                        if (ptrP->moving && nearestP->moving) {
                            ptrP->pos += correction;
                            nearestP->pos -= correction;
                        }
                    }
                }
            }
        }
    }
    return;
}

void Cloth::suppParticle(sf::Vector2f mousePos) {
    bool stop = false;
    for (int i = 0; i < this->height; i++) {
        for (int j = 0; j < this->width; j++) {
            if (stop) break;
            Particle* ptrP = this->TABparticles[i][j];
            if (ptrP != nullptr) {
                if ((mousePos.x + 5.0 > ptrP->pos.x) && (mousePos.x - 5.0 < ptrP->pos.x)) {
                    if ((mousePos.y + 5.0 > ptrP->pos.y) && (mousePos.y - 5.0 < ptrP->pos.y)) {

                        for (auto pNearest: this->TABparticles[i][j]->nearestParticles) {
                            if (pNearest == nullptr) continue;
                            for (auto &p: pNearest->nearestParticles) {
                                Particle** ptrp = &p;
                                if (p == ptrP) {
                                    p = nullptr;
                                }
                            }
                        }
                        this->TABparticles[i][j]= nullptr;
                        stop = true;
                        break;
                    }
                }
            }
        }
    }
}
