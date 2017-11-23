#ifndef _SCRIPT_HPP_
#define _SCRIPT_HPP_

#include <string>
#include <vector>

#include <boost/lexical_cast.hpp>
#include <SDL/SDL.h>

using namespace boost;
using namespace std;


/*!
    Script is the class that implements the parser
    to load the engine scripts.
    \author Javier Bermejo García
*/
class Script {
    vector<string> mLines;	//!< Script lines buffer.

private:
    /*!
        Get the value of a key.
        \param key the key name.
        \param value the string object to store the value.
        \return true if success, false otherwise.
    */
    bool GetValue(string key, string &value);

public:
    /*
        Script destructor.
    */
    ~Script(void);

    /*!
        Load a script file.
        \param filepath the script filepath.
        \return true if success, false otherwise.
    */
    bool Load(const char *filepath);

    /*!
        Load a script file.
        \param filepath the script filepath.
        \return true if success, false otherwise.
    */
    bool Load(string filepath);

    /*!
        Unload the script.
    */
    void Unload(void);

    /*!
        Get the boolean value of a key.
        \param key the key name.
        \return the boolean value (or false if the key is not found).
    */
    bool GetBool(const char *key);

    /*!
        Get the boolean value of a key.
        \param key the key name.
        \return the boolean value (or false if the key is not found).
    */
    bool GetBool(string key);

    /*!
        Get the string value of a key.
        \param key the key name.
        \return the string value (or an empty string if the key is not found).
    */
    string GetString(const char *key);

    /*!
        Get the string value of a key.
        \param key the key name.
        \return the string value (or an empty string if the key is not found).
    */
    string GetString(string key);
    
    /*!
        Get the SDL_Color value of a key.
        \param key the key name.
        \return the SDL_Color value (or black color if the key is not found).
    */
    SDL_Color GetColor(const char *key);

    /*!
        Get the SDL_Color value of a key.
        \param key the key name.
        \return the SDL_Color value (or black color if the key is not found).
    */
    SDL_Color GetColor(string key);

    /*!
        Get the value of a key in the specified format.
        \param key the key name.
        \return the value in the specified format (or zero if the key is not found).
    */
    template <class T>
    T GetValue(string key)
    {
        string val;
        bool   ret;

        /* Get value */
        ret = GetValue(key, val);
        if (!ret)
            return T(0);

        /* Return value */
        return lexical_cast<T>(val);
    }
};

#endif
