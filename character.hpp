#ifndef _CHARACTER_HPP_
#define _CHARACTER_HPP_

#include "entity.hpp"


/*!
    Character defines an entity class that works
    as a game character (player, enemy...).
    \author Javier Bermejo García
*/
class Character : public Entity {

protected:
    /*!
        Load the character settings.
        \param path the path of the settings file.
        \return true if success, false otherwise.
    */
    bool LoadSettings(const char *path);

public:
    /*!
        Character constructor.
    */
    Character(void);

    /*!
        Reset the character.
    */
    virtual void Reset(void);

    /*!
        Load a character entity.
        \param path the path of the settings file.
        \return true if success, false otherwise.
    */
    virtual bool Load(const char *path);

    /*!
        Load a character entity.
        \param path the path of the settings file.
        \return true if success, false otherwise.
    */
    virtual bool Load(string path);

    /*!
        Update the character state.
        \return true if success, false otherwise.
    */
    virtual bool Update(void);

    /*!
        Draw the character on the screen.
        \param idle if the character is idle.
        \return true if success, false otherwise.
    */
    virtual bool Draw(bool idle = false);
};

#endif
