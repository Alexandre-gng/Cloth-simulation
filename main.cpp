#include "ClassCloth.h"

using namespace std;

int main() {
    cout << "STARTING..." << endl;

    const float DELTA_T_S = 0.005;
    const short DELTA_T_MS = DELTA_T_S*1000;

    Cloth* ptrCloth;

    hash<float> a;

    const float MIN_DISTANCE_ROPE = 6.f;

    ptrCloth = new Cloth(250,200,17, 25, 15.f, 2500.f, 0.05f);

    sf::RenderWindow window(sf::VideoMode(800, 600), "cloth simulation");

    while (window.isOpen()) {
        this_thread::sleep_for(chrono::milliseconds(DELTA_T_MS));
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                sf::Vector2f newMousePos = window.mapPixelToCoords(mousePos);
                ptrCloth->suppParticle(newMousePos);
            }
        }
        window.clear(sf::Color::Black);

            ptrCloth->updateAllAccelerations();
            ptrCloth->simulateVerlet(DELTA_T_S);
            ptrCloth->JakobsenMethod();

        for (int i = 0; i < ptrCloth->height; i++) {
            for (int j = 0; j < ptrCloth->width; j++) {
                Particle *ptrP = ptrCloth->TABparticles[i][j];
                if (ptrP == nullptr) continue;
                float forceTOT = sqrt(ptrP->forces.x*ptrP->forces.x + ptrP->forces.y*ptrP->forces.y);
                ptrP->shape.setPosition(ptrP->pos.x, ptrP->pos.y);
                window.draw(ptrP->shape);
            }
        }
        window.display();
    }

    return 0;
}
