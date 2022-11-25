#include <SDL2/SDL_events.h>
#include <SDL2/SDL_log.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <stdint.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <stdlib.h>
#include <time.h>

#define WIDTH 400
#define HEIGHT 300

typedef struct{
    float x;
    float y;
    int w;
    int h;
    float speed;
    int score;
} player;

typedef struct{
    float x;
    float y; 
    float x_s;
    float y_s;
    float s;
} ball;

void draw_number(SDL_Renderer *renderer, int n, int x, int y);

int main(int argc, char **argv)
{
    int game_running = 1;

    // Initialization
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("Pong", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_OPENGL);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    uint32_t next_game_tick = SDL_GetTicks();
    player p1 = {30, 50, 20, 75, 0, 0};
    player p2 = {350, 50, 20, 75, 0, 0};
    ball b = {WIDTH/2.0-5, HEIGHT/2.0-5, 0, 0, 3};
    SDL_Rect r_p1 = {0, 0, 20, 75};
    SDL_Rect r_p2 = {0, 0, 20, 75};
    SDL_Rect r_b = {0, 0, 10, 10};
    srand(time(NULL));

    b.x_s = b.s;    // Initial kick

    while (game_running){
        // Clear frame
        SDL_RenderClear(renderer);

        // Check conditions and set flags (victory, score, etc.)

        // Get input
        SDL_Event event;
        while (SDL_PollEvent(&event)){
            switch (event.type) {
                case SDL_QUIT: game_running = 0; break;
                case SDL_KEYDOWN:
                    switch(event.key.keysym.sym){
                        // Player 1
                        case SDLK_w: p1.speed = -4; break;
                        case SDLK_s: p1.speed = +4; break;
                        // Player 2
                        case SDLK_o: p2.speed = -4; break;
                        case SDLK_l: p2.speed = +4; break;
                    }; break;
                case SDL_KEYUP:
                    switch(event.key.keysym.sym){
                        case SDLK_w: p1.speed = 0; break;
                        case SDLK_s: p1.speed = 0; break;
                        case SDLK_o: p2.speed = 0; break;
                        case SDLK_l: p2.speed = 0; break;
                    }; break;
            }
        }

        // Movements
        if (p1.y + p1.speed > 0 && p1.y + p1.h + p1.speed < HEIGHT)
            p1.y += p1.speed;
        if (p2.y + p2.speed > 0 && p2.y + p2.h + p2.speed < HEIGHT)
            p2.y += p2.speed;
        b.x += b.x_s;
        b.y += b.y_s;
        

        // Game logic
        if (b.x <= p1.x + p1.w && b.y < p1.y + p1.h && b.y + 10 > p1.y){ // bounce on p1
            b.x_s *= -1;
            b.y_s = ((float) rand() / (float) RAND_MAX) * 6 - 3;
        }
        if (b.x + 10 > p2.x && b.y < p2.y + p2.h && b.y + 10 > p2.y){    // bounce on p2
            b.x_s *= -1;
            b.y_s = ((float) rand() / (float) RAND_MAX) * 6 - 3;
        }
        if (b.y < 0 || b.y + 10 > HEIGHT) // up down ball bounce
            b.y_s *= -1;
        if (b.x < 0){
            // p2 score
            p2.score++;
            b.x = WIDTH/2.0-5;
            b.y = HEIGHT/2.0-5;
            b.y_s = 0;

            SDL_Log("score =\tp1 = %d\tp2 = %d\n", p1.score, p2.score);
        }
        if (b.x + 10 > WIDTH){
            // p1 score
            p1.score++;
            b.x = WIDTH/2.0-5;
            b.y = HEIGHT/2.0-5;
            b.y_s = 0;

            SDL_Log("score =\tp1 = %d\tp2 = %d\n", p1.score, p2.score);
        }

        // Render frame
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        r_p1.x = (int) p1.x;
        r_p1.y = (int) p1.y;
        SDL_RenderFillRect(renderer, &r_p1);
        r_p2.x = (int) p2.x;
        r_p2.y = (int) p2.y;
        SDL_RenderFillRect(renderer, &r_p2);
        r_b.x = (int) b.x;
        r_b.y = (int) b.y;
        SDL_RenderFillRect(renderer, &r_b);
        // Render score
        draw_number(renderer, p1.score, WIDTH/2 - 60, 0);
        draw_number(renderer, p2.score, WIDTH/2, 0);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); //background
        SDL_RenderPresent(renderer);

        // Wait for the next frame
        next_game_tick += 1000/60;
        int32_t sleep = next_game_tick - SDL_GetTicks();

        if (sleep > 0)
            SDL_Delay(sleep);
    }

    // Cleanup
    SDL_Quit();

    return 0;
}

// Draw a number (0-9) at the provided coordinates (60x60)
void draw_number(SDL_Renderer *renderer, int n, int x, int y){
    static SDL_Rect r[5];

    if (n < 0 || n > 9)
        return;

    switch (n) {
        case 0: {
            r[0].x = x+10;
            r[0].y = y+10;
            r[0].h = 50;
            r[0].w = 10;
            r[1].x = x+20;
            r[1].y = y+10;
            r[1].h = 10;
            r[1].w = 20;
            r[2].x = x+40;
            r[2].y = y+10;
            r[2].h = 50;
            r[2].w = 10;
            r[3].x = x+20;
            r[3].y = y+50;
            r[3].h = 10;
            r[3].w = 20;
            r[4].x = r[4].y = r[4].h = r[4].w = 0;
        }; break;
        case 1: {
            r[0].x = x+20;
            r[0].y = y+20;
            r[0].h = r[0].w = 10;
            r[1].x = x+30;
            r[1].y = y+10;
            r[1].h = 40;
            r[1].w = 10;
            r[2].x = x+20;
            r[2].y = y+50;
            r[2].h = 10;
            r[2].w = 30;
            r[3].x = r[3].y = r[3].h = r[3].w = 0;
            r[4].x = r[4].y = r[4].h = r[4].w = 0;
        }; break;
        case 2: {
            r[0].x = x+10;
            r[0].y = y+10;
            r[0].h = 10;
            r[0].w = 40;
            r[1].x = x+40;
            r[1].y = y+20;
            r[1].h = 20;
            r[1].w = 10;
            r[2].x = x+10;
            r[2].y = y+30;
            r[2].h = 10;
            r[2].w = 30;
            r[3].x = x+10;
            r[3].y = y+40;
            r[3].h = 20;
            r[3].w = 10;
            r[4].x = x+20;
            r[4].y = y+50;
            r[4].h = 10;
            r[4].w = 30;
        }; break;
        case 3: {
            r[0].x = x+10;
            r[0].y = y+10;
            r[0].h = 10;
            r[0].w = 40;
            r[1].x = x+40;
            r[1].y = y+20;
            r[1].h = 30;
            r[1].w = 10;
            r[2].x = x+20;
            r[2].y = y+30;
            r[2].h = 10;
            r[2].w = 20;
            r[3].x = x+10;
            r[3].y = y+50;
            r[3].h = 10;
            r[3].w = 40;
            r[4].x = r[4].y = r[4].h = r[4].w = 0;
        }; break;
        case 4: {
            r[0].x = x+30;
            r[0].y = y+10;
            r[0].h = 50;
            r[0].w = 10;
            r[1].x = x+20;
            r[1].y = y+20;
            r[1].h = 10;
            r[1].w = 10;
            r[2].x = x+10;
            r[2].y = y+30;
            r[2].h = 10;
            r[2].w = 10;
            r[3].x = x+10;
            r[3].y = y+40;
            r[3].h = 10;
            r[3].w = 40;
            r[4].x = r[4].y = r[4].h = r[4].w = 0;
        }; break;
        case 5: {
            r[0].x = x+10;
            r[0].y = y+10;
            r[0].h = 10;
            r[0].w = 40;
            r[1].x = x+10;
            r[1].y = y+20;
            r[1].h = 20;
            r[1].w = 10;
            r[2].x = x+20;
            r[2].y = y+30;
            r[2].h = 10;
            r[2].w = 30;
            r[3].x = x+40;
            r[3].y = y+40;
            r[3].h = 20;
            r[3].w = 10;
            r[4].x = x+10;
            r[4].y = y+50;
            r[4].h = 10;
            r[4].w = 30;
        }; break;
        case 6: {
            r[0].x = x+20;
            r[0].y = y+10;
            r[0].h = 10;
            r[0].w = 30;
            r[1].x = x+10;
            r[1].y = y+10;
            r[1].h = 50;
            r[1].w = 10;
            r[2].x = x+20;
            r[2].y = y+30;
            r[2].h = 10;
            r[2].w = 30;
            r[3].x = x+20;
            r[3].y = y+50;
            r[3].h = 10;
            r[3].w = 30;
            r[4].x = x+40;
            r[4].y = y+40;
            r[4].h = 10;
            r[4].w = 10;
        }; break;
        case 7: {
            r[0].x = x+10;
            r[0].y = y+10;
            r[0].h = 10;
            r[0].w = 40;
            r[1].x = x+40;
            r[1].y = y+20;
            r[1].h = 10;
            r[1].w = 10;
            r[2].x = x+30;
            r[2].y = y+30;
            r[2].h = 10;
            r[2].w = 10;
            r[3].x = x+20;
            r[3].y = y+40;
            r[3].h = 10;
            r[3].w = 10;
            r[4].x = x+10;
            r[4].y = y+50;
            r[4].h = 10;
            r[4].w = 10;
        }; break;
        case 8: {
            r[0].x = x+10;
            r[0].y = y+10;
            r[0].h = 50;
            r[0].w = 10;
            r[1].x = x+20;
            r[1].y = y+10;
            r[1].h = 10;
            r[1].w = 20;
            r[2].x = x+40;
            r[2].y = y+10;
            r[2].h = 50;
            r[2].w = 10;
            r[3].x = x+20;
            r[3].y = y+30;
            r[3].h = 10;
            r[3].w = 20;
            r[4].x = x+20;
            r[4].y = y+50;
            r[4].h = 10;
            r[4].w = 20;
        }; break;
        case 9: {
            r[0].x = x+10;
            r[0].y = y+10;
            r[0].h = 30;
            r[0].w = 10;
            r[1].x = x+20;
            r[1].y = y+10;
            r[1].h = 10;
            r[1].w = 20;
            r[2].x = x+40;
            r[2].y = y+10;
            r[2].h = 50;
            r[2].w = 10;
            r[3].x = x+20;
            r[3].y = y+30;
            r[3].h = 10;
            r[3].w = 20;
            r[4].x = x+10;
            r[4].y = y+50;
            r[4].h = 10;
            r[4].w = 30;
        }; break;
    }

    for (int i = 0; i < 5; i++)
        SDL_RenderFillRect(renderer, &r[i]);
}