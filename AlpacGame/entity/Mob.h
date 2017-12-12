
#ifndef ALPACGAME_MOB_H
#define ALPACGAME_MOB_H

/**
 * Abstract class defining the mechanics for the random movement AI.
 * Includes a clock to determine if the AI is ready to switch to next action.
 * Used as a parent for NPC entities.
 *
 * Definition of a mob:
 *  1. An entity that is not controlled by player (i.e NPC), but by AI instead.
 */
class Mob : public EntityWarm{

protected:

    /**
     * CONSTRUCTOR: Defines the random number generator with given seed, and the clock
     * that is used to determine if the AI can switch to next action.
     * @param seeder additional seed to spice up the generator's seed.
     */
    explicit Mob(int seeder){
        // Creating Random Number Generator
        long long int seed = std::chrono::system_clock::now().time_since_epoch().count() + seeder;
        generator = std::default_random_engine(seed);
    }

    /**
     * Checks if the clock has surpassed the given time tick.
     * Used to determine if the AI is ready to switch action.
     * @param randomActionTick the time tick to compare the clock with.
     * @return true if the clock has surpassed the given time tick (Triggered), false
     * if not (Not Triggered).
     */
    bool randomActionTriggered(const float& randomActionTick){
        return isCooldownTriggered(&randomActionClock, randomActionTick);
    }



    /**
    * Generates a random number from the generator in the range of given lower and upper.
    * @param lower the left number in the range.
    * @param upper the right number in the range.
    * @return returns a number between the lower and upper.
    */
    int randomNumberGenerator(int lower, int upper) {
        std::uniform_int_distribution<int> distribution(lower, upper);
        return distribution(generator);
    }

    /**
     * The clock used to check if the AI is ready to switch action.
     */
    sftools::Chronometer randomActionClock{};



private:

    /**
     * The random number generator.
     */
    std::default_random_engine generator{};

};

#endif //ALPACGAME_MOB_H
