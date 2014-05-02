#include "main.hpp"

const char *ImageAssets::ActorAssetFilename = "ActorAssets.png";
const char *ImageAssets::TileAssetFilename = "TileAssets.png";

ImageAssets::ImageAssets(): actorAssetSheet(NULL), tileAssetSheet(NULL)
{}

void ImageAssets::LoadImagesFromFile()
{
	ALLEGRO_COLOR mask = al_map_rgb(0, 255, 0);

	int index = 0;
	//Create a valid, empty actor asset to return for invalid actor assets
	ALLEGRO_BITMAP *empty = al_create_bitmap(ACTOR_IMAGE_SIZE, ACTOR_IMAGE_SIZE);
	actorAssets.insert(std::pair<int, ALLEGRO_BITMAP*>(index++, empty));

	actorAssetSheet = al_load_bitmap(ActorAssetFilename);
	if(actorAssetSheet)
	{
		al_convert_mask_to_alpha(actorAssetSheet, mask);
		int bitmapWidth = al_get_bitmap_width(actorAssetSheet);
		int bitmapHeight = al_get_bitmap_height(actorAssetSheet);
		
		for(int y = 0; y < bitmapHeight; y+= ACTOR_IMAGE_SIZE)
		for(int x = 0; x < bitmapWidth; x+= ACTOR_IMAGE_SIZE)
		{
			ALLEGRO_BITMAP *actorAsset = al_create_sub_bitmap(actorAssetSheet, x, y, ACTOR_IMAGE_SIZE, ACTOR_IMAGE_SIZE);
			if(actorAsset)
			{
				actorAssets.insert(std::pair<int, ALLEGRO_BITMAP*>(index++, actorAsset));
			}			
		}
	}

	index = 0;
	//Create a valid, empty tile asset to return for invalid actor assets
	empty = al_create_bitmap(TILE_IMAGE_SIZE, TILE_IMAGE_SIZE);
	tileAssets.insert(std::pair<int, ALLEGRO_BITMAP*>(index++, empty));

	tileAssetSheet = al_load_bitmap(TileAssetFilename);
	if(tileAssetSheet)
	{
		al_convert_mask_to_alpha(tileAssetSheet, mask);
		int bitmapWidth = al_get_bitmap_width(tileAssetSheet);
		int bitmapHeight = al_get_bitmap_height(tileAssetSheet);
		
		for(int y = 0; y < bitmapHeight; y+= TILE_IMAGE_SIZE)
		for(int x = 0; x < bitmapWidth; x+= TILE_IMAGE_SIZE)
		{
			ALLEGRO_BITMAP *tileAsset = al_create_sub_bitmap(tileAssetSheet, x, y, TILE_IMAGE_SIZE, TILE_IMAGE_SIZE);
			if(tileAsset)
			{
				tileAssets.insert(std::pair<int, ALLEGRO_BITMAP*>(index++, tileAsset));
			}			
		}
	}
}

void ImageAssets::FreeImages()
{

	for(auto iter = actorAssets.begin(); iter != actorAssets.end(); iter++)
	{
		if( (*iter).second) al_destroy_bitmap((*iter).second);
	}
	if(actorAssetSheet) al_destroy_bitmap(actorAssetSheet);
	//actorAssets.clear();

	for(auto iter = tileAssets.begin(); iter != tileAssets.end(); iter++)
	{
		al_destroy_bitmap((*iter).second);
	}
	if(tileAssetSheet) al_destroy_bitmap(tileAssetSheet);
	tileAssets.clear();	
}

ALLEGRO_BITMAP *ImageAssets::GetActorImage(int imageNo) const
{
	if(actorAssets.count(imageNo) == 1)
	{
		return actorAssets.at(imageNo);
	} 
	//else return actorAssets.at(0);
	else return NULL;
	
}

ALLEGRO_BITMAP *ImageAssets::GetTileImage(int imageNo) const
{
	if(tileAssets.count(imageNo) == 1)
	{
		return tileAssets.at(imageNo);
	} 
	//else return tileAssets.at(0);
	else return NULL;
}

ImageAssets::~ImageAssets()
{}