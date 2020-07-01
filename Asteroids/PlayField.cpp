#include "PlayField.h"
#include "EventHandle.h"
#include "GameEngine.h"
#include "Misc Functions.h"

GameState_PlayField::GameState_PlayField()
{
	Init();
}

void GameState_PlayField::Init()
{
	playFieldTest = TTF(game.GetRenderer().renderer);

	playFieldTest.x = 300;
	playFieldTest.y = 300;
	playFieldTest.SetFont(GetEXEPath() + "\\Fonts\\pxl.ttf", 30);
	playFieldTest.SetText("Test");	

	iManager->Bind(SDLK_SPACE, new CommandPlayFieldReturnToLastState());
}

void GameState_PlayField::Cleanup()
{
	allTextures.Cleanup();

	playFieldTest.Clear();
}

bool GameState_PlayField::HandleInput()
{
	bool running = this->iManager->GenerateInput(this->iManager->commandList);

	while (!this->iManager->commandList.empty())
	{
		if (!this->iManager->commandList.back()->Execute())
		{
			running = false;
			break;
		}
		if (this->iManager->commandList.size() > 0)
			this->iManager->commandList.pop_back();
	}

	if (!running)	
		game.PopLastState();

	return true;
}

void GameState_PlayField::HandleEvents()
{
	if (playFieldTest.PointIntersectsTexture(iManager->GetMouseLocation()))
		playFieldTest.SetColor(0, 0, 255);
	else
		playFieldTest.SetColor(255, 255, 255);
}

void GameState_PlayField::Render()
{
	SDL_RenderClear(game.GetRenderer().renderer);

	playFieldTest.Draw();

	SDL_RenderPresent(game.GetRenderer().renderer);
}