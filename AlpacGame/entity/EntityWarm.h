
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

    enum class Direction{
        RIGHT, LEFT
    };

    enum class Action{
        IDLE, WALKING, JUMP
    };

    enum class Status{
        GROUNDED, AIRBORNE
    };

    Direction currentDirection = Direction::RIGHT;
    Action currentAction = Action::IDLE;
    Status currentStatus = Status::GROUNDED;





protected:

    /**
     * Wrapper for isCoolDownTriggered(), used to throttle movement using a clock.
     * Used to check if next movement is available to perform.
     * @param triggerTick the time tick to trigger clock.
     * @return true if the clock has surpassed triggerTick (Triggered), false if not (Not Triggered).
     */
    bool isMovementAvailable(const float &triggerTick){
        return isCooldownTriggered(&movementTriggerClock, triggerTick);
    }

    /**
     * The general function used by all mechanics revolving around clock and ticks.
     * Used to determine if the clock has triggered the given tick.
     * @param clock the clock to compare with.
     * @param triggerTick the time tick to check if the clock has surpassed or not.
     * @return true if the clock has surpassed triggerTick (Triggered), false if not (Not Triggered).
     */
    bool isCooldownTriggered(sf::Clock *clock, const float &triggerTick){
        bool triggered = clock->getElapsedTime().asSeconds() >= triggerTick;
        if(triggered){
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
