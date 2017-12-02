
#ifndef ALPACGAME_ENTITYWARM_H
#define ALPACGAME_ENTITYWARM_H

#include "Entity.h"


/**
 * This class is a derived class of the class Entity.
 * The definition of a warm entity is:
 *  1. An entity (Check Entity.h).
 *  2. Can move by own will.
 */
class EntityWarm : public Entity {

public:

    enum class Health {
        ALIVE = 0,
        DEAD = 1,
        GHOST = 2
    };

    Health currentHealth = Health::ALIVE;

    sf::RectangleShape *sf_ShapeGhost = nullptr;

    sftools::Chronometer deathClock;
    float ghostTick = 15.0f;
    float decayTick = 5.0f;
    float deathTick = 3.0f;

    int HP = 0;

    void dealDamage(int damage) {
        HP -= damage;
    }

    void handleHealth() {

        switch (currentHealth) {

            case Health::ALIVE: {

                if (HP <= 0) {

                    // Create filter of dead entity
                    b2Filter deadFilter;
                    deadFilter.categoryBits = (uint16) getEntity_ID();
                    deadFilter.maskBits = (uint16) ID::PLANET;

                    // Set filter
                    fixture_body->SetFilterData(deadFilter);
                    fixture_hit->SetFilterData(deadFilter);
                    if (fixture_detection) fixture_detection->SetFilterData(deadFilter);

                    // Proceed state
                    currentHealth = Health::DEAD;
                    deathClock.reset(true);

                }
                break;
            }

            case Health::DEAD: {

                if (deathClock.getElapsedTime().asSeconds() >= deathTick) {

                    // Set update ghostShape
                    sf_ShapeGhost->setPosition(sf_ShapeEntity->getPosition());
                    sf_ShapeGhost->setRotation(sf_ShapeEntity->getRotation() - 45);
//                    ghostShape->rotate();

                    // Proceed state
                    currentHealth = Health::GHOST;
                    deathClock.reset(true);
                }
                break;

            }
            case Health::GHOST: {

                if (deathClock.getElapsedTime().asSeconds() >= ghostTick) {

                    // Stop clock to kill entity
                    deathClock.reset(false);
                }
                break;

            }

        }
    }

    /**
     * The label above entities' head in-game.
     */
    sf::Text *label_ID = new sf::Text();
    sf::Text *label_HP = new sf::Text();

    void createLabel(sf::Text *label, sf::Font *font, std::string text) {
        label->setString(text);
        label->setFont(*font);
        label->setCharacterSize(40);
        label->setFillColor(sf::Color::White);
        label->setOutlineColor(sf::Color::Black);
        label->setOutlineThickness(3);
        label->setOrigin(label->getLocalBounds().width / 2.f, label->getLocalBounds().height / 2.f);
    };

    /// Action Functions

    /**
     * Changes the current action accordingly to the entity it is called by.
     */
    virtual void switchAction() = 0;

    /**
     * Perform the current action chosen.
     */
    virtual void performAction() = 0;

    /// Enums

    enum class Action {
        IDLE = 0, WALKING = 1, JUMP = 2
    };



    Direction currentDirection = Direction::RIGHT;
    Action currentAction = Action::IDLE;
    Status currentStatus = Status::AIRBORNE;


protected:

    sftools::Chronometer behaviorClock;


    /**
     * Wrapper for isCoolDownTriggered(), used to throttle movement using a clock.
     * Used to check if next movement is available to perform.
     * @param triggerTick the time tick to trigger clock.
     * @return true if the clock has surpassed triggerTick (Triggered), false if not (Not Triggered).
     */
    bool isMovementAvailable(const float &triggerTick) {
        return isCooldownTriggered(&movementTriggerClock, triggerTick);
    }

    /**
     * The general function used by all mechanics revolving around clock and ticks.
     * Used to determine if the clock has triggered the given tick.
     * @param clock the clock to compare with.
     * @param triggerTick the time tick to check if the clock has surpassed or not.
     * @return true if the clock has surpassed triggerTick (Triggered), false if not (Not Triggered).
     */
    bool isCooldownTriggered(sf::Clock *clock, const float &triggerTick) {
        bool triggered = clock->getElapsedTime().asSeconds() >= triggerTick;
        if (triggered) {
            clock->restart();
        }
        return triggered;
    }

private:

    /**
     * Clock used to determine if entity is allowed to move or not.
     */
    sf::Clock movementTriggerClock{};


};

#endif //ALPACGAME_ENTITYWARM_H
