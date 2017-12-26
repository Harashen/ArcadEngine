#include <fstream>

#include "script.hpp"
#include "utils.hpp"

Script::~Script (void)
{
    Unload();
}

bool Script::Load(const char *filepath)
{
    ifstream File;
    string   line;
    
    /* Open file */
    File.open(filepath);

    /* File not opened */
    bool ret = File.is_open();
    if (!ret) return false;

    /* Read file */
    while (getline(File, line)) {
        /* Empty line */
        if (!line.size()) continue;

        /* Discard invalid lines */
        if (line[0] == '#' || line[0] == ' ') {
            continue;
        }

        /* Add line */
        mLines.push_back(line);
    }

    /* Close file */
    File.close();

    return true;
}

bool Script::Load(string filepath)
{
    const char *c = filepath.c_str();

    /* Load script */
    return Load(c);
}

void Script::Unload(void)
{
    /* Clear lines */
    mLines.clear();
}

bool Script::GetValue(string key, string &value)
{
    vector<string>::iterator it;

    /* Search line */
    foreach (mLines, it) {
        string line = *it;

        string name;
        size_t length, pos;

        /* Find delimitator */
        pos = line.find("=");

        /* Not found */
        if (pos == string::npos) continue;

        /* Get key */
        name = line.substr(0, pos);
        RemoveSpaces(name);

        /* Check key */
        if (name.compare(key)) continue;

        /* Value length */
        length = line.size() - pos;

        /* Get value */
        value = line.substr(pos + 1, length);
        RemoveSpaces(value);

        return true;
    }

    return false;
}

bool Script::GetBool(const char *key)
{
    string k(key);

    /* Get bool */
    return GetBool(k);
}

bool Script::GetBool(string key)
{
    string val;
    bool   ret;

    /* Get key value */
    ret = GetValue(key, val);
    if (!ret) return false;

    /* Return bool */
    return !val.compare("true");
}

string Script::GetString(const char *key)
{
    string k(key);

    /* Get string */
    return GetString(k);
}

string Script::GetString(string key)
{
    string val;

    /* Get key value */
    GetValue(key, val);

    return val;
}

SDL_Color Script::GetColor(const char *key)
{
    string k(key);
    
    /* Get color*/
    return GetColor(k);
}
    
SDL_Color Script::GetColor(string key)
{    
    SDL_Color color = {0, 0, 0, 0};
    
    color.r = GetValue<Uint16>(key + ".Red");
    color.g = GetValue<Uint16>(key + ".Green");
    color.b = GetValue<Uint16>(key + ".Blue");
    
    return color;
}
