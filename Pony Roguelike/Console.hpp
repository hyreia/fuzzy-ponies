#ifndef CONSOLE_HPP
#define CONSOLE_HPP

#include <vector>
struct ConsoleTile;
struct ALLEGRO_COLOR;
struct Map;

enum PRINT_ALIGNMENT
{
	PRINT_LEFT,
	PRINT_CENTER,
	PRINT_RIGHT
};

struct Console
{
	Console(int width, int height);
	~Console();

	void SetDefaultBackgroundColor(const Color background);
	void SetDefaultForegroundColor(const Color foreground);
	void SetColorPair(int colorIndex, const Color &fg, const Color &bg);


	/** Clears all cells to console's background color, removes any textures or icons */
	void Clear();

	/** Force console to redraw its bitmap */
	void ForceRedraw();

	/** Draws any console cells that have been updated but not drawn yet */
	void UpdateBitmap();

	static void SetGlobalConsoleTileSize(int width, int height, ALLEGRO_FONT *font);

	/** Copies bitmap of src console to bitmap of dest console
		
		If alpha is < 1.0f then transparency is used
		If alpha is 0.0f this has no effect
	*/
	static void GraphicBlit(Console *src, const int &sx, const int &sy, int sw, int sh,
		Console *dest, 
		const int dx, const int dy, 
		float alpha = 1.0f);

	/* Draws console bitmap to target bitmap*/
	void GraphicBlit(float dx, float dy, float dw, float dh, float alpha = 1.0f);

	std::vector< std::vector<ConsoleTile>> tiles;
	const int width, height;

	/* Sets the character at the given tile position with tile's color properties */
	void SetChar(int x, int y, int c);
	/* Places character at given tile position with console's colors */
	void PutChar(int x, int y, int c);
	void PutChar(int x, int y, int c, const Color characterColor, ALLEGRO_BITMAP *texture=NULL);

	int ReadChar(int x, int y);



	/* Prints string to console in starting cell, 
	   - ignores \n (to multiline, use PrintRect)
	   Can also use %c with COLOR_PAIR_# variables to change color mid string
	*/
	//void Print(int x, int y, const Message message, bool preserveColor = true);

	void ClearRect(int x1, int y1, int x2, int y2, const Color color,
		char c = ' ');


	/* Prints text on multiple lines. Respects \n */
	//int PrintRect(int x, int y, int w, int h,  const std::string &string, ...);
	
	PRINT_ALIGNMENT GetPrintAlignment();
	void SetPrintAlignment(PRINT_ALIGNMENT alignment);


	void DrawMap(Map &map, int firstMapTileX, int firstMapTileY, int firstConsoleTileX, int firstConsoleTileY);
private:
	int PrintInternal(int x, int y, int x2, int y2, std::string &str, bool useForeground, bool useBackground, bool isMultipleLines);

	static int tileWidth, tileHeight;
	static int tileHalfWidthMinusFontHalfWidth, tileHalfHeightMinusFontHalfHeight;
	static ALLEGRO_FONT *font;

	PRINT_ALIGNMENT printAlignment;

	ALLEGRO_BITMAP *bitmap;
	Color backgroundColor, foregroundColor;

	ColorPair colorPairs[TOTAL_COLOR_PAIRS];
};

#endif