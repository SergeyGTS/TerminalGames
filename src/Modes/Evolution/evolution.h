#ifndef EVOLUTION_H
#define EVOLUTION_H
#include <Screen/screen.h>
#include <Evolution/Bars/evoInfoBar.h>
#include <Evolution/Bars/evoToolsBar.h>
#include <Evolution/Bars/evoSquareBar.h>
#include <Evolution/Models/snakeEvoModel.h>
#include <Database/database.h>

extern Screen currentScreen;

class Evolution
{
    private:
        EvoField field;
        EvoInfoBar infoBar;
        EvoToolsBar toolsBar;
        EvoSquareBar squareBar;

        int key = 0;

        struct snakesList{
            SnakeEvoModel *currentSnake = nullptr;
            struct snakesList *nextSnake = nullptr;
        };
        snakesList* snakes = nullptr;
        snakesList* parentSnakes = nullptr;

        struct bestSnakesList{
            SnakeEvoModel *currentSnake = nullptr;
            struct bestSnakesList *nextSnake = nullptr;
            struct bestSnakesList *prevSnake = nullptr;
        };

        void turn(snakesList *thisSnakes);
        int saveGame(snakesList *thisSnakes);
        int loadGame(snakesList *thisSnakes);
        void drawScreen();
        void evolveSnakes();
        void getBest(snakesList* thisSnakes);
        void initSnakes(snakesList *thisSnakes);
        void drawStuff();
        void deleteSnakes(snakesList *thisSnakes);
        static int saveSnakes(void *NotUsed, int argc, char **argv, char **azColName);

    public:
        Evolution();
	    void run();
};
#endif
