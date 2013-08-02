#include <fstream>
#include "script.h"
#include "utils.h"

CScript::~CScript (void)
{
    Unload();
}

bool CScript::Load(const char *filepath)
{
	ifstream File;
	string   line;
	
	/* Open file */
	File.open(filepath);

	/* File not opened */
	bool ret = File.is_open();
	if (!ret)
		return false;

	/* Read file */
	while (getline(File, line)) {
		/* Empty line */
		if (!line.size())
			continue;

		/* Discard invalid lines */
		if (line[0] == '#' || line[0] == ' ')
			continue;

		/* Add line */
		Lines.push_back(line);
	}

	/* Close file */
	File.close();

	return true;
}

bool CScript::Load(string filepath)
{
	const char *c = filepath.c_str();

	/* Load script */
	return Load(c);
}

void CScript::Unload(void)
{
	/* Clear lines */
	Lines.clear();
}

bool CScript::GetValue(string key, string &value)
{
	vector<string>::iterator it;

	/* Search line */
	foreach (Lines, it) {
		string line = *it;

		string name;
		size_t length, pos;

		/* Find delimitator */
		pos = line.find("=");

		/* Not found */
		if (pos == string::npos)
			continue;

		/* Get key */
		name = line.substr(0, pos);
		RemoveSpaces(name);

		/* Check key */
		if (name.compare(key))
			continue;

		/* Value length */
		length = line.size() - pos;

		/* Get value */
		value = line.substr(pos + 1, length);
		RemoveSpaces(value);

		return true;
	}

	return false;
}

bool CScript::GetBool(const char *key)
{
	string k(key);

	/* Get bool */
	return GetBool(k);
}

bool CScript::GetBool(string key)
{
	string val;
	bool   ret;

	/* Get key value */
	ret = GetValue(key, val);
	if (!ret)
		return false;

	/* Return bool */
	return !val.compare("true");
}

string CScript::GetString(const char *key)
{
	string k(key);

	/* Get string */
	return GetString(k);
}

string CScript::GetString(string key)
{
	string val;

	/* Get key value */
	GetValue(key, val);

	return val;
}

SDL_Color CScript::GetColor(const char *key)
{
    string k(key);
    
	/* Get color*/
    return GetColor(k);
}
    
SDL_Color CScript::GetColor (string key)
{    
    SDL_Color Color;
	
	/* Default color */
	Color.r = Color.g = Color.b = 0;
    
    Color.r = GetValue<Uint16>(key + ".Red");
    Color.g = GetValue<Uint16>(key + ".Green");
    Color.b = GetValue<Uint16>(key + ".Blue");
    
    return Color;
}
