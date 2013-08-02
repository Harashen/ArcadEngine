#include <GL/gl.h>
#include <GL/glu.h>
#include <boost/format.hpp>
#include <string>

#include "credits.h"
#include "engine.h"
#include "font.h"
#include "menu.h"
#include "timer.h"

CEngine::CEngine(void)
{
	/* Initialize variables */
	Exit     = false;
	MenuExit = false;
	Pause    = false;
	Width	 = 0;
	Height	 = 0;
	Credits	 = 0;

	/* Create objects */
	Ai       = new CAi;
	Audio    = new CAudio;
	Camera   = new CCamera;
	Hud      = new CHud;
	Level    = new CLevel;
	Keyboard = new CKeyboard;
	Joystick = new CJoystick;
	Player   = new CPlayer;
	Story    = new CStory;
	Video    = new CVideo;

	/* Active objects */
	     Ai->SetActive();
	  Audio->SetActive();
	 Camera->SetActive();
	    Hud->SetActive();
	  Level->SetActive();
	 Player->SetActive();
	  Story->SetActive();
	  Video->SetActive();
}

CEngine::~CEngine(void)
{
	/* Destroy objects */
	if (Ai)
		delete Ai;
	if (Audio)
		delete Audio;
	if (Camera)
		delete Camera;
	if (Hud)
		delete Hud;
	if (Level)
		delete Level;
	if (Keyboard)
		delete Keyboard;
	if (Joystick)
		delete Joystick;
	if (Player)
		delete Player;
	if (Story)
		delete Story;
	if (Video)
		delete Video;
}

bool CEngine::Init(void)
{
	bool ret;

	Basepath = str(format(".\\%s\\") % "ArcadEngine");

	/* Load settings */
	ret = LoadSettings();
	if (!ret) 
		return false;

	/* Initialize video */
	ret = Video->Init();
	if (!ret)
		return false;

	/* Initialize audio */
	ret = Audio->Init();
	if (!ret) 
		return false;

	/* Initialize HUD */
	ret = Hud->Init(Basepath + "HUD\\");
	if (!ret)
		return false;

	return true;
}

bool CEngine::LoadSettings(void)
{
	CScript Script;
	bool    ret;

	/* Load script */
	ret = Script.Load(Basepath + "config.ini");
	if (!ret)
		return false;

	/* Set video config */
	Width  = Script.GetValue<Uint16>("Width");
	Height = Script.GetValue<Uint16>("Height");
	
	/* Set ways */
	Ways = Script.GetValue<Uint16>("Ways");
	if (!Ways)
		Ways = WAY_4;

	/* Set game name */
	GameName = Script.GetString("GameName");

	return true;
}

bool CEngine::Configure(void)
{
	bool ret;

	/* Set video mode */
	ret = Video->Configure(Width, Height);
	if (!ret)
		return false;

	/* Set video attributes */
	Video->SetCaption(GameName);

	/* Set audio mode */
	ret = Audio->Configure();
	if (!ret)
		return false;

	/* Set camera target */
	Camera->SetTarget(pPlayer);
	
	/* Set sreen size */
	Hud->SetScreen(Width, Height);

	/* Load storyboard */
	Story->Load(Basepath);
	
	/* Load player */
	ret = Player->Load(Basepath + "Player\\");
	if (!ret)
		return false;
		
	return true;
}

void CEngine::Events(void)
{
	SDL_Event Event;

	/* Poll events */
	while (SDL_PollEvent(&Event)) {
		/* Check event */
		switch (Event.type) {
		/* Exit */
		case SDL_QUIT:
			Exit     = true;
			MenuExit = true;
			break;

		/* Key down */
		case SDL_KEYDOWN:
			Keyboard->Listen(Event.key.keysym.sym, true);
			break;

		/* Key up */
		case SDL_KEYUP:
			Keyboard->Listen(Event.key.keysym.sym, false);
			break;
		
		/* Button down */		
		case SDL_JOYBUTTONDOWN:
                Joystick->Listen(Event.jbutton.button, true);
                break;
        
		/* Button up */  
		case SDL_JOYBUTTONUP:
			Joystick->Listen(Event.jbutton.button, false);           
			break;
		

		default:
			break;
		}
	}
}

void CEngine::Input_2Horizontal(void)
{       
    /* JOYSTICK */
    if (Joystick->IsLoaded()) {
		Sint16 joyx = 99;
		Sint16 joyy = 99;
        
        Joystick->GetAxis(joyx, joyy);        
        
		/* Check direction */		
        if (abs(joyx) > 500) {
        	if (joyx < -500) {
        	    Player->SetMotion(ENTITY_LEFT);
                Player->Run();
            }
            else {
        	    Player->SetMotion(ENTITY_RIGHT);
                Player->Run();
            }
        }
        else
            Player->Stop();
        
    }
    
    /* Keyboard */
    else {
		/* Check direction */
        if (Keyboard->IsPressed(SDLK_LEFT)) {
            Player->SetMotion(ENTITY_LEFT);
            Player->Run();
        }
        else
            if (Keyboard->IsPressed(SDLK_RIGHT)) {
                Player->SetMotion(ENTITY_RIGHT);
                Player->Run();
            }
            else
                Player->Stop();
    }
}

void CEngine::Input_2Vertical(void)
{    
    /* JOYSTICK */
    if (Joystick->IsLoaded()) {
		Sint16 joyx = 99;
		Sint16 joyy = 99;
            
        Joystick->GetAxis(joyx, joyy);        
        
		/* Check direction */
        if (abs(joyy) > 500) {
        	if (joyy < -500) {
        	    Player->SetMotion(ENTITY_UP);
                Player->Run();
            }
            else {
        	    Player->SetMotion(ENTITY_DOWN);
                Player->Run();
            }
        }
        else
            Player->Stop();
        
    }
    
	/* Keyboard */
    else {
		/* Check direction */
        if (Keyboard->IsPressed(SDLK_UP)) {
            Player->SetMotion(ENTITY_UP);
            Player->Run();
        }
        else
            if (Keyboard->IsPressed(SDLK_DOWN)) {
                Player->SetMotion(ENTITY_DOWN);
                Player->Run();
            }
            else
                Player->Stop();
    }
}

void CEngine::Input_4Ways(void)
{        
    /* JOYSTICK */
    if (Joystick->IsLoaded()) {
		Sint16 joyx = 99;
		Sint16 joyy = 99;
            
        Joystick->GetAxis(joyx, joyy);        
              
        /* Check direction (testing mode)*/
        if (abs(joyx) < abs(joyy)) {
            if (joyy < -500) {
                Player->SetMotion(ENTITY_UP);
                Player->Run();
            }
        	if (joyy > 500) {
                Player->SetMotion(ENTITY_DOWN);
                Player->Run();
            }
        }
        else {
            if (abs(joyx) > 500) {
            	if (joyx < -500) {
            	    Player->SetMotion(ENTITY_LEFT);
                    Player->Run();
                }
                else {
            	    Player->SetMotion(ENTITY_RIGHT);
                    Player->Run();
                }
            }
        }
        
        if (joyx == 0 && joyy == 0)
            Player->Stop();
        
    }
   
	/* Keyboard */
    else {   
		/* Check direction (testing mode)*/
        if (Keyboard->IsPressed(SDLK_UP)) {
            Player->SetMotion(ENTITY_UP);
            Player->Run();
        }
        else {
            if (Keyboard->IsPressed(SDLK_DOWN)) {
                Player->SetMotion(ENTITY_DOWN);
                Player->Run();
            }
            else {
                if (Keyboard->IsPressed(SDLK_LEFT)) {
                    Player->SetMotion(ENTITY_LEFT);
                    Player->Run();
                }
                else {
                    if (Keyboard->IsPressed(SDLK_RIGHT)) {
                        Player->SetMotion(ENTITY_RIGHT);
                        Player->Run();
                    }
                    else
                        Player->Stop();
                }
            }
        }
    }    
}

void CEngine::Run(void)
{
	bool   ret;
    Sint16 gameRet;
    Uint16 cont;    
    
    while (!Exit) {
		/* Configure game */
        Credits = 0;
        pPlayer->SetLives(0);
		
		/* Show menu */
        ret = Menu();
        if (!ret)
            return;
        
		/* Start level */
        Story->SetCurrent(0);
        Player->ResetScore();
		
        while (!MenuExit) {
			/* Level loop */
            do{
				/* Load level */
                ret = Story->LoadLevel();
                if (!ret)
                    return;
                
				/* Reset player position */
                Player->Reset();

				/* Draw */
				IngameDraw();
				
				/* Render video */
				Video->Flip(true);
				
				/* Start game */
                gameRet = Ingame();
				
                if (gameRet == -1) {
                    Level->Draw(true);
					
					/* Show dead message */
                    Hud->DrawText("You lose a life", 0.85);
					
					/* Render video */
                    Video->Flip(false);
                    
					cont++;  
                    SDL_Delay (1000);
                }
                
				/* Level finished */
                if (gameRet == 1)
                    break;
                
				/* Update player lives */
                if (!Player->GetLives() ) {
                    Story->SaveHighScore(Basepath);
                    Story->UnloadLevel();
					
					/* Exit to menu */
                    MenuExit = true;
					
                    break;
                }
                
				/* Unload level */
                Story->UnloadLevel();    
            }while (gameRet == -1);
			
			/* Level finished */
            if (gameRet == 1){
				/* Clear screen */
                Video->Clear();
				
				/* Victory message */
                Level->Draw(true);
                Hud->DrawText("VICTORY", 0.85);
				
				/* Render video */
                Video->Flip(false);
				
                SDL_Delay (1500);
                
                /* Delivery points */
                DeliveryPoints();
                
				/* Reset player position */
                Player->Reset(); 
                
				/* Load next level */
                ret = Story->NextLevel();
				
				/* No more levels */
                if (!ret) {
					/* Save Highscore */
                    Story->SaveHighScore(Basepath);
                  
					/* Clear screen */
                    Video->Clear();
					
					/* Draw message */
                    Level->Draw(true);
                    Hud->DrawText("Game Over", 0.85);
					
					/* Render video */
                    Video->Flip(false);
					
                    SDL_Delay (1500);
					
					/* Show credits */
                    ShowCredits();
                    break;
                }
            }
        }
    }
}


/**
 * Engine menu code
 */

bool CEngine::Menu(void)
{
	CMenu  Menu;
	Sint16 selected = MENU_NOTHING;
	bool   ret;

	Sint16 joyx, joyy;
	
	/* Load menu */
	ret = Menu.Load(Basepath + "Menu\\");
	if (!ret)
		return false;

	/* Reset variable */
	if (MenuExit)
		MenuExit = false;

	/* Play menu */
	ret = Menu.Play();
	if (!ret)
		return false;
		
	/* Draw menu */
	ret = Menu.Draw();
	if (!ret)
		return false;

	/* Menu loop */
	while (!Exit) {
		/* Events */
		Events();

		/* JOYSTICK */
		if (Joystick->IsLoaded()) {
            Joystick->GetAxis(joyx, joyy, true);
            if (abs(joyy) > 500) {
            	if (joyy < -500)
            	    Menu.Change(MENU_DOWN);
                else
            	    Menu.Change(MENU_UP);
            }
            
            if (Joystick->IsPressed(BUTTON_1, true))
        	   selected = Menu.Select();
        	   
        	if (Joystick->IsPressed(BUTTON_EXIT, true)) {
                Exit     = true;
                MenuExit = true;
                selected = MENU_EXIT;
            }
            
            if (Joystick->IsPressed(BUTTON_INSERT, true))
                if (Credits < 99)
                    Credits++;
                
        }
		
		/* Keyboard */
        else {
            if (Keyboard->IsPressed(SDLK_UP, true))
                Menu.Change(MENU_UP);
            else
                if (Keyboard->IsPressed(SDLK_DOWN, true))
                    Menu.Change(MENU_DOWN);
                    
            if (Keyboard->IsPressed(SDLK_ESCAPE, true)) {
                MenuExit = true;
                Exit     = true;
                selected = MENU_EXIT;
            }
                
            if (Keyboard->IsPressed(SDLK_RETURN, true)) 
                selected = Menu.Select();
            
            if (Keyboard->IsPressed(SDLK_c, true))
				if (Credits < 99)
					Credits++;        
        }
        
		/* Select option */
        switch(selected) {
		case MENU_NOTHING:
			break;
			
		/* Start game */
		case MENU_PLAY:
			if (Credits) {
				Menu.Stop();
				Story->SetLives(Credits);
				return true;
			}
			
			/* No credits */
			selected = MENU_NOTHING;
			break;
		
		/* Options menu */
		case MENU_OPTIONS:
			selected = MENU_NOTHING;
			break;
		
		/* Show credits */
		case MENU_CREDITS:
			Menu.Stop();
			ShowCredits();
			
			/* Restart menu */
			selected = MENU_NOTHING;
			Menu.Play();
			break;
			
		/* Close menu */
		case MENU_EXIT:
			return false;
        }

		/* Clear screen */
		Video->Clear();

		/* Draw menu */
		Menu.Draw();
		Hud->DrawCredits(Credits);

		/* Render video */
		Video->Flip(false);
	}

	/* Stop menu */
	Menu.Stop();
	
	return true;
}

/**
 * Engine ingame code
 */

void CEngine::IngameInput(void)
{
    switch (Ways) {
        case WAY_2_HORIZONTAL:
            Input_2Horizontal();
            break;
        case WAY_2_VERTICAL:
            Input_2Vertical();
            break;
        case WAY_4:
            Input_4Ways();
            break;
        //case WAY_8:
            //Input_8Ways();
            //break;
        default:
            Input_4Ways();
    }
    
	/* Joystick */
    if (Joystick->IsLoaded()) {
        if (Joystick->IsPressed(BUTTON_1, true)) {
            Player->Stop();
    	    Player->Shot();
        }
        
        if (Joystick->IsPressed(BUTTON_PRNT_SCR, true))
    	    Video->SaveScreen(Basepath + "Screnshots\\");
    	   
    	if (Joystick->IsPressed(BUTTON_PAUSE, true))
    	    IngamePause();
    	    
    	if (Joystick->IsPressed(BUTTON_INSERT, true)) {
            if (Credits < 99) {
                Credits++;
                Story->SetLives();
            }
        }
    	
    	if (Joystick->IsPressed(BUTTON_EXIT, true)){
            if (MenuExit)
                Exit = true;
            else
                MenuExit = true;
        }
    }
	/* Keyboard */
    else {
        if (Keyboard->IsPressed(SDLK_a, true)) {
            Player->Stop();
            Player->Shot();
        }
        
        if (Keyboard->IsPressed(SDLK_i, true))
            Video->SaveScreen(Basepath + "Screnshots\\");
        
        if (Keyboard->IsPressed(SDLK_p, true))
            IngamePause();
            
        if (Keyboard->IsPressed(SDLK_c, true)) {
            if (Credits < 99) {
                Credits++;
                Story->SetLives();
            }
        }
          
        if (Keyboard->IsPressed(SDLK_ESCAPE, true)) {
                if (MenuExit)
                    Exit = true;
                else
                    MenuExit = true;
        }
    }
}

void CEngine::IngameUpdate(void)
{
	/* Update player */
	Player->Update();
	
	/* Update AI */
	Ai->Update();

	/* Update level */
	Level->Update();
}

void CEngine::IngameDraw(void)
{
	/* Clear screen */
	Video->Clear();

	/* Set camera */
	Camera->Look();

	/* Draw level */
	Level->Draw();
	
	/* Draw player */
	Player->Draw();

	/* Draw HUD */
	Hud->DrawFps();
	Hud->DrawScore();
	Hud->DrawHighScore();
	Hud->DrawLives();
}

void CEngine::IngamePause(void)
{
	/* Pause loop */
	while (!Exit) {
		/* Events */
		Events();
		
		/* Joystick */
		if (Joystick->IsLoaded() ) {			   
			if (Joystick->IsPressed(BUTTON_PAUSE, true))
				break;
    	
			if (Joystick->IsPressed(BUTTON_EXIT, true)) {
				MenuExit = true;
				break;
			}
		}
		else {
			if (Keyboard->IsPressed(SDLK_p, true))
				break;
				
			if (Keyboard->IsPressed(SDLK_ESCAPE, true)) {
				MenuExit = true;
				break;
			}
		}

		/* Draw */
		IngameDraw();
		
		/* Draw pause */
		Hud->DrawText("PAUSE", 0.5);

		Video->Flip(false);
	}
}

Sint16 CEngine::Ingame(void)
{
	/* In-game loop */
	while (!MenuExit && !Level->GetFinished(Player->GetLevelScore())) {
		/* Events */
		Events();

		/* Input */
		IngameInput();

		/* Update */
		IngameUpdate();

		/* Draw */
		IngameDraw();

		/* Render */
		Video->Flip(true);
		
		if (Player->GetState() & ENTITY_DEAD)
			return -1;
	}

	/* Exit actual game */
	if (MenuExit)
		return 0;

	/* Update */
	IngameUpdate();

	/* Draw */
	IngameDraw();
	
	/* Render */
	Video->Flip(true);
		
	return 1;
}


/**
 * Engine credits code
 */

void CEngine::ShowCredits(void)
{
	CCredits Credits;
	bool     ret;
	bool 	 end = false;

	/* Load credits */
	ret = Credits.Load(Basepath + "Credits\\");
	if (!ret)
		return;
		
	/* Set screen */
	Credits.SetScreen(Width, Height);

	/* Play credits */
	ret = Credits.Play();
	if (!ret)
		return;
		
	Uint32 i;

	/* Credits loop */
	while (!Exit && !end) {
		/* Events */
		Events();

		/* JOYSTICK */
		if (Joystick->IsLoaded()) {
			Uint32 buttons = Joystick->GetButtonsNumber();
			for (i = 0; i < buttons; i++)
				if (Joystick->IsPressed(i, true)) {
					end = true;
					break;
				}
        }
		
		/* Keyboard */
        else {
			for (i = SDLK_FIRST; i < SDLK_LAST; i++)
				if (Keyboard->IsPressed(i, true)) {
					end = true;
					break;
				}
        }

		/* Clear screen */
		Video->Clear();

		/* Draw credits */
		ret = Credits.Draw();
		if (!ret)
			break;

		/* Render video */
		Video->Flip(false);
	}

	/* Stop credits */
	Credits.Stop();
}

void CEngine::DeliveryPoints(void)
{
    bool   end   = false;
	Uint32 score = Player->GetLives() * Story->GetPoints();
	
	/* Set score */
    Player->SetScore(score);
    
    /* Clear screen */
	Video->Clear();
	
	/* Draw */
    Level->Draw(true);
    Hud->DrawPoints(0.85);
    Hud->DrawPress();
	
	/* Render video */
    Video->Flip(false);
	
	Uint32 i;
    
    /* Delivery points loop */
	while (!Exit && !end) {
		/* Events */
		Events();

		/* JOYSTICK */
		if (Joystick->IsLoaded()) {
			Uint32 buttons = Joystick->GetButtonsNumber();
			for (i = 0; i < buttons; i++)
				if (Joystick->IsPressed(i, true)) {
					end = true;
					break;
				}
        }
		
		/* Keyboard */
        else {
			for (i = SDLK_FIRST; i < SDLK_LAST; i++)
				if (Keyboard->IsPressed(i, true)) {
					end = true;
					break;
				}
        }
	}
}