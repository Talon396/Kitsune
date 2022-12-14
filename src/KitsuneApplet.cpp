#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

namespace Kitsune::Applet {
    static SDL_Window* appletWindow = nullptr;
    static SDL_Surface* appletSurface = nullptr;
    static SDL_Renderer* appletRenderer = nullptr;
    static SDL_Texture* appletFont = nullptr;
    std::string GetExecutableParentDirectory() {
        lua_State* L = luaL_newstate();
        luaL_openlibs(L);
        lua_pushstring(L, executablePath);
        lua_setglobal(L, "EXEC_FILE");
        (void)luaL_dostring(L,"return EXEC_FILE:match(\"^(.+)[/\\\\].*$\")");
        std::string path = std::string(lua_tostring(L, -1));
        lua_close(L);
        return path;
    }
    void SetWindowIcon() {
        SDL_Surface *surf = IMG_Load((GetExecutableParentDirectory()+"/data/Assets/Kitsune.svg").c_str());
        SDL_SetWindowIcon(appletWindow, surf);
        SDL_FreeSurface(surf);
    }
    void Initialize() {
        SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
        SDL_EnableScreenSaver();
        SDL_SetHint(SDL_HINT_FRAMEBUFFER_ACCELERATION, "1");
        #if SDL_VERSION_ATLEAST(2, 0, 5)
            SDL_SetHint(SDL_HINT_MOUSE_FOCUS_CLICKTHROUGH, "1");
        #endif
        SDL_EventState(SDL_DROPFILE, SDL_ENABLE);
        atexit(SDL_Quit);
        SDL_DisplayMode dm;
        SDL_GetCurrentDisplayMode(0, &dm);
        appletWindow = SDL_CreateWindow("Kitsune", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_HIDDEN);
        SetWindowIcon();
        appletRenderer = SDL_CreateRenderer(appletWindow, -1, SDL_RENDERER_ACCELERATED);
        appletSurface = SDL_GetWindowSurface(appletWindow);
        std::string parent = GetExecutableParentDirectory();
        appletFont = IMG_LoadTexture(appletRenderer, (parent+"/data/Assets/ZapLightFont.png").c_str());
        SDL_SetTextureScaleMode(appletFont,SDL_ScaleModeNearest);
    }
    void Show() {
        SDL_ShowWindow(appletWindow);
    }
};