#ifndef CLOTHES_CLASSPARTICLE_H
#define CLOTHES_CLASSPARTICLE_H

#include <vector>
#include <math.h>
#include <iostream>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System/Vector2.hpp>

#include <thread>
#include <chrono>

using namespace std;

class Particle {
public:
    bool         moving      = true;
    int          id;
    float        friction;
    float        mass        = 1.0f;
    sf::Vector2f pos;
    sf::Vector2f prev_pos;
    sf::Vector2f forces;
    sf::Vector2f velocity;
    sf::Vector2f acc;

    std::vector<Particle*> nearestParticles;
    sf::CircleShape shape;



    void applyFriction() {
        forces -= velocity * friction;
    }
    void applyGravity(float grav) {
        if (moving) {
            forces.y += mass * grav;
        }
    }
    void updateAcceleration(float grav) {
        forces = {0.f, 0.f};
        applyGravity(grav);
        applyFriction();
        acc = forces * (1/mass);
    }
    void updateVelocity(float dt) {
        this->velocity = (pos - prev_pos) / dt;
    }
    void cutTwoParticles(Particle* Pa) {
        for (auto &PaNearest: Pa->nearestParticles) {
            if (PaNearest == this) {
                PaNearest = nullptr;
            }
        }
        for (auto &PbNearest: this->nearestParticles) {
            if (PbNearest == Pa) {
                PbNearest = nullptr;
            }
        }
    }

    void updateVerlet (float dt) {
        if (this->moving) {
            sf::Vector2f posCopy = this->pos;
            this->pos = 2.f*this->pos - this->prev_pos + (dt * dt) * this->acc;
            this->prev_pos = posCopy;
        }
    }

    Particle(float x, float y, float m) : pos(sf::Vector2f {x,y}), prev_pos(sf::Vector2f {x,y}), mass(m) {
        // SFML Shape creation
        sf::CircleShape ParticleShape(2.f);
        ParticleShape.setFillColor(sf::Color(255, 255, 255));
        this->shape.setPosition(x, y);
        this->shape = ParticleShape;
    }
    Particle(float xprev, float yprev, float x, float y) {
        this->pos = {x, y};
        this->prev_pos = {x, y};
    }
};

#endif //CLOTHES_CLASSPARTICLE_H