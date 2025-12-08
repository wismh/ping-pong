#include <SDL3/SDL.h>
#include <glad/glad.h>
#include <iostream>

int main() {
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    std::cout << "SDL Error: " << SDL_GetError() << std::endl;
    return -1;
  }

  SDL_Window* win = SDL_CreateWindow("Ping-Pong", 800, 600, SDL_WINDOW_OPENGL);
  SDL_GLContext gl = SDL_GL_CreateContext(win);

  gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);

  bool running = true;
  SDL_Event e;

  while (running) {
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_EVENT_QUIT)
        running = false;
    }

    glClearColor(0.0f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    SDL_GL_SwapWindow(win);
  }

  SDL_DestroyWindow(win);
  SDL_Quit();
}
