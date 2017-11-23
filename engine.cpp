#include <string>

#include <GL/gl.h>
#include <GL/glu.h>
#include <boost/format.hpp>

#include "engine.hpp"
#include "credits.hpp"
#include "font.hpp"
#include "menu.hpp"
#include "timer.hpp"

Engine::Engine(void)
{
    /* Initialize variables */
    mExit     = false;
    mMenuExit = false;
    mPause    = false;
    mWidth    = 0;
    mHeight	  = 0;
    mCredits  = 0;

    /* Create objects */
    mpAi       = new Ai;
    mpAudio    = new Audio;
    mpCamera   = new Camera;
    mpLog  = new Log;
    mpHud      = new Hud;
    mpLevel    = new Level;
    mpKeyboard = new Keyboard;
    mpJoystick = new Joystick;
    mpPlayer   = new Player;
    mpStory    = new Story;
    mpVideo    = new Video;

    /* Active objects */
         mpAi->SetActive();
      mpAudio->SetActive();
     mpCamera->SetActive();
    mpLog->SetActive();
        mpHud->SetActive();
      mpLevel->SetActive();
     mpPlayer->SetActive();
      mpStory->SetActive();
      mpVideo->SetActive();
}

Engine::~Engine(void)
{
    /* Destroy objects */
    if (mpAi)       delete mpAi;
    if (mpAudio)    delete mpAudio;
    if (mpCamera)   delete mpCamera;
    if (mpHud)      delete mpHud;
    if (mpLevel)    delete mpLevel;
    if (mpKeyboard)	delete mpKeyboard;
    if (mpJoystick)	delete mpJoystick;
    if (mpPlayer)	delete mpPlayer;
    if (mpStory)	delete mpStory;
    if (mpVideo)	delete mpVideo;

    /* Destroy log */
    if (mpLog) delete mpLog;
}

bool Engine::Init(void)
{
    bool ret;

    /* Set log mode */
    mpLog->StdOut(true);

    *mpLog << "[ENGINE] Initializing engine..." << endl;

    mBasepath = str(format(".\\%s\\") % "ArcadEngine");

    /* Load settings */
    ret = LoadSettings();
    if (!ret) {
        *mpLog << "[ENGINE] ERROR: Could not load engine settings!" << endl;
        return false;
    }

    /* Initialize video */
    ret = mpVideo->Init();
    if (!ret) {
        *mpLog << "[ENGINE] ERROR: Could not initialize the video subsystem!" << endl;
        return false;
    }

    /* Initialize audio */
    ret = mpAudio->Init();
    if (!ret) {
        *mpLog << "[ENGINE] ERROR: Could not initialize the audio subsystem!" << endl;
        return false;
    }

    /* Initialize HUD */
    ret = mpHud->Init(mBasepath + "HUD\\");
    if (!ret) {
        *mpLog << "[ENGINE] ERROR: Could not initialize the HUD!" << endl;
        return false;
    }

    return true;
}

bool Engine::LoadSettings(void)
{
    Script script;
    bool   ret;

    *mpLog << "[ENGINE] Loading resources..." << endl;

    /* Load script */
    ret = script.Load(mBasepath + "config.ini");
    if (!ret) {
        *mpLog << "[ENGINE] ERROR: Could not load the storyboard!" << endl;
        return false;
    }

    /* Set video config */
    mWidth  = script.GetValue<Uint16>("Width");
    mHeight = script.GetValue<Uint16>("Height");
    
    /* Set ways */
    mWays = script.GetValue<Uint16>("Ways");
    if (!mWays)	mWays = WAY_4;

    /* Set game name */
    mGameName = script.GetString("GameName");

    return true;
}

bool Engine::Configure(void)
{
    bool ret;

    *mpLog << "[ENGINE] Configuring engine..." << endl;

    /* Set video mode */
    ret = mpVideo->Configure(mWidth, mHeight);
    if (!ret) {
        *mpLog << "[ENGINE] ERROR: Could not configure the video subsystem!" << endl;
        return false;
    }

    /* Set video attributes */
    mpVideo->SetCaption(mGameName);

    /* Set audio mode */
    ret = mpAudio->Configure();
    if (!ret) {
        *mpLog << "[ENGINE] ERROR: Could not configure the audio subsystem!" << endl;
        return false;
    }

    /* Set camera target */
    mpCamera->SetTarget(mpPlayer);
    
    /* Set sreen size */
    mpHud->SetScreen(mWidth, mHeight);

    /* Load storyboard */
    mpStory->Load(mBasepath);
    
    /* Load player */
    ret = mpPlayer->Load(mBasepath + "Player\\");
    if (!ret) {
        *mpLog << "[ENGINE] ERROR: Could not load the player!" << endl;
        return false;
    }
        
    return true;
}

void Engine::Events(void)
{
    SDL_Event event;

    /* Poll events */
    while (SDL_PollEvent(&event)) {
        /* Check event */
        switch (event.type) {                
            case SDL_QUIT: /* Exit */
                mExit     = true;
                mMenuExit = true;
                break;

            /* Key down */
            case SDL_KEYDOWN:
                mpKeyboard->Listen(event.key.keysym.sym, true);
                break;

            /* Key up */
            case SDL_KEYUP:
                mpKeyboard->Listen(event.key.keysym.sym, false);
                break;

            /* Button down */		
            case SDL_JOYBUTTONDOWN:
                mpJoystick->Listen(event.jbutton.button, true);
                break;

            /* Button up */  
            case SDL_JOYBUTTONUP:
                mpJoystick->Listen(event.jbutton.button, false);           
                break;

            default:
                break;
        }
    }
}

void Engine::Input_2Horizontal(void)
{       
    /* JOYSTICK */
    if (mpJoystick->IsLoaded()) {
        Sint16 joyx = 99;
        Sint16 joyy = 99;
        
        mpJoystick->GetAxis(joyx, joyy);        
        
        /* Check direction */		
        if (abs(joyx) > 500) {
            if (joyx < -500) {
                mpPlayer->SetMotion(ENTITY_LEFT);
                mpPlayer->Run();
            } else {
                mpPlayer->SetMotion(ENTITY_RIGHT);
                mpPlayer->Run();
            }
        } else {
            mpPlayer->Stop();
        }        
    } else { /* Keyboard */
        /* Check direction */
        if (mpKeyboard->IsPressed(SDLK_LEFT)) {
            mpPlayer->SetMotion(ENTITY_LEFT);
            mpPlayer->Run();
        } else {
            if (mpKeyboard->IsPressed(SDLK_RIGHT)) {
                mpPlayer->SetMotion(ENTITY_RIGHT);
                mpPlayer->Run();
            } else {
                mpPlayer->Stop();
            }
        }
    }
}

void Engine::Input_2Vertical(void)
{    
    /* JOYSTICK */
    if (mpJoystick->IsLoaded()) {
        Sint16 joyx = 99;
        Sint16 joyy = 99;
            
        mpJoystick->GetAxis(joyx, joyy);        
        
        /* Check direction */
        if (abs(joyy) > 500) {
            if (joyy < -500) {
                mpPlayer->SetMotion(ENTITY_UP);
                mpPlayer->Run();
            } else {
                mpPlayer->SetMotion(ENTITY_DOWN);
                mpPlayer->Run();
            }
        } else {
            mpPlayer->Stop();
        }        
    } else {/* Keyboard */
        /* Check direction */
        if (mpKeyboard->IsPressed(SDLK_UP)) {
            mpPlayer->SetMotion(ENTITY_UP);
            mpPlayer->Run();
        } else {
            if (mpKeyboard->IsPressed(SDLK_DOWN)) {
                mpPlayer->SetMotion(ENTITY_DOWN);
                mpPlayer->Run();
            } else {
                mpPlayer->Stop();
            }
        }
    }
}

void Engine::Input_4Ways(void)
{        
    /* JOYSTICK */
    if (mpJoystick->IsLoaded()) {
        Sint16 joyx = 99;
        Sint16 joyy = 99;
            
        mpJoystick->GetAxis(joyx, joyy);        
              
        /* Check direction (testing mode)*/
        if (abs(joyx) < abs(joyy)) {
            if (joyy < -500) {
                mpPlayer->SetMotion(ENTITY_UP);
                mpPlayer->Run();
            }

            if (joyy > 500) {
                mpPlayer->SetMotion(ENTITY_DOWN);
                mpPlayer->Run();
            }
        } else {
            if (abs(joyx) > 500) {
                if (joyx < -500) {
                    mpPlayer->SetMotion(ENTITY_LEFT);
                    mpPlayer->Run();
                } else {
                    mpPlayer->SetMotion(ENTITY_RIGHT);
                    mpPlayer->Run();
                }
            }
        }
        
        if (joyx == 0 && joyy == 0) {
            mpPlayer->Stop();        
        }
    } 
    else /* Keyboard */
    {
        /* Check direction (testing mode)*/
        if (mpKeyboard->IsPressed(SDLK_UP)) {
            mpPlayer->SetMotion(ENTITY_UP);
            mpPlayer->Run();
        } else {
            if (mpKeyboard->IsPressed(SDLK_DOWN)) {
                mpPlayer->SetMotion(ENTITY_DOWN);
                mpPlayer->Run();
            } else {
                if (mpKeyboard->IsPressed(SDLK_LEFT)) {
                    mpPlayer->SetMotion(ENTITY_LEFT);
                    mpPlayer->Run();
                } else {
                    if (mpKeyboard->IsPressed(SDLK_RIGHT)) {
                        mpPlayer->SetMotion(ENTITY_RIGHT);
                        mpPlayer->Run();
                    } else {
                        mpPlayer->Stop();
                    }
                }
            }
        }
    }    
}

void Engine::Run(void)
{
    bool   ret;
    Sint16 gameRet;
    Uint16 cont;

    *mpLog << "[ENGINE] Running..." << endl;
    
    while (!mExit) {
        /* Configure game */
        mCredits = 0;
        gpPlayer->SetLives(0);
        
        /* Show menu */
        ret = ShowMenu();
        if (!ret) return;
        
        /* Start level */
        mpStory->SetCurrent(0);
        mpPlayer->ResetScore();

        *mpLog << "[ENGINE] Main loop running." << endl;
        
        while (!mMenuExit) {
            /* Level loop */
            do {
                *mpLog << "[ENGINE] Load level." << endl;

                /* Load level */
                ret = mpStory->LoadLevel();
                if (!ret) {
                    *mpLog << "[ENGINE] Could not load the level." << endl;
                    return;
                }
                
                /* Reset player position */
                mpPlayer->Reset();

                /* Draw */
                IngameDraw();
                
                /* Render video */
                mpVideo->Flip(true);

                *mpLog << "[ENGINE] Start game." << endl;
                
                /* Start game */
                gameRet = Ingame();
                
                if (gameRet == -1) {
                    mpLevel->Draw(true);
                    
                    /* Show dead message */
                    mpHud->DrawText("You lose a life", 0.85);
                    
                    /* Render video */
                    mpVideo->Flip(false);
                    
                    cont++;  
                    SDL_Delay(1000);
                }
                
                /* Level finished */
                if (gameRet == 1) break;
                
                /* Update player lives */
                if (!mpPlayer->GetLives()) {
                    mpStory->SaveHighScore(mBasepath);
                    mpStory->UnloadLevel();
                    
                    /* Exit to menu */
                    mMenuExit = true;
                    
                    break;
                }
                
                /* Unload level */
                mpStory->UnloadLevel();    
            } while (gameRet == -1);
            
            /* Level finished */
            if (gameRet == 1) {
                /* Clear screen */
                mpVideo->Clear();
                
                /* Victory message */
                mpLevel->Draw(true);
                mpHud->DrawText("VICTORY", 0.85);
                
                /* Render video */
                mpVideo->Flip(false);
                
                SDL_Delay(1500);
                
                /* Delivery points */
                DeliveryPoints();
                
                /* Reset player position */
                mpPlayer->Reset(); 
                
                /* Load next level */
                ret = mpStory->NextLevel();
                
                /* No more levels */
                if (!ret) {
                    /* Save Highscore */
                    mpStory->SaveHighScore(mBasepath);
                  
                    /* Clear screen */
                    mpVideo->Clear();
                    
                    /* Draw message */
                    mpLevel->Draw(true);
                    mpHud->DrawText("Game Over", 0.85);
                    
                    /* Render video */
                    mpVideo->Flip(false);
                    
                    SDL_Delay(1500);
                    
                    /* Show credits */
                    ShowCredits();
                    break;
                }
            }
        }

        *mpLog << "[ENGINE] Main loop finished." << endl;
    }
}

/**
 * Engine menu code
 */
bool Engine::ShowMenu(void)
{
    Menu   menu;
    Sint16 selected = MENU_NOTHING;
    bool   ret;

    Sint16 joyX;
    Sint16 joyY;
    
    /* Load menu */
    ret = menu.Load(mBasepath + "Menu\\");
    if (!ret) return false;

    /* Reset variable */
    if (mMenuExit) mMenuExit = false;

    /* Play menu */
    ret = menu.Play();
    if (!ret) return false;
        
    /* Draw menu */
    ret = menu.Draw();
    if (!ret) return false;

    /* Menu loop */
    while (!mExit) {
        /* Events */
        Events();

        /* JOYSTICK */
        if (mpJoystick->IsLoaded()) {
            mpJoystick->GetAxis(joyX, joyY, true);

            if (abs(joyY) > 500) {
                if (joyY < -500) {
                    menu.Change(MENU_DOWN);
                } else {
                    menu.Change(MENU_UP);
                }
            }
            
            if (mpJoystick->IsPressed(BUTTON_1, true)) {
               selected = menu.Select();
            }
               
            if (mpJoystick->IsPressed(BUTTON_EXIT, true)) {
                mExit     = true;
                mMenuExit = true;
                selected  = MENU_EXIT;
            }
            
            if (mpJoystick->IsPressed(BUTTON_INSERT, true) && mCredits < 99) {
               mCredits++;
            }
        } else { /* Keyboard */ 
            if (mpKeyboard->IsPressed(SDLK_UP, true)) {
                menu.Change(MENU_UP);
            } else if (mpKeyboard->IsPressed(SDLK_DOWN, true)) {
                menu.Change(MENU_DOWN);
            }
                    
            if (mpKeyboard->IsPressed(SDLK_ESCAPE, true)) {
                mMenuExit = true;
                mExit     = true;
                selected  = MENU_EXIT;
            }
                
            if (mpKeyboard->IsPressed(SDLK_RETURN, true)) {
                selected = menu.Select();
            }
            
            if (mpKeyboard->IsPressed(SDLK_c, true) && mCredits < 99) {
                mCredits++;
            }
        }
        
        /* Select option */
        switch(selected) {
            case MENU_NOTHING:
                break;
            
            /* Start game */
            case MENU_PLAY:
                if (mCredits) {
                    menu.Stop();
                    mpStory->SetLives(mCredits);

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
                menu.Stop();
                ShowCredits();
            
                /* Restart menu */
                selected = MENU_NOTHING;
                menu.Play();
                break;
            
            /* Close menu */
            case MENU_EXIT:
                return false;
        }

        /* Clear screen */
        mpVideo->Clear();

        /* Draw menu */
        menu.Draw();
        mpHud->DrawCredits(mCredits);

        /* Render video */
        mpVideo->Flip(false);
    }

    /* Stop menu */
    menu.Stop();
    
    return true;
}

/**
 * Engine ingame code
 */

void Engine::IngameInput(void)
{
    switch (mWays) {
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
    if (mpJoystick->IsLoaded()) {
        if (mpJoystick->IsPressed(BUTTON_1, true)) {
            mpPlayer->Stop();
            mpPlayer->Shot();
        }
        
        if (mpJoystick->IsPressed(BUTTON_PRNT_SCR, true)) {
            mpVideo->SaveScreen(mBasepath + "Screnshots\\");
        }
           
        if (mpJoystick->IsPressed(BUTTON_PAUSE, true)) {
            IngamePause();
        }
            
        if (mpJoystick->IsPressed(BUTTON_INSERT, true) && mCredits < 99) {
            mCredits++;
            mpStory->SetLives();
        }
        
        if (mpJoystick->IsPressed(BUTTON_EXIT, true)) {
            if (mMenuExit) {
                mExit = true;
            } else {
                mMenuExit = true;
            }
        }
    } else { /* Keyboard */ 
        if (mpKeyboard->IsPressed(SDLK_a, true)) {
            mpPlayer->Stop();
            mpPlayer->Shot();
        }
        
        if (mpKeyboard->IsPressed(SDLK_i, true)) {
            mpVideo->SaveScreen(mBasepath + "Screnshots\\");
        }
        
        if (mpKeyboard->IsPressed(SDLK_p, true)) {
            IngamePause();
        }
            
        if (mpKeyboard->IsPressed(SDLK_c, true) && mCredits < 99) {
            mCredits++;
            mpStory->SetLives();
        }
          
        if (mpKeyboard->IsPressed(SDLK_ESCAPE, true)) {
            if (mMenuExit) {
                mExit = true;
            } else {
                mMenuExit = true;
            }
        }
    }
}

void Engine::IngameUpdate(void)
{
    /* Update player */
    mpPlayer->Update();
    
    /* Update AI */
    mpAi->Update();

    /* Update level */
    mpLevel->Update();
}

void Engine::IngameDraw(void)
{
    /* Clear screen */
    mpVideo->Clear();

    /* Set camera */
    mpCamera->Look();

    /* Draw level */
    mpLevel->Draw();
    
    /* Draw player */
    mpPlayer->Draw();

    /* Draw HUD */
    mpHud->DrawFps();
    mpHud->DrawScore();
    mpHud->DrawHighScore();
    mpHud->DrawLives();
}

void Engine::IngamePause(void)
{
    /* Pause loop */
    while (!mExit)  {
        /* Events */
        Events();
        
        /* Joystick */
        if (mpJoystick->IsLoaded()) {			   
            if (mpJoystick->IsPressed(BUTTON_PAUSE, true)) {
                break;
            }
        
            if (mpJoystick->IsPressed(BUTTON_EXIT, true)) {
                mMenuExit = true;
                break;
            }
        } else {
            if (mpKeyboard->IsPressed(SDLK_p, true)) {
                break;
            }
                
            if (mpKeyboard->IsPressed(SDLK_ESCAPE, true)) {
                mMenuExit = true;
                break;
            }
        }

        /* Draw */
        IngameDraw();
        
        /* Draw pause */
        mpHud->DrawText("PAUSE", 0.5);

        mpVideo->Flip(false);
    }
}

Sint16 Engine::Ingame(void)
{
    /* In-game loop */
    while (!mMenuExit && !mpLevel->GetFinished(mpPlayer->GetLevelScore())) {
        /* Events */
        Events();

        /* Input */
        IngameInput();

        /* Update */
        IngameUpdate();

        /* Draw */
        IngameDraw();

        /* Render */
        mpVideo->Flip(true);
        
        if (mpPlayer->GetState() & ENTITY_DEAD) {
            return -1;
        }
    }

    /* Exit actual game */
    if (mMenuExit) return 0;

    /* Update */
    IngameUpdate();

    /* Draw */
    IngameDraw();
    
    /* Render */
    mpVideo->Flip(true);
        
    return 1;
}


/**
 * Engine credits code
 */

void Engine::ShowCredits(void)
{
    Credits credits;
    bool    ret;
    bool    end = false;

    /* Load credits */
    ret = credits.Load(mBasepath + "Credits\\");
    if (!ret) return;
        
    /* Set screen */
    credits.SetScreen(mWidth, mHeight);

    /* Play credits */
    ret = credits.Play();
    if (!ret) return;

    /* Credits loop */
    while (!mExit && !end) {
        /* Events */
        Events();

        /* JOYSTICK */
        if (mpJoystick->IsLoaded()) {
            Uint32 buttons = mpJoystick->GetButtonsNumber();

            for (Uint32 i = 0; i < buttons; i++) {
                if (mpJoystick->IsPressed(i, true)) {
                    end = true;
                    break;
                }
            }
        } else { /* Keyboard */        
            for (Uint32 i = SDLK_FIRST; i < SDLK_LAST; i++) {
                if (mpKeyboard->IsPressed(i, true)) {
                    end = true;
                    break;
                }
            }
        }

        /* Clear screen */
        mpVideo->Clear();

        /* Draw credits */
        ret = credits.Draw();
        if (!ret) break;

        /* Render video */
        mpVideo->Flip(false);
    }

    /* Stop credits */
    credits.Stop();
}

void Engine::DeliveryPoints(void)
{
    bool   end   = false;
    Uint32 score = mpPlayer->GetLives() * mpStory->GetPoints();
    
    /* Set score */
    mpPlayer->SetScore(score);
    
    /* Clear screen */
    mpVideo->Clear();
    
    /* Draw */
    mpLevel->Draw(true);
    mpHud->DrawPoints(0.85);
    mpHud->DrawPress();
    
    /* Render video */
    mpVideo->Flip(false);
    
    /* Delivery points loop */
    while (!mExit && !end) {
        /* Events */
        Events();

        /* JOYSTICK */
        if (mpJoystick->IsLoaded()) {
            Uint32 buttons = mpJoystick->GetButtonsNumber();

            for (Uint32 i = 0; i < buttons; i++) {
                if (mpJoystick->IsPressed(i, true)) {
                    end = true;
                    break;
                }
            }
        } else { /* Keyboard */
            for (Uint32 i = SDLK_FIRST; i < SDLK_LAST; i++) {
                if (mpKeyboard->IsPressed(i, true)) {
                    end = true;
                    break;
                }
            }
        }
    }
}