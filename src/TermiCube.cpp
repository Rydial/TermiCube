#include <iostream>
#include <curses.h>
#include "TermiCube.h"


void GameWindow::pollEvents() 
{

}


void GameWindow::updateWindow() 
{
    screenList[static_cast<size_t>(screen)]->updateScreen();
    screenList[static_cast<size_t>(screen)]->drawGraphics();
}

void MainMenuScreen::drawGraphics() 
{

}


void MainMenuScreen::updateScreen()
{
    std::cout << "HI" << '\n';
    clear();
}



void MainMenuScreen::userInput()
{

}

void GameScreen::drawGraphics()
{

}



void GameScreen::updateScreen()
{

}



void GameScreen::userInput()
{

}
