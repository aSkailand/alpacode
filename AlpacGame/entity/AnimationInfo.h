#ifndef ALPACGAME_ANIMATIONINFO_H
#define ALPACGAME_ANIMATIONINFO_H

#include <SFML/Graphics/Texture.hpp>

class AnimationInfo
{
public:

    int startFrame;
    int endFrame;
    std::vector<sf::Texture*> sprites;
};

#endif //ALPACGAME_ANIMATIONINFO_H
