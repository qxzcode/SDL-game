#include <SDL.h>
#include <iostream>
using std::cout;
using std::cerr;
using std::endl;
using std::cin;

#include "util/SDL_util.h"
#include "util/util.h"

#include "game/World.h"
#include "game/Player.h"
#include "game/Wall.h"

const std::string WINDOW_TITLE = "[ G A M E ]";

int main(int, char*[]) {
    cout << "Program started." << endl;
    
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        cerr << "SDL_Init Error: " << SDL_GetError() << endl;
        return 1;
    }
    
    SDL_Window *win = SDL_CreateWindow(WINDOW_TITLE.c_str(), 100, 100, 640, 480, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (win == nullptr) {
        cerr << "SDL_CreateWindow Error: " << SDL_GetError() << endl;
        SDL_Quit();
        return 1;
    }
    
    SDL_Renderer *ren = util::renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (ren == nullptr) {
        SDL_DestroyWindow(win);
        cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << endl;
        SDL_Quit();
        return 1;
    }
    
    game::World world;
    world.addEntity(std::make_unique<game::Player>(300, 200));
    for (int n = 0; n < 140; n++) {
        world.addEnvEntity(std::make_unique<game::Wall>(util::rand()*2000, util::rand()*2000));
    }
    
    int fpsCount = 0;
    double nextFPSUpdate = 1.0;
    while (!SDL_QuitRequested()) {
        // get & handle user input and such
        util::processEvents();
        
        // update stuff
        world.update(util::getDeltaTime());//SDL_Delay(100);
        
        // clear the renderer
        util::setColor(0, 0, 0);
        SDL_RenderClear(ren);
        
        // draw stuff
        world.draw();
        
        // update the screen
        SDL_RenderPresent(ren);
        
        fpsCount++;
        if (util::getTotalTime() >= nextFPSUpdate) {
            std::string newTitle = WINDOW_TITLE + " (";
            newTitle += std::to_string(fpsCount);
            newTitle += " FPS)";
            SDL_SetWindowTitle(win, newTitle.c_str());
            fpsCount = 0;
            nextFPSUpdate += 1.0;
        }
    }
    
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}