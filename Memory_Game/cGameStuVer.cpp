/*
==================================================================================
cGame.cpp
==================================================================================
*/
#include "cGame.h"

cGame* cGame::pInstance = NULL;
static cTextureMgr* theTextureMgr = cTextureMgr::getInstance();


/*
=================================================================================
Constructor
=================================================================================
*/
cGame::cGame()
{

}
/*
=================================================================================
Singleton Design Pattern
=================================================================================
*/
cGame* cGame::getInstance()
{
	if (pInstance == NULL)
	{
		pInstance = new cGame();
	}
	return cGame::pInstance;
}


void cGame::initialise(SDL_Window* theSDLWND, SDL_Renderer* theRenderer)
{
	this->m_lastTime = high_resolution_clock::now();
	
	// Clear the buffer with a black background
	SDL_SetRenderDrawColor(theRenderer, 0, 0, 0, 255);
	SDL_RenderPresent(theRenderer);
	
	theTextureMgr->setRenderer(theRenderer);
	// Store the textures
	textureName = { "grass", "grassstone", "grassbare", "grassdark", "treedark", "treelight", "treescary", "treeautumn", "theBackground", "transparent" };
	texturesToUse = { "Images\\tile1.png", "Images\\tile2.png", "Images\\tile3.png", "Images\\tile4.png", "Images\\tree1.png", "Images\\tree2.png", "Images\\tree3.png", "Images\\tree4.png", "Images\\theBackground.png", "Images\\transparent.png" };
	for (unsigned int tCount = 0; tCount < textureName.size(); tCount++)
	{	
		theTextureMgr->addTexture(textureName[tCount], texturesToUse[tCount]);
	}
	theTileMap.setMapStartXY({ 100, 100 });
	theTreePicker.setTreeListStartXY({ 740, 100 });

	spriteBkgd.setSpritePos({ 0, 0 });
	spriteBkgd.setTexture(theTextureMgr->getTexture("theBackground"));
	spriteBkgd.setSpriteDimensions(theTextureMgr->getTexture("theBackground")->getTWidth(), theTextureMgr->getTexture("theBackground")->getTHeight());

	dragTile.setSpritePos({ 0, 0 });
	dragTile.setTexture(theTextureMgr->getTexture("transparent"));
	dragTile.setSpriteDimensions(theTextureMgr->getTexture("transparent")->getTWidth(), theTextureMgr->getTexture("transparent")->getTHeight());

}

void cGame::run(SDL_Window* theSDLWND, SDL_Renderer* theRenderer)
{
	bool loop = true;

	while (loop)
	{
		//We get the time that passed since the last frame
		double elapsedTime = this->getElapsedSeconds();

		loop = this->getInput(loop);
		this->update(elapsedTime);
		this->render(theSDLWND, theRenderer);
	}
}

void cGame::render(SDL_Window* theSDLWND, SDL_Renderer* theRenderer)
{
	SDL_RenderClear(theRenderer);
	spriteBkgd.render(theRenderer, NULL, NULL, spriteBkgd.getSpriteScale());
	theTileMap.render(theSDLWND, theRenderer, theTextureMgr, textureName);
	theTreePicker.render(theSDLWND, theRenderer, theTextureMgr, textureName);
	if (theTreePicker.getTreeToPlant() > -1)
	{
		dragTile.render(theRenderer, &dragTile.getSpriteDimensions(), &dragTile.getSpritePos(), spriteBkgd.getSpriteScale());
	}
	SDL_RenderPresent(theRenderer);
}

void cGame::render(SDL_Window* theSDLWND, SDL_Renderer* theRenderer, double rotAngle, SDL_Point* spriteCentre)
{

	SDL_RenderPresent(theRenderer);
}

void cGame::update()
{

}

void cGame::update(double deltaTime)
{
}

bool cGame::getInput(bool theLoop)
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
		{
			theLoop = false;
		}

		switch (event.type)
		{
			case SDL_MOUSEBUTTONDOWN:
				switch (event.button.button)
				{
				case SDL_BUTTON_LEFT:
				{
					
					// Lab code goes here
					//get mouse point
					SDL_Point mousePos;
					SDL_GetMouseState(&mousePos.x, &mousePos.y);
					//call update on tree picker to set the tree to plant
					theTreePicker.update(mousePos);
					/*
					//call update on tile map passing in the tree to plant
					theTileMap.update(mousePos, theTreePicker.getTreeToPlant());
					*/

					//show the drag tile
					int textureIndex = theTreePicker.getTreeToPlant();
					if (textureIndex >= 0)
					{
						textureIndex = 4 + (textureIndex % 5);
						dragTile.setTexture(theTextureMgr->getTexture(textureName[textureIndex]));
					}
				}
				break;
				case SDL_BUTTON_RIGHT:
					break;
				default:
					break;
				}
				break;
			case SDL_MOUSEBUTTONUP:
				switch (event.button.button)
				{
				case SDL_BUTTON_LEFT:
				{
					
					// get mouse point
						SDL_Point mousePos;
					SDL_GetMouseState(&mousePos.x, &mousePos.y);
					//call update on tile map passing in the tree to plant
					theTileMap.update(mousePos, theTreePicker.getTreeToPlant());

					//reset the tree to plant
					theTreePicker.setTreeToPlant(-1);

					//hide the drag tile
					dragTile.setTexture(theTextureMgr->getTexture("transparent"));
					
				}
				break;
				case SDL_BUTTON_RIGHT:
					break;
				default:
					break;
				}
				break;
			case SDL_MOUSEMOTION:
			{
				
				// Lab Code goes here
				//get mouse point
				SDL_Point mousePos;
				SDL_GetMouseState(&mousePos.x, &mousePos.y);

				//set drag tile position
				dragTile.setSpritePos(mousePos);
			}
			break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					theLoop = false;
					break;
				default:
					break;
				}

			default:
				break;
		}

	}
	return theLoop;
}

double cGame::getElapsedSeconds()
{
	this->m_CurrentTime = high_resolution_clock::now();
	this->deltaTime = (this->m_CurrentTime - this->m_lastTime);
	this->m_lastTime = this->m_CurrentTime;
	return deltaTime.count();
}

void cGame::cleanUp(SDL_Window* theSDLWND)
{
	// Delete our OpengL context
	SDL_GL_DeleteContext(theSDLWND);

	// Destroy the window
	SDL_DestroyWindow(theSDLWND);

	// Quit IMG system
	IMG_Quit();

	// Shutdown SDL 2
	SDL_Quit();
}

