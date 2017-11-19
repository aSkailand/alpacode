#ifndef ALPACGAME_ANIMATIONINFO_H
#define ALPACGAME_ANIMATIONINFO_H

struct SpriteInfo {

    int startFrame;
    int endFrame;

    std::vector<sf::Texture*> sprites;

    /**
     * Operator overloading [] to access sprites vector with a shorter syntax.
     * @param a index of sprites.
     * @return the correct texture from the sprites.
     */
    sf::Texture* operator[](int a) const{
        return sprites.at(static_cast<unsigned int>(a));
    }

};

#endif //ALPACGAME_ANIMATIONINFO_H
