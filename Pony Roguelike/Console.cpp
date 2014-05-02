#include "main.hpp"
#include <stdarg.h>
#include <algorithm>


int Console::tileWidth = 0;
int Console::tileHeight = 0;
int Console::tileHalfWidthMinusFontHalfWidth = 0;
int Console::tileHalfHeightMinusFontHalfHeight = 0;

ALLEGRO_FONT *Console::font = NULL;

Console::Console(int width, int height):
width(width), height(height), printAlignment(PRINT_LEFT),
backgroundColor(Color::Black), foregroundColor(Color::White)
{
	ConsoleTile tile;
	std::vector<ConsoleTile> conTileColumn(height, tile);
	std::vector<std::vector<ConsoleTile> > conTileRow(width, conTileColumn);
	tiles = conTileRow;

	bitmap = al_create_bitmap(width*tileWidth, height*tileHeight);
}


void Console::SetDefaultBackgroundColor(const Color background)
{
	backgroundColor = background;
}

void Console::SetDefaultForegroundColor(const Color foreground)
{
	foregroundColor = foreground;
}

void Console::SetColorPair(int colorIndex, const Color &fg, const Color &bg)
{
	if(colorIndex < 1 || colorIndex >= TOTAL_COLOR_PAIRS) return;

	colorPairs[colorIndex-1].Set(fg, bg);

}


Console::~Console()
{
	al_destroy_bitmap(bitmap);
}

void Console::Clear()
{
	for(int x = 0; x < width; x++)
		for(int y = 0; y < height; y++)
		{
			tiles[x][y].backgroundColor = backgroundColor;
			tiles[x][y].foregroundColor = foregroundColor;
			tiles[x][y].texture = NULL;
			tiles[x][y].ch = ' ';
			tiles[x][y].redraw = true;
			//Clear any bitmaps
		}
		al_set_target_bitmap(bitmap);
		al_clear_to_color(backgroundColor);
}

void Console::ClearRect(int x1, int y1, int x2, int y2, const Color color,
char c)
{
	if(y2 > height-1) y2 = height-1;
	if(x2 > width-1) x2 = width-1;
	if(y1 > y2){ int temp = y1; y1 = y2; y2 = temp; }
	if(x1 > x2){ int temp = x1; x1 = x2; x2 = temp; }

	for(int x = x1; x < x2; x++) for(int y = y1; y < y2; y++)
	{
		tiles[x][y].redraw = true;
		tiles[x][y].backgroundColor = color;
		tiles[x][y].foregroundColor = foregroundColor;
		tiles[x][y].ch = c;
		tiles[x][y].texture = NULL;

	}

}

void Console::ForceRedraw()
{
	for(int x = 0; x < width; x++)
		for(int y = 0; y < height; y++)
		{
			tiles[x][y].redraw = true;
		}

	UpdateBitmap();
}

void Console::UpdateBitmap()
{
	al_set_target_bitmap(bitmap);
	for(int x = 0; x < width; x++) for(int y = 0; y < height; y++)
	{
		if(tiles[x][y].redraw)
		{
			//No graphic
			if(!tiles[x][y].texture)
			{
				al_draw_filled_rectangle(x*tileWidth, y*tileHeight, (x+1)*tileWidth, (y+1)*tileHeight,
				tiles[x][y].backgroundColor);

				if(tiles[x][y].ch != ' ')
				{
					al_draw_textf(font, tiles[x][y].foregroundColor, 
						x*tileWidth + (tileHalfWidthMinusFontHalfWidth), 
						y*tileHeight + (tileHalfHeightMinusFontHalfHeight)/*+ fontHalfHeight*1.5f*/, ALLEGRO_ALIGN_CENTRE, " %c",
					tiles[x][y].ch);
				}
			}
			else
			{
				al_draw_bitmap(tiles[x][y].texture, x*tileWidth, y*tileHeight, 0);
			}
			//else we draw the textures and icons here

			//Do char
			

			tiles[x][y].redraw = false;
		}
	}
}


void Console::SetGlobalConsoleTileSize(int width, int height, ALLEGRO_FONT *_font)
{
	tileWidth=width;
	tileHeight=height;
	font=_font;
	tileHalfWidthMinusFontHalfWidth   = tileWidth/2 - al_get_text_width(font, "A")/2;
	tileHalfHeightMinusFontHalfHeight = tileWidth/2 - al_get_font_line_height(font)/2;

}

void Console::GraphicBlit(Console *src, const int &sx, const int &sy, int sw, int sh,
Console *dest, 
const int dx, const int dy, 
float alpha)
{
	src->UpdateBitmap();
	dest->UpdateBitmap();
	
	//Making blitting safe
	if(src == NULL || dest == NULL) return;
	if(alpha <= 0.0f) return;
	if(alpha > 1.0f) alpha = 1.0f;

	al_set_target_bitmap(dest->bitmap);
	al_draw_tinted_bitmap_region(src->bitmap, al_map_rgba_f(1.0f, 1.0f, 1.0f, alpha), 
		sx*tileWidth, sy*tileHeight, sw*tileWidth, sh*tileHeight, 
		dx*tileWidth, dy*tileHeight, 0);

	
}

void Console::GraphicBlit(float dx, float dy, float dw, float dh, float a)
{
	al_draw_tinted_scaled_rotated_bitmap(bitmap, 
		al_map_rgba_f(a, a, a, a), 0, 0,
		0, 0, //width*tileWidth, height*tileHeight, 
		1.0f, 1.0f,
		0, 0);
		//dx, dy, dw, dh, 0);
}

void Console::SetChar(int x, int y, int c)
{
	tiles[x][y].ch = c;
	tiles[x][y].redraw = true;
}

int Console::ReadChar(int x, int y)
{
	return tiles[x][y].ch;
}

void Console::PutChar(int x, int y, int c)
{
	tiles[x][y].ch = c;
	tiles[x][y].redraw = true;
	tiles[x][y].backgroundColor = backgroundColor;
	tiles[x][y].foregroundColor = foregroundColor;
}

void Console::PutChar(int x, int y, int c, const Color characterColor, ALLEGRO_BITMAP *texture)
{
	tiles[x][y].ch = c;
	tiles[x][y].redraw = true;
	tiles[x][y].texture = texture;
	tiles[x][y].backgroundColor = backgroundColor;
	tiles[x][y].foregroundColor = characterColor;
}

#pragma warning(push)
#pragma warning(disable: 4996)

int GetCountOfControlKeysInString(const std::string &str)
{
	int count = 0;
	size_t i = 0;
	unsigned char c;
	while(i < str.size())
	{
		c = str.c_str()[i];
		if(c > 0 && c <= 5) count++;
		i++;
	}

	return count;
}

/* Inserts newlines as needed, replaces tabs with four spaces, ignores control kes when counting line length, returns number of lines */
int PrepareStringForPrinting(std::string &unformatted, int lineWidthInChars, int maxLines)
{
	size_t tab = unformatted.find("\t", 0);
	while(tab != std::string::npos)
	{
		unformatted.replace(tab, 1, "     ");
		tab = unformatted.find("\t", tab+1);
	}

	int lines = 0;
	size_t c = 0;
	c += lineWidthInChars;
	std::string line;
	while(c < unformatted.size())
	{
		lines++;
		if(lines >= maxLines) break;
		line = unformatted.substr(c-lineWidthInChars, lineWidthInChars);
		int numOfControlKeys = GetCountOfControlKeysInString(line);
		while( (int)line.length() - numOfControlKeys < lineWidthInChars && line.length()+c-lineWidthInChars < unformatted.size())
		{
			line = unformatted.substr(c-lineWidthInChars, lineWidthInChars + numOfControlKeys);
			numOfControlKeys = GetCountOfControlKeysInString(line);
		}

		size_t newline = line.find("\n");
		if(newline == std::string::npos)
		{
			//No newline found, we need to add one where the last space is
			newline = line.find_last_of(" ");
			if(newline != std::string::npos)
			{
				//Make newline here
				unformatted.replace(c-lineWidthInChars+newline, 1, "\n");
				c = c-lineWidthInChars+newline+1;
			}
			else
			{
			

				//Force newline here
				unformatted.insert(c-lineWidthInChars+lineWidthInChars, "\n");
				c++;
			}
			
		}
		else
		{
			//Found newline here
			c = c-lineWidthInChars+newline+1;
		}
		c+=lineWidthInChars;

	}
	lines++;

	return lines;
}
/*
void Console::Print(int x, int y, const Message msg, bool preserveColors)
{
	Color oldBack = backgroundColor;
	Color oldFore = foregroundColor;
	
	if(y >= height) return;


	if(printAlignment == PRINT_CENTER)
	{
		x = msg.size()/2 + 1;
	}
	else if(printAlignment == PRINT_RIGHT)
	{
		x = x - msg.size();
	}
	//else left aligned, do nothing

	for(size_t idx = 0; idx < msg.size(); idx++)
	{
		if(x >= 0 && x < width)
		{
			unsigned char c = msg.c_str()[idx];
			if(c >= 32) //Printable
			{
				tiles[x][y].ch = (int)c;
				tiles[x][y].redraw = true;
				tiles[x][y].foregroundColor = foregroundColor;
				tiles[x][y].backgroundColor = backgroundColor;
				x++;
			}
			else //Possibly a control key
			{
				if(c <= TOTAL_COLOR_PAIRS) //change colors, no \0 would get this far
				{
					backgroundColor = colorPairs[c-1].back;
					foregroundColor = colorPairs[c-1].fore;
				}
				else if(c == DEFAULT_CONSOLE_COLORS)
				{
					foregroundColor = oldFore;
					backgroundColor = oldBack;
				}
				else if(c == 9) //Tab
				{
					x+=5;
				}
			}
		}
		else
		{
			if(x >= width)
			{
				break;
			}
			//else just keep going, eventually a character might be in view
			x++;
		}
	}

	if(preserveColors)
	{
		backgroundColor = oldBack;
		foregroundColor = oldFore;
	}

}

int Console::PrintRect(int x, int y, int w, int h, const std::string &unformatted, ...)
{
    int final_n, n = unformatted.size() * 2; /* reserve 2 times as much as the length of the fmt_str
    std::string str;
    std::unique_ptr<char[]> formatted;
    va_list ap;
    while(1) {
        formatted.reset(new char[n]); /* wrap the plain char array into the unique_ptr
        strcpy(&formatted[0], unformatted.c_str());
        va_start(ap, unformatted);
        final_n = vsnprintf(&formatted[0], n, unformatted.c_str(), ap);
        va_end(ap);
        if (final_n < 0 || final_n >= n)
            n += abs(final_n - n + 1);
        else
            break;
    }


	std::string formatted_str(formatted.get());

	PrintInternal(x, y, x+w, 0, formatted_str, true, true, true);
	return 0;
}*/
#pragma warning(pop)

void Console::DrawMap(Map &map, int mapSrcX, int mapSrcY, int conDestX, int conDestY)
{
/*	//Draw each map tile to each corresponding console tile until we run out of console or map tiles
	int x = 0, y = 0;

	while( (size_t)x+mapSrcX < map.tile.size() && (size_t)x+conDestX < tiles.size())
	{
		y = 0;
		while((size_t)y+mapSrcY < map.tile[x].size() && (size_t)y+conDestY < tiles[x].size())
		{
			int materialType = map.tile[x+mapSrcX][y+mapSrcY].materialTypeIndex;
			int variant = map.tile[x+mapSrcX][y+mapSrcY].variant;
			Material material = game.materials[materialType];

			auto tile = &tiles[x+conDestX][y+conDestY];
			
			tile->redraw = true;
			tile->ch = material.ch(variant);
			tile->backgroundColor = material.bgColor;
			tile->foregroundColor = material.fgColor;
			tile->texture = material.texture;			

			y++;
		}
		x++;
	}


	*/
}

int Console::PrintInternal(int minx, int miny, int maxx, int maxy, std::string &buffer, bool useForeground, bool useBackground, bool isMultipleLines)
{
	//if(y >= height) return 0; //No drawing is possible, so just quit now
	
	//bool isMultipleLines = (y2!=y);

	int numOfLines = 0;
	if(isMultipleLines)
	{ 
		//Adds newlines where needed, respecting the existing ones, color controls not counting
		numOfLines = PrepareStringForPrinting(buffer, maxx-minx, this->height-miny); 
	}
	else
	{
		//Ignore newline characters by removing them!
		std::remove(buffer.begin(), buffer.end(), '\n');
		numOfLines = 1;
	}

	int x=minx, y=miny;
	unsigned char c=0;
	while(y <= maxy)
	{
		/*
		//Find where the first character on this line goes
		if(printAlignment == PRINT_LEFT){ x=minx; {
		else if(printAlignment == PRINT_RIGHT)
		{
			//set it as far right as possible
			x=maxx-buffer.find_first_of('\n', c);
			//if the line is too long to fit inside the box we made (how?)
			if(x < minx) x = minx;
		}

		
		*/
		y++;
	}


	//if(printAlignment == PRINT_CENTER)
	//{
	//	x -= buffer.size()/2;
	//}
	//else if(printAlignment == PRINT_RIGHT)
	//{
	//x -= buffer.size()-1;
	//}

	//first and last character to draw
	//size_t startChar = x<0? 0-x: 0;
	//size_t endChar = buffer.size()+x> size_t(width)? 
	//								size_t(width-x)-1: 
	//								buffer.size();

	
	/*for(size_t c = startChar; c < endChar; c++)
	{
		tiles[x+c][y].ch = buffer.c_str()[c];
		tiles[x+c][y].redraw = true;
		if(useForeground) tiles[x+c][y].foregroundColor = foregroundColor;
		if(useBackground) tiles[x+c][y].backgroundColor = backgroundColor;
	}*/



	return numOfLines;
}




PRINT_ALIGNMENT Console::GetPrintAlignment()
{
	return printAlignment;
}

void Console::SetPrintAlignment(PRINT_ALIGNMENT alignment)
{
	printAlignment = alignment;
}