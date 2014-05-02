#ifndef IMAGE_ASSETS_HPP
#define IMAGE_ASSETS_HPP

struct ALLEGRO_BITMAP;

enum ACTOR_IMAGE_ASSET_LOCATIONS
{
	MARKER_PONY_IMAGE = 1,
	GRIFFIN_IMAGE
};

enum TILE_IMAGE_ASSET_LOCATIONS
{
	GRASS_TILE_IMAGE = 1,
	STONE_TILE_IMAGE
};

class ImageAssets
{
public:
	ImageAssets();
	void LoadImagesFromFile();
	void FreeImages();
	~ImageAssets();

	static const int TILE_IMAGE_SIZE = 32;
	static const char *TileAssetFilename;
	ALLEGRO_BITMAP *GetTileImage(int imageNo) const;

	static const int ACTOR_IMAGE_SIZE = 32;
	static const char *ActorAssetFilename;
	ALLEGRO_BITMAP* GetActorImage(int imageNo) const;

private:
	std::map<int, ALLEGRO_BITMAP*> actorAssets;
	ALLEGRO_BITMAP *actorAssetSheet;

	std::map<int, ALLEGRO_BITMAP*> tileAssets;
	ALLEGRO_BITMAP *tileAssetSheet;
};

#endif