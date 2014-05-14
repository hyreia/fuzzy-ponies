#include "main.hpp"

Renderer::Renderer(){}

TileRenderer::TileRenderer(){}

void TileRenderer::RenderWorld(WorldStage *stage, int focusX, int focusY)
{
	Color tint[4];
	tint[0] = Color(255, 255, 255, 255);
	tint[1] = Color(245, 245, 245, 255);
	tint[2] = Color(235, 235, 235, 255);
	tint[3] = Color(225, 225, 225, 255);

	const int tileSize = game->GetTileSize();
	int width = game->GetWindowWidth() / tileSize;
	int height = game->GetWindowHeight() / tileSize;

	//Draw current map
	for(int x = 0; x < width && focusX+x < (int)stage->currentMap->tile.size(); x++)
	{
		for(int y = 0; y < height && focusY+y < (int)stage->currentMap->tile[0].size(); y++)
		{
			auto m = stage->materials[stage->currentMap->tile[focusX+x][focusY+y].materialTypeIndex];
			auto variant = stage->currentMap->tile[focusX+x][focusY+y].variant;
			auto texture = game->imageAssets.GetTileImage(m.image);
			if(texture)
			{
				al_draw_tinted_bitmap(texture, 
					tint[stage->currentMap->tile[focusX+x][focusY+y].variant ], x*tileSize, y*tileSize, 0);
			}
		}
	}

	auto actors = stage->actorList.GetActors();
	while(!actors.empty())
	{
		auto a = actors.top();
		auto texture = game->imageAssets.GetActorImage(a->image);
		if(texture)
		{
			al_draw_bitmap(texture, (a->x-focusX)*tileSize, (a->y-focusY)*tileSize, 0);
		}

		actors.pop();
	}
	//Draw actors
	
	{
/*		auto texture = game->imageAssets.GetActorImage((*actor)->image);
		if(texture)
		{
			al_draw_bitmap(texture, 
			((*actor)->x-xOffset)*TILE_WIDTH, ((*actor)->y-yOffset)*TILE_HEIGHT, 
			0);
		}
		else
		{
			al_draw_textf(tileFont, (*actor)->color, TILE_WIDTH*(*actor)->x-xOffset, TILE_HEIGHT*(*actor)->y-yOffset, 0, "%c", 
				(*actor)->ascii);
		}*/
	}
}