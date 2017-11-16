#ifndef ALPACGAME_ANIMATIONINFO_H
#define ALPACGAME_ANIMATIONINFO_H

//#include <SFML/Graphics/Texture.hpp>

struct SpriteInfo {
    int startFrame;
    int endFrame;
    std::vector<sf::Texture*> sprites;
};

#endif //ALPACGAME_ANIMATIONINFO_H
