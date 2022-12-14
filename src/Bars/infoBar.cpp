#include "infoBar.h"

void InfoBar::initInfoBar(int xStart, int yStart, int xEnd, int yEnd)
{
    infoBarXStart = xStart;
    infoBarYStart = yStart;
    infoBarXEnd = xEnd;
    infoBarYEnd = yEnd;
}


void InfoBar::drawInfoBar()
{
    currentInfoBarTitle = firstInfoBarTitle;
    count = 0;

    for (int x = infoBarXStart ; x < infoBarXEnd; x++){
        currentScreen.setCursor(x, infoBarYStart - 1);
        currentScreen.setColor(COLOR_YELLOW, COLOR_YELLOW);
        currentScreen.writeText(" ");
    }

    for (int currentColumn = 1; currentColumn <= columns; currentColumn++) {
        for (int currentRow = 1; currentRow <= rows; currentRow++) {
             currentScreen.setCursor(
                infoBarXStart + (infoBarXEnd - infoBarXStart) / (rows + 1) * currentRow,
                infoBarYStart + (infoBarYEnd - infoBarYStart) / (columns + 3) * currentColumn * 2 - currentColumn * 2
            );
            
            currentScreen.setColor(COLOR_WHITE, COLOR_BLACK);
            writeString(count);

            currentScreen.setCursor(
                1 + infoBarXStart + (infoBarXEnd - infoBarXStart) / (rows + 1) * currentRow,
                infoBarYStart + (infoBarYEnd - infoBarYStart) / (columns + 3) * currentColumn * 2 - currentColumn * 2
            );
            currentInfoParameter = getParameter(currentInfoBarTitle);
            printParameter(currentInfoParameter);
            currentInfoBarTitle++;
            count++;
        }
    }
}


void InfoBar::printParameter(int parameter)
{
    std::string parameterString;
    parameterString = std::to_string(parameter);
    const char *parameterChar = parameterString.c_str();
	currentScreen.writeText(parameterChar);
}