#ifndef _STORYBOARD_H_
#define _STORYBOARD_H_

#include <vector>

using namespace std;


/*!
	CStory is the class that manages the storyboard
	of the game (level list, level loading...).
	\author Javier Bermejo García
*/
class CStory {
	vector<string> List;	//!< Level list array.

	Uint32 Levels;		//!< Number of levels.
	Uint32 Current;		//!< Current level.
	
	Uint32 Lives;		//!< Player lives.
	Uint32 Points;		//!< Points per live.
	Uint32 Score;		//!< Highscore in the game.
	
private:
	/*!
		Load the highscore.
		\param path highscore file path.
		\return true if success, false otherwise.
	*/
	bool LoadHighScore(string path);

public:
	/*!
		CStory constructor.
	*/
	CStory(void);
	
	/*!
		CStory destructor.
	*/
	~CStory(void);

	/*!
		Set the active storyboard object.
	*/
	void SetActive(void);

	/*!
		Load the storyboard.
		\param path setting file path.
		\return true if success, false otherwise.
	*/
	bool Load(string path);
	
	/*!
		Save the highscore.
		\param path highscore file path.
		\return true if success, false otherwise.
	*/
	bool SaveHighScore(string path);

	/*!
		Set the previous level as the current level.
		\return true if success, false otherwise.
	*/
	bool PrevLevel(void);

	/*!
		Set the next level as the current level.
		\return true if success, false otherwise.
	*/
	bool NextLevel(void);

	/*!
		Check if the storyboard has ended.
		\return true if ended, false otherwise.
	*/
	bool End(void);

	/*!
		Load the current level.
		\return true if success, false otherwise.
	*/
	bool LoadLevel(void);

	/*!
		Unload the current level.
	*/
	void UnloadLevel(void);
	
	/*!
        Set the number of levels in the game.
        \param value the number of levels.
    */
    void SetLevels(Uint32 value);
    
    /*!
        Set the current level.
        \param value current level.
    */
    void SetCurrent(Uint32 value);
    
    /*!
        Set the lives of the player, according to the current credits.
        \param credits number of available credits.
    */
    void SetLives(Uint16 credits = 1);
    
    /*!
        Get the number of levels in the game.
        \return the number of levels.
    */
    Uint32 GetLevels(void);
    
    /*!
        Get the current level.
        \return the current level.
    */
    Uint32 GetCurrent(void);
    
    /*!
        Get the number of points per life.
        \return the number of points.
    */
    Uint32 GetPoints(void);
    
    /*!
        Get the highscore.
        \return the highscore.
    */
    Uint32 GetHighscore(void);
};

//! Active storyboard object.
extern CStory *pStory;

#endif

