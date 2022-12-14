#include "evolution.h"
#include <Evolution/Parameters/evolutionParameters.h>
#include <fstream>
#include <iostream>

extern EvolutionParameters evolutionParameters;
extern Database *database;

Evolution::Evolution()
{
    infoBar.initInfoBar(
        evolutionParameters.infoBarStartX,
        evolutionParameters.infoBarStartY,
        evolutionParameters.infoBarEndX,
        evolutionParameters.infoBarEndY
    );
    toolsBar.initToolsBar(
        evolutionParameters.toolsBarStartX,
        evolutionParameters.toolsBarStartY, 
        evolutionParameters.toolsBarEndX, 
        evolutionParameters.toolsBarEndY
    );
    squareBar.initSquareBar(
        evolutionParameters.squareBarStartX,
        evolutionParameters.squareBarStartY, 
        evolutionParameters.squareBarEndX, 
        evolutionParameters.squareBarEndY
    );
}


void Evolution::run()
{
    timeout(evolutionParameters.delayDuration);

    currentScreen.clearScreen();

    evolutionParameters.turn = 0;
    evolutionParameters.score = 0;
    evolutionParameters.generation = 1;
    evolutionParameters.snakeIdCounter = 1;
    evolutionParameters.aliveSnakes = 0;

    snakes = new snakesList;
    initSnakes(snakes);

	evolutionParameters.gameOn = true;

    while(evolutionParameters.gameOn){
        key = currentScreen.controllHandler();

        if (!toolsBar.pauseOn){
            turn(snakes);
            evolutionParameters.hightTurnsLeft--;
            evolutionParameters.turn++;
            evolutionParameters.time++;
            snakesList *snakesListTmp = nullptr;
            snakesListTmp = snakes;
            while(snakesListTmp->nextSnake){
                if (snakesListTmp->currentSnake->isAlive)
                {
                    snakesListTmp->currentSnake->setVector(key);
                }
                snakesListTmp = snakesListTmp->nextSnake;
            }
            snakesListTmp = nullptr;
        }

        if (evolutionParameters.aliveSnakes < 1){
            evolutionParameters.hightTurnsLeft = evolutionParameters.turnsToDeath;
            evolutionParameters.snakeIdCounter = 1;
            evolutionParameters.bestFood = 0;
            evolutionParameters.generation++;
            evolutionParameters.turn = 0;
            if (evolutionParameters.score > evolutionParameters.theBestScore){
                evolutionParameters.theBestScore = evolutionParameters.score;
            }
            evolutionParameters.score = 0;
            evolveSnakes();
        }

        drawScreen();
        drawStuff();
        toolsBar.menuControllHandler(key);
        if (!evolutionParameters.gameOn){
            deleteSnakes(snakes);
        }

        if (evolutionParameters.saveGame){
            saveGame(snakes);
            evolutionParameters.saveGame = false;
        }

        if (evolutionParameters.loadGame){
            loadGame(snakes);
            evolutionParameters.loadGame = false;
        }
    }
    currentScreen.clearScreen();
}


void Evolution::turn(struct snakesList *thisSnakes)
{
    snakesList *snakesListTmp = nullptr;
    snakesListTmp = thisSnakes;
    while(snakesListTmp->nextSnake){
        if (snakesListTmp->currentSnake->isAlive)
        {
            snakesListTmp->currentSnake->score++;
            if (snakesListTmp->currentSnake->score > evolutionParameters.score){
                evolutionParameters.score = snakesListTmp->currentSnake->score;
                evolutionParameters.bestSnakeId = snakesListTmp->currentSnake->snakeId;
            }
            snakesListTmp->currentSnake->move();
        }
        snakesListTmp = snakesListTmp->nextSnake;
    }
    snakesListTmp = nullptr;

    currentScreen.endFrame();
    currentScreen.clearScreen();
}


void Evolution::drawScreen()
{
    infoBar.drawInfoBar();
    toolsBar.drawToolsBar();

    snakesList *snakesListTmp = nullptr;
    snakesListTmp = snakes;
    do {
        if (snakesListTmp->currentSnake->isAlive){
            snakesListTmp->currentSnake->drawField();
            squareBar.drawMap(snakesListTmp->currentSnake->field);
            break;
        }
        snakesListTmp = snakesListTmp->nextSnake;
    } while (snakesListTmp->nextSnake);
    snakesListTmp = nullptr;
}


void Evolution::drawStuff()
{
    snakesList *snakesListTmp = nullptr;
    snakesListTmp = snakes;

    while(snakesListTmp->nextSnake) {
        if (snakesListTmp->currentSnake->isAlive) { 
            snakesListTmp->currentSnake->drawStuff();
        }
	    snakesListTmp = snakesListTmp->nextSnake;
    }
    snakesListTmp = nullptr; 
}


void Evolution::initSnakes(snakesList *thisSnakes)
{
    snakesList *snakesListTmp = nullptr;
    snakesListTmp = thisSnakes;

    for (int count = 0; count < evolutionParameters.countOfSnakes; count++)
	{
        SnakeEvoModel *newSnake = new SnakeEvoModel;
        newSnake->init(
            evolutionParameters.fullFieldSizeX / 2,
            evolutionParameters.fullFieldSizeY / 2,
            evolutionParameters.snakeLength,
            evolutionParameters.snakeOneBodyColor
        );
        snakesListTmp->currentSnake = newSnake;
        snakesListTmp->nextSnake = new snakesList;
        snakesListTmp = snakesListTmp->nextSnake;
        snakesListTmp->nextSnake = nullptr;
        snakesListTmp->currentSnake = nullptr;
    }
    snakesListTmp = nullptr;
    squareBar.initField(snakes->currentSnake->field);
}


void Evolution::evolveSnakes()
{
    //snakesList *parentSnakes = nullptr;
    //parentSnakes = getBest(snakes);
     getBest(snakes);

    snakesList *newSnakesList = new snakesList;
    snakesList *snakesListTmp = nullptr;
    snakesListTmp = newSnakesList;

    //snakeSecondTmp = new snakesList;
    //snakesListTmp = snakeSecondTmp;

    int counter = 1;
    for (int count = 0; count < evolutionParameters.countOfSnakes; count++)
	{
        SnakeEvoModel *newSnake = new SnakeEvoModel;
        if ((evolutionParameters.fullFieldSizeY / 2) + (count * 2) + counter > (evolutionParameters.fullFieldSizeY - 3)){
            counter = 1;
        }
        newSnake->init(
            evolutionParameters.fullFieldSizeX / 2,
            evolutionParameters.fullFieldSizeY / 2,
            evolutionParameters.snakeLength,
            evolutionParameters.snakeOneBodyColor
        );

        int parentsCount = ((evolutionParameters.countOfSnakes / 10) * evolutionParameters.countOfBest / 10) + ((evolutionParameters.countOfSnakes / 10) * evolutionParameters.countOfWorst / 10);
        
        std::random_device random_device;
        std::mt19937 generator(random_device());
        std::uniform_int_distribution<> randGen(0, parentsCount - 1);

        int parentOneNum = 0;
        int parentTwoNum = 0;

        while (parentOneNum == parentTwoNum) {
            parentOneNum = randGen(generator);
            parentTwoNum = randGen(generator);
        }

        snakesList *firstParent = nullptr;
        firstParent = parentSnakes;
        for (int countX = 0; countX < parentOneNum; countX++){
            firstParent = firstParent->nextSnake;
        }

        snakesList *secondParent = nullptr;
        secondParent = parentSnakes;
        for (int countX = 0; countX < parentTwoNum; countX++){
            secondParent = secondParent->nextSnake;
        }
        
        newSnake->network.mergeNetworks(firstParent->currentSnake->network.layersList, secondParent->currentSnake->network.layersList);
        
        snakesListTmp->currentSnake = newSnake;
        snakesListTmp->nextSnake = new snakesList;
        snakesListTmp = snakesListTmp->nextSnake;
        snakesListTmp->nextSnake = nullptr;
        snakesListTmp->currentSnake = nullptr;
        counter++;
    }

    deleteSnakes(snakes);
    deleteSnakes(parentSnakes);
    snakes = newSnakesList;
}


void Evolution::deleteSnakes(snakesList *thisSnakes)
{
    snakesList *snakesTmpList = nullptr;
    snakesList *snakesSecondTmpList = nullptr;

    snakesTmpList = thisSnakes;

    while(snakesTmpList){
        if (snakesTmpList->currentSnake){
            snakesTmpList->currentSnake->deleteSnake();
            //delete snakesTmpList->currentSnake;
        }
        snakesTmpList->currentSnake = nullptr;
        snakesSecondTmpList = snakesTmpList;
        snakesTmpList = snakesTmpList->nextSnake;
        delete snakesSecondTmpList;
        snakesSecondTmpList = nullptr;
    }
}


void Evolution::getBest(snakesList *thisSnakes)
{
    snakesList *snakesListTmp = nullptr;
    snakesListTmp = thisSnakes;


    bestSnakesList *snakesRating = nullptr;
    snakesRating = new bestSnakesList;

    snakesRating->currentSnake = snakesListTmp->currentSnake;
    bestSnakesList *snakesTmpRating = nullptr;
    snakesListTmp = snakesListTmp->nextSnake;

    while (snakesListTmp->currentSnake){
        snakesTmpRating = snakesRating;
        while(snakesTmpRating->currentSnake){
        
            if (snakesListTmp->currentSnake->score >= snakesTmpRating->currentSnake->score){
                bestSnakesList *snakeTmp = new bestSnakesList;
                snakeTmp->currentSnake = snakesListTmp->currentSnake;
                snakeTmp->nextSnake = snakesTmpRating;
                if (snakesTmpRating->prevSnake != nullptr) {
                    snakeTmp->prevSnake = snakesTmpRating->prevSnake;
                    snakesTmpRating->prevSnake->nextSnake = snakeTmp;
                } else {
                    snakeTmp->prevSnake = nullptr;
                    snakesRating = snakeTmp;
                }
                snakesTmpRating->prevSnake = snakeTmp;
                break;
            } else if (!snakesTmpRating->nextSnake){
                snakesTmpRating->nextSnake = new bestSnakesList;
                snakesTmpRating->nextSnake->currentSnake = snakesListTmp->currentSnake;
                snakesTmpRating->nextSnake->nextSnake = nullptr;
                snakesTmpRating->nextSnake->prevSnake = snakesTmpRating;
                break;
            }
            snakesTmpRating = snakesTmpRating->nextSnake;
        }
        snakesListTmp = snakesListTmp->nextSnake;
    }

    snakesTmpRating = snakesRating;
    while(snakesTmpRating->nextSnake){
        snakesTmpRating = snakesTmpRating->nextSnake;
    }
    bestSnakesList *worstSnake = nullptr;
    worstSnake = snakesTmpRating;

    bestSnakesList *bestSnake = nullptr;
    bestSnake = snakesRating;

    parentSnakes = new snakesList;
    snakesList *parentTmpSnakes = nullptr;
    parentTmpSnakes = parentSnakes;


    for (int count = 0; count < ((evolutionParameters.countOfSnakes / 10) * evolutionParameters.countOfBest / 10); count++){
        parentTmpSnakes->currentSnake = bestSnake->currentSnake;
        parentTmpSnakes->nextSnake = new snakesList;
        parentTmpSnakes = parentTmpSnakes->nextSnake;
        parentTmpSnakes->nextSnake = nullptr;

        bestSnake = bestSnake->nextSnake;
    }

    for (int count = 0; count < ((evolutionParameters.countOfSnakes / 10) * evolutionParameters.countOfWorst / 10); count++){
        parentTmpSnakes->currentSnake = worstSnake->currentSnake;
        parentTmpSnakes->nextSnake = new snakesList;
        parentTmpSnakes = parentTmpSnakes->nextSnake;
        parentTmpSnakes->nextSnake = nullptr;

        worstSnake = worstSnake->prevSnake;
    }
    while (snakesRating){
        snakesTmpRating = snakesRating;
        snakesRating = snakesRating->nextSnake;
        delete snakesTmpRating;
    }

    snakesRating = nullptr;
    worstSnake = nullptr;
    bestSnake = nullptr;
    //deleteSnakes(snakesRating);
    //return parentSnakes;
}


int Evolution::saveGame(snakesList *thisSnakes)
{
    sqlite3_stmt *stmt;

    std::string request = "INSERT INTO straight_network_session ( "
                                "generation, best_score, turns_to_death, count_of_snakes, snake_length, count_of_food, "
                                "count_of_borders, first_layer_neuron_count, last_layer_neuron_count, output_layer_neuron_count, "
                                "count_of_best, count_of_worst, mutation_chance, game_sub_mode_id "
                            ") values ("
                                "?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, (SELECT ID FROM game_sub_modes WHERE name='Straight Network') "
                            ")";

    int result = sqlite3_prepare_v2(database->ppDb, request.c_str(), -1, &stmt, NULL);
    if (result) {
        printf("Error executing sql statement\n");
        sqlite3_close(database->ppDb);
        exit(1);
    }

    sqlite3_bind_int(stmt, 1, evolutionParameters.generation);
    sqlite3_bind_int(stmt, 2, evolutionParameters.theBestScore);
    sqlite3_bind_int(stmt, 3, evolutionParameters.turnsToDeath);
    sqlite3_bind_int(stmt, 4, evolutionParameters.countOfSnakes);
    sqlite3_bind_int(stmt, 5, evolutionParameters.snakeLength);
    sqlite3_bind_int(stmt, 6, evolutionParameters.countOfFood);
    sqlite3_bind_int(stmt, 7, evolutionParameters.countOfBorders);
    sqlite3_bind_int(stmt, 8, evolutionParameters.firstLayerNeuronCount);
    sqlite3_bind_int(stmt, 9, evolutionParameters.lastLayerNeuronCount);
    sqlite3_bind_int(stmt, 10, evolutionParameters.outputLayerNeuronCount);
    sqlite3_bind_int(stmt, 11, evolutionParameters.countOfBest);
    sqlite3_bind_int(stmt, 12, evolutionParameters.countOfWorst);
    sqlite3_bind_int(stmt, 13, evolutionParameters.mutationChance);

    sqlite3_step(stmt);

    sqlite3_finalize(stmt);

    sqlite3_exec(ppDb, "SELECT ID FROM straight_network_session ORDER BY ID DESC", saveSnakes, 0, &(database->errmsg));

    return 0;

}


int saveSnakes(void *NotUsed, int argc, char **argv, char **azColName)
{
    
    return 0;
}


int Evolution::loadGame(snakesList *thisSnakes)
{
    return 0;
}