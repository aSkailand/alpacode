
#ifndef ALPACGAME_ENTITYWARM_H
#define ALPACGAME_ENTITYWARM_H

#include "Entity.h"
#include "HitPoint/HitPointBarometer.h"


/**
 * This class is a derived class of the class Entity.
 * The definition of a warm entity is:
 *  1. An entity (Check Entity.h).
 *  2. Can move by own will.
 */
class EntityWarm : public Entity {

public:

    /// HEALTH LOGIC

    /*
     * HEALTH LOGIC EXPLANATION:
     *
     * The health logic of an entity is following an one-way, acyclic pattern,
     * meaning that when the state changes, it won't be able to return to
     * the previous state.
     *
     * A warm entity must be in one of three different health states:
     *      1. Alive:  Entity is alive and will behave normally.
     *      2. Dead:   Entity is dead, and will therefore not move anymore.
     *      3. Ghost:  Entity is still dead, and the ghost of it has spawned.
     *
     * Note: A warm entity is to be deleted if the two statements are true:
     *          1. The entity is in the ghost state.
     *          2. The deathClock has stopped.
     *
     * Note: A cold entity can also "die" and be deleted, but a warm entity has
     *       additional logic to it to handle the ghost and decay of the entity.
     */

    // Health states
    enum class Health {
        ALIVE = 0,
        DEAD = 1,
        GHOST = 2
    };

    // Current Health state
    Health currentHealth = Health::ALIVE;

    // The shape of the ghost
    sf::RectangleShape *sf_ShapeGhost = nullptr;

    // The Clock to handle the death + ghost state
    sftools::Chronometer deathClock;

    float deathTick = 3.0f;     // How many seconds after killing blow before change to ghost state.
    float decayTick = 5.0f;     // How many seconds after death before the decay will start.
    float ghostTick = 15.0f;    // How many seconds the ghost will last.

    // Health points
    int HP = 0;

    /// Visual Entity Info
    sf::Shape* get_sf_ShapeEntity() const {
        return sf_ShapeEntity;
    }

    bool currentlyMousedOver = false;


    /**
     * How much damage to substract from entity's HP.
     * @param damage the amount to substract from entity's HP.
     */
    void dealDamage(int damage) {
        HP -= damage;
    }

#include "../Resources/ConfigGame.h"



    void removeEntityCollision(){

        // Create filter of dead entity
        b2Filter deadFilter;
        deadFilter.categoryBits = (uint16) getEntity_ID();
        deadFilter.maskBits = (uint16) ID::PLANET;

        // Set filter
        fixture_body->SetFilterData(deadFilter);
        fixture_hit->SetFilterData(deadFilter);
        if (fixture_detection) fixture_detection->SetFilterData(deadFilter);

    }

    /**
     * Handles the health state logic.
     * In short, it handles when to transition through the health states.
     * In detail, when:
     *      Alive:  Check if hp <= 0. Move to dead state if true.
     *      Dead:   Check if it is time to change to ghost state.
     *      Ghost:  Check if it is time to stop death clock.
     */
    void handleHealth(unsigned int* numOfAlpacas = nullptr) {

        switch (currentHealth) {

            case Health::ALIVE: {

                if (HP <= 0) {

                    removeEntityCollision();

                    if(entity_ID == ID::ALPACA){
                        *numOfAlpacas -= 1;
                    }

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

                    // Proceed state
                    currentHealth = Health::GHOST;
                    deathClock.reset(true);
                }
                break;

            }
            case Health::GHOST: {

                if (deathClock.getElapsedTime().asSeconds() >= ghostTick) {

                    // Stop clock to signal deletion of entity
                    deathClock.reset(false);
                }
                break;

            }

        }
    }


    /**
     * The label above entities' head in-game.
     */
    sf::Text label_ID{};

    HitPointBarometer *hitPointBarometer = nullptr;;

    /// Action Functions

    /**
     * Changes the current action accordingly to the entity it is called by.
     */
    virtual void switchAction() = 0;

    /**
     * Perform the current action chosen.
     */
    virtual void performAction() = 0;


    virtual void switchCurrentTexture() = 0;

    void calcShapeEntityPlacement() {

        float delta_Y = sf_ShapeEntity->getLocalBounds().height / 2 - fixture_body->GetShape()->m_radius * SCALE;
        b2Vec2 offsetPoint = getBody()->GetWorldPoint(b2Vec2(0.f, -delta_Y / SCALE));

        float shape_x = offsetPoint.x * SCALE;
        float shape_y = offsetPoint.y * SCALE;

        sf_ShapeEntity->setPosition(shape_x, shape_y);
        sf_ShapeEntity->setRotation((getBody()->GetAngle() * DEGtoRAD));

    }

    void renderDeath() {

        if(!deathClock.isRunning())
            return;

        // Fade Entity Shape when decay begins
        if (deathClock.getElapsedTime().asSeconds() >= decayTick) {
            renderFadeOut();
        }

        // Move Ghost upwards
        b2Vec2 ghostMovementVector = getBody()->GetWorldVector(b2Vec2(0.f, -0.01f));
        sf_ShapeGhost->move(sf::Vector2f(ghostMovementVector.x * SCALE, ghostMovementVector.y * SCALE));

    }

    void renderFadeOut(){
        sf_ShapeEntity->setFillColor(sf_ShapeEntity->getFillColor() - sf::Color(0, 0, 0, 1));
    }

    void renderFadeIn(){
        sf_ShapeEntity->setFillColor(sf_ShapeEntity->getFillColor() + sf::Color(0, 0, 0, 1));
    }



    virtual void renderDebugMode() = 0;

    /// Enums

    enum class Action {
        IDLE = 0, WALKING = 1, JUMP = 2
    };

    Direction currentDirection = Direction::RIGHT;
    Action currentAction = Action::IDLE;
    Status currentStatus = Status::AIRBORNE;


protected:

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
    bool isCooldownTriggered(sftools::Chronometer *clock, const float &triggerTick) {
        bool triggered = clock->getElapsedTime().asSeconds() >= triggerTick;
        if (triggered) {
            clock->reset(true);
        }
        return triggered;
    }

protected:

    /**
     * Clock used to determine if entity is allowed to move or not.
     */
    sftools::Chronometer movementTriggerClock{};

};

#endif //ALPACGAME_ENTITYWARM_H
