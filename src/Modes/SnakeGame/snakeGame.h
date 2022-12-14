#ifndef SNAKE_GAME_H
#define SNAKE_GAME_H
#include <SnakeGame/Models/snakeModel.h>
#include <SnakeGame/Models/foodModel.h>
#include <SnakeGame/Menus/pauseMenu.h>
#include <Screen/screen.h>
#include <string>

extern Screen currentScreen;

class SnakeGame
{
    private:
        int foodX, foodY, snakeX, snakeY;

        Screen::controll_keys key;
        Screen::controll_keys vector;
        PauseMenu pauseMenu;

        std::string deathText = "YOU ARE FINISHED";
        int deathTextColor = COLOR_RED;
        int deathTextBackground = COLOR_BLACK;

        struct foodList {
            FoodModel currentFood;
            bool isFood;
            struct foodList *nextFood;
        };
        foodList* food = new foodList;
        foodList* foodTmp = new foodList;

        struct snakesList{
            SnakeModel currentSnake;
            struct foodList *thisSnakesFood;
            struct snakesList *nextSnake;
        };
        snakesList* snakes = new snakesList;
        snakesList* snakeTmp = new snakesList;

        void turn();
        void checkFood();
        void checkDeath();
        void death();
        void drawSnakes();
        void drawFood();
        void eatFood(snakesList* snakes, foodList* food);
       
    public:
	    void run();
};
#endif
