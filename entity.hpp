#ifndef _ENTITY_HPP_
#define _ENTITY_HPP_

#include <map>
#include <string>

#include <SDL/SDL.h>

#include "sample.hpp"
#include "script.hpp"
#include "surface.hpp"

//! Entity motion values.
enum {
    DEFAULT         = 0,
    ENTITY_LEFT     = 1,
    ENTITY_RIGHT    = 2,
    ENTITY_UP       = 4,
    ENTITY_DOWN     = 8,
    ENTITY_STOP     = 16,
    ENTITY_INACTIVE = 32
};

//! Entity state values.
enum {
    ENTITY_IDLE		 = 0,
    ENTITY_WALK		 = 1,
    ENTITY_RUN		 = 2,
    ENTITY_JUMP 	 = 4,
    ENTITY_FALL		 = 8,
    ENTITY_DEAD		 = 16,
    ENTITY_INVISIBLE = 32
};


/*!
    Entity is the class that defines the interactable
    objects of the game (player, enemies, items...).
    \author Javier Bermejo García
*/
class Entity {
protected:
    Surface mAnimation;		//!< Animation surface.
    Sample  mSound;			//!< Entity sound.

    Uint16 mWidth;			//!< Sprite width.
    Uint16 mHeight;			//!< Sprite height.

    float mSpeed;			//!< Entity speed
    float mSpeedX;			//!< Current horizontal speed.
    float mSpeedY;			//!< Current vertical speed.
    float mForceX;			//!< Current horizontal force.
    float mForceY;			//!< Current vertical force.
    
    map<Sint32, Uint16> mAniMap; //!< Animation map.
    
    bool mNoGravity;		//!< Gravity affect.
    bool mLoaded;			//!< Loaded flag.
    
    Uint16 mEntityNumber;	//!< Entity number with the same ID.
    Uint16 mId;				//!< Entity ID.

    string mType;			//!< Entity type.
    string mName;			//!< Entity name.
    string mBasepath;		//!< Entity path.
    string mSprite;			//!< Sprite path.
    
    Uint32 mPoints;			//!< Entity value in points.
    Uint16 mMotion;			//!< Current direction.
    
    bool mCollidable;		//!< Collidable flag.
    
    Uint16 mState;			//!< Current state.
    Uint16 mCollisionRect;	//!< Number of collision rectangles.

    float mAccelX;			//!< Current horizontal acceleration.
    float mAccelY;			//!< Current vertical acceleration.

    float mStartX;			//!< X-axis start position.
    float mStartY;			//!< Y-axis start position.
    
    Rect mRect;				//!< Display rectangle.
    
public:
    vector<Rect *> mCollisions; //!< Collision vector.

protected:
    /*!
        Load the entity settings.
        \param filepath the settings file path.
        \return true if success, false otherwise.
    */
    virtual bool LoadSettings(const char *filepath);
    
    /*!
        Load the entity collisions.
        \param Script the script with the entity settings.
        \return true if success, false otherwise.
    */
    virtual bool LoadCollisions(Script script);

    /*!
        Apply the enviroment forces to the entity.
    */
    virtual void Physics(void);
    
    /*!
        Unload the entity.
    */
    virtual void Unload(void);

public:
    /*!
        Entity constructor.
    */
    Entity(void);

    /*!
        Entity destructor.
    */
    virtual ~Entity(void);

    /*!
        Reset the entity.
    */
    virtual void Reset(void);

    /*!
        Load an entity.
        \param filepath the settings file path.
        \return true if success, false otherwise.
    */
    virtual bool Load(const char *filepath);

    /*!
        Load an entity.
        \param filepath the settings file path.
        \return true if success, false otherwise.
    */
    virtual bool Load(string filepath);

    /*!
        Entity collision handler.
        \param Entity the entity that collided with the enemy.
        \param col the collision bitmap.
    */
    virtual void Collision(Entity *Entity, Uint32 &col);

    /*!
        Update the entity state.
        \return true if success, false otherwise.
    */
    virtual bool Update(void);

    /*!
        Draw the entity on the screen.
        \param idle if the character is idle.
        \return true if success, false otherwise.
    */
    virtual bool Draw(bool idle = false);

    /*!
        Move the entity.
        \param dir the walking direction.
    */
    virtual void Move(Uint16 dir);
    
    /*!
        Update entity animation.
    */
    void Run(void);

    /*!
        Make the entity stop.
    */
    void Stop(void);

    /*!
        Kill the entity.
    */
    void Kill(void);
    
    /*!
        Get entity type.
        \return the entity type.
    */
    string GetType(void);
    
    /*!
        Get entity name.
        \return the entity name.
    */
    string GetName(void);
    
    /*!
        Get entity points.
        \return the entity points.
    */
    Uint32 GetPoints(void);
    
    /*!
        Get entity rectangle.
        \return the entity rectangle.
    */
    Rect GetRect(void);
    
    /*!
        Get entity collidable.
        \return the entity collidable.
    */
    bool GetCollidable(void);
    
    /*!
        Get entity state.
        \return the entity state.
    */
    Uint16 GetState(void);
    
    /*!
        Get entity motion.
        \return the entity motion.
    */
    Uint16 GetMotion(void);
    
    /*!
        Get entity Id.
        \return the entity Id.
    */
    Uint16 GetId(void);
    
    /*!
        Get entity number.
        \return the entity number.
    */
    Uint16 GetEntityNumber (void);

    /*!
        Set the motion state of the entity.
        \param value the motion state of the entity.
    */
    void SetMotion(Uint16 value);
    
    /*!
        Set the initial position.
        \param x the initial X position.
        \param y the initial Y position.
    */
    void SetStart(float x, float y);
    
    /*!
        Set the entity state.
        \param value the entity state.
    */
    void SetState(Uint16 value);
    
    /*!
       Set the entity ID.
        \param value the entity ID.
    */
    void SetId(Uint16 value);
    
    /*!
        Set the entity basepath.
        \param value entity basepath.
    */
    void SetBasepath(string value);
    
    /*!
        Reset the entity to its initiaal position.
    */
    void ResetRect(void);
};

#endif
