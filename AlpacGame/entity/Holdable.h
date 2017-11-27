//
// Created by Trong on 17/11/2017.
//

#ifndef ALPACGAME_HOLDABLE_H
#define ALPACGAME_HOLDABLE_H

struct Holdable{
    bool isHeld = false;
    bool farmerTouch = false;

    virtual void performHold() = 0;
    virtual void performThrow() = 0;

};

#endif //ALPACGAME_HOLDABLE_H
