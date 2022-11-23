#include <SDL2/SDL_events.h>
#include <SDL2/SDL_hints.h>
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <stdio.h>
#include <stdlib.h>

#include <SDL2/SDL.h>

typedef struct{
    SDL_Rect *body;
    int velocity;
    int speed;
} Player;

typedef struct{
    SDL_Rect *body;
    float angle;
    int speed;
    int direction;
} Ball;

#define TRUE 1
#define FALSE 0

static const int WIDTH = 400;
static const int HEIGHT = 300;
//static const int SPEED = 3;

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
static Player *player1 = NULL;
static Player *player2 = NULL;
static Ball *ball = NULL;

void initialize();
void cleanup();
void handle_key_press(SDL_Keysym *keysym, SDL_EventType type);
void handle_player_movements(Player *player);
void handle_ball_movement(Ball *ball);
void render_player(Player *player);
void render_ball(Ball *ball);

Player *CreatePlayer(int x, int y, int width, int height, int speed);
Ball *CreateBall(int size);

int main(int argc, char **argv)
{
    int isRunning = TRUE;
    initialize();

    int sleep = 0;
    Uint32 next_game_tick = SDL_GetTicks();

    // Game loop
    while (isRunning){
        // Clear the previous rendered frame
        SDL_RenderClear(renderer);

        // Catch input
        SDL_Event event;
        while (SDL_PollEvent(&event)){
            switch (event.type) {
                case SDL_QUIT: isRunning = FALSE; break;
                case SDL_KEYDOWN: handle_key_press(&event.key.keysym, SDL_KEYDOWN); break;
                case SDL_KEYUP: handle_key_press(&event.key.keysym, SDL_KEYUP); break;
            }
        }

        // Handle the movements
        handle_player_movements(player1);
        handle_player_movements(player2);
        handle_ball_movement(ball);

        // Render players
        render_player(player1);
        render_player(player2);

        // Render the ball
        render_ball(ball);

        // Present the new frame
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); //background
        SDL_RenderPresent(renderer);

        //Wait for the next tick
        next_game_tick += 1000/60;
        sleep = next_game_tick - SDL_GetTicks();
        if (sleep > 0)
            SDL_Delay(sleep);
    }

    cleanup();
    return 0;
}

// Perform all the necessary initializations of SDL
void initialize(){
    // Initialize SDL
    SDL_Init(SDL_INIT_VIDEO);

    // Create the window
    window = SDL_CreateWindow(
        "Pong", 
        SDL_WINDOWPOS_UNDEFINED, 
        SDL_WINDOWPOS_UNDEFINED, 
        WIDTH, 
        HEIGHT, 
        SDL_WINDOW_OPENGL
    );

    // Create renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Create players
    player1 = CreatePlayer(30, 50, 20, 75, 3);
    player2 = CreatePlayer(350, 50, 20, 75, 3);

    // Create ball
    ball = CreateBall(10);
}

// Perform all the necessary cleanups before quitting
void cleanup(){
    free(player1);
    free(player2);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

// Handle all the keyboard input
void handle_key_press(SDL_Keysym *keysym, SDL_EventType type){
    if (type == SDL_KEYDOWN)
        switch (keysym->sym) {
            // Player1
            case SDLK_w: player1->velocity = -1; break;
            case SDLK_s: player1->velocity = +1; break;

            // Player2
            case SDLK_o: player2->velocity = -1; break;
            case SDLK_l: player2->velocity = +1; break;
        }

    if (type == SDL_KEYUP)
        switch (keysym->sym) {
            // Player1
            case SDLK_w: player1->velocity = 0; break;
            case SDLK_s: player1->velocity = 0; break;

            // Player2
            case SDLK_o: player2->velocity = 0; break;
            case SDLK_l: player2->velocity = 0; break;
        }
}

// Handle the player movements
void handle_player_movements(Player *player){
    if (player->body->y + player->velocity * player->speed > 0 &&
            player->body->y + player->body->h + player->velocity * player->speed < HEIGHT)
            player->body->y += player->velocity * player->speed;
}

// Handle all the ball movements
void handle_ball_movement(Ball *ball){
    int *x = &ball->body->x;
    int *y = &ball->body->y;
    int *speed = &ball->speed;
    float *angle = &ball->angle;
    int *direction = &ball->direction;
    int size = ball->body->w;

    if ((*x) >  WIDTH - size)
        (*direction) = -1;
    
    if ((*x) < 0)
        (*direction) = +1;


    (*x) += (*direction) * (*speed);
    //(*y) += (int) (*angle) * (*x);
}

// Render the player to the screen
void render_player(Player *player){
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, player->body);
}

// Render the ball to the screen
void render_ball(Ball *ball){
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, ball->body);
}

// Return a pointer to a newly created player
Player *CreatePlayer(int x, int y, int width, int height, int speed){
    Player *p = (Player*) malloc(sizeof(Player));;
    p->body = (SDL_Rect*) malloc(sizeof(SDL_Rect));

    p->body->x = x;
    p->body->y = y;
    p->body->w = width;
    p->body->h = height;
    p->speed = speed;

    return p;
}

// Return a pointer to a newly created ball
Ball *CreateBall(int size){
    Ball *b = (Ball*) malloc(sizeof(Ball));
    b->body = (SDL_Rect*) malloc(sizeof(SDL_Rect));

    b->body->x = (WIDTH/2) - (size/2);
    b->body->y = (HEIGHT/2) - (size/2);
    b->body->h = size;
    b->body->w = size;

    b->angle = 1.0f;
    b->speed = 2;
    b->direction = 1;

    return b;
}