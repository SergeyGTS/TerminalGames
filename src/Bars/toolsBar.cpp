#include "toolsBar.h"

void ToolsBar::initToolsBar(int xStart, int yStart, int xEnd, int yEnd)
{
    toolsBarXStart = xStart;
    toolsBarYStart = yStart;
    toolsBarXEnd = xEnd;
    toolsBarYEnd = yEnd;
}


void ToolsBar::drawToolsBar()
{
    currentToolsBarTitle = firstToolsBarTitle;
    count = 0;

    for (int y = toolsBarYStart ; y <= toolsBarYEnd; y++){
        currentScreen.setCursor(toolsBarXStart - 1, y);
        currentScreen.setColor(COLOR_YELLOW, COLOR_YELLOW);
        currentScreen.writeText(" ");
    }

    for (currentColumn = 1; currentColumn <= columns; currentColumn++) {
        if (count == choicesCount) { break; }
        for (currentRow = 1; currentRow <= rows; currentRow++) {
            currentScreen.setCursor(
                toolsBarXStart + (toolsBarXEnd - toolsBarXStart) / (rows + 1) * currentRow,
                toolsBarYStart + (toolsBarYEnd - toolsBarYStart) / (columns + 1) * currentColumn
            );

            if (currentChoice == count) {
                currentScreen.setColor(COLOR_BLACK, COLOR_WHITE);	
            } else {
                currentScreen.setColor(COLOR_WHITE, COLOR_BLACK);
            }

            writeString(count);

            count++;
            currentToolsBarTitle++;
        }
    }
}


void ToolsBar::menuControllHandler(int key)
{
    switch(key) {
    case currentScreen.controll_keys::UP:
        if (currentChoice == 0) {
            currentChoice = choicesCount - 1;
        } else {
            currentChoice--;
        }
        break;
    case currentScreen.controll_keys::DOWN:
        if (currentChoice == choicesCount - 1) {
            currentChoice = 0;
        } else {
            currentChoice++;
        }
        break;
    case currentScreen.controll_keys::RIGHT:
        if ((currentChoice + rows) > (choicesCount -1)) {
            currentChoice = 0 + (currentChoice % rows);
        } else {
            currentChoice += rows;
        }
        break;
    case currentScreen.controll_keys::LEFT:
        if ((currentChoice - rows) < 0) {
            currentChoice = choicesCount + (currentChoice - rows);
        } else {
            currentChoice -= rows;
        }
        break;
    case currentScreen.controll_keys::SELECT:
        menuControllSelect();
        break;
    }
}