#include "main.hpp"
#include "Random.hpp"

#include <list>


std::unique_ptr<Game> game(nullptr);

static const int CON_WIDTH = 40;
static const int CON_HEIGHT = 25;
static const int TILE_WIDTH =  32;
static const int TILE_HEIGHT = 32;

Game::Game():
isWindowClosed(false), windowWidth(CON_WIDTH*TILE_WIDTH), windowHeight(CON_HEIGHT*TILE_HEIGHT),
isDrawingActors(true), isDrawingItems(true), printAlignment(ALLEGRO_ALIGN_LEFT)
{}

Game::~Game(){}

void Game::Init()
{
	rng::Seed();

	InitAllegro();
	SetWindowTitle("Pony Roguelike Development: The @ Demo 2: Electric Scootaloo: Long Title Edition");	

	Console::SetGlobalConsoleTileSize(TILE_WIDTH, TILE_HEIGHT, font);

	imageAssets.LoadImagesFromFile();
	actorList.LoadActorsFromFile();

	materials.push_back(Material("grass", "a patch of grass", '.', ',', ',', '.', 
		Color::Black, Color::DarkGreen, false, GRASS_TILE_IMAGE, 0));

	materials.push_back(Material("stone wall", "a rough stone wall", '#', '#', '#', '#',
		Color::DarkGrey, Color::Grey, true, STONE_TILE_IMAGE, 0));

}

KeyEvent Game::GetLastKey()
{
	auto ret = lastKey;
	lastKey.code = 0;
	lastKey.ch = -1;

	return ret;
}

void Game::Run()
{

	//Putting map here for now
	Map map;
	map.InitToField(CON_WIDTH*2, CON_HEIGHT*2, 0);
	
	const int ARBITRARY_NUMBER_OF_WALLS = 20;
	for(int nWalls = 0; nWalls < ARBITRARY_NUMBER_OF_WALLS; nWalls++)
	{
		//Manually place down some walls to run into
		int x = rng::Rand(CON_WIDTH*2);
		int y = rng::Rand(CON_HEIGHT*2);
		map.tile[x][y].materialTypeIndex = 1;
	}

	//Don't spawn MarkerPony inside a wall
	if(map.tile[20][15].materialTypeIndex == 1)
	{
		map.tile[20][15].materialTypeIndex = 0;
	}
	currentMap = &map;

	
	//Ad hoc message queue
	//std::list<Message> messageQueue;
	bool isTimeToRender=true;
	std::string startMessage = "Move around with the arrow keys or numpad!";
	
	//messageQueue.push_back( startMessage );
	playerActor = new Actor(20, 15, '@', Color::Pink, MARKER_PONY_IMAGE);
	map.actors.push_back(playerActor);
	
	//root->DrawMap(map, 0, 0, 0, 0);

	//Run the logic here for now
	while(!IsWindowClosed())
	{
		//OUTPUT
		if(isTimeToRender)
		{
			isTimeToRender = false;
			Flush();
		}		
		

		//INPUT
		CheckForEvents();
		auto key = GetLastKey();
		//Lazy raw key checking for now
		int dx=0,dy=0;
		switch(key.code)
		{
			case ALLEGRO_KEY_UP:
			case ALLEGRO_KEY_PAD_8:
				dy--;
				break;
			case ALLEGRO_KEY_PAD_4:
			case ALLEGRO_KEY_LEFT:
				dx--;
				break;
			case ALLEGRO_KEY_PAD_6:
			case ALLEGRO_KEY_RIGHT:
				dx++;
				break;
			case ALLEGRO_KEY_PAD_2:
			case ALLEGRO_KEY_DOWN: 
				dy++;
				break;
			case ALLEGRO_KEY_PAD_9:	dx++; dy--; break;
			case ALLEGRO_KEY_PAD_7: dx--; dy--; break;
			case ALLEGRO_KEY_PAD_1: dx--; dy++; break;
			case ALLEGRO_KEY_PAD_3: dx++; dy++; break;
			case ALLEGRO_KEY_B:
				{
					messageQueue.push_back( Message(Color::Yellow, "BOOP @ %d %d!", playerActor->x, playerActor->y) );
					isTimeToRender = true;
					break;
				}
			case ALLEGRO_KEY_P:
				{
					Message poem(Color::White, "One find boop in the middle of the night, two dead pones got up to fight. Back to back they faced each other.");
					messageQueue.push_back( poem );
					isTimeToRender = true;
				}
			default:break;
		}

		
		//PROCESSING
		//Check for collisions
		if(dx != 0 || dy != 0)
		{
			//In range
			if(playerActor->x+dx < (int)map.tile.size() && playerActor->y+dy < (int)map.tile[0].size() &&
				playerActor->x+dx >= 0 && playerActor->y+dy >= 0)
			{
				int materialIndex = map.tile[playerActor->x+dx][playerActor->y+dy].materialTypeIndex;
				int materialVariant = map.tile[playerActor->x+dx][playerActor->y+dy].variant;
				if(!materials[materialIndex].isBlocking)
				{
					playerActor->x += dx;
					playerActor->y += dy;
					//Message hereis(Color::White, "You see %s here.", materials[materialIndex].description.c_str());
					//messageQueue.push_back(hereis);
					isTimeToRender=true;
				}
				else
				{
					if(rng::IsRandBoolTrue())
					{
						//Message msg("Please don't boop the %s!", materials[materialIndex].name.c_str());
						//messageQueue.push_back(msg);
					}
					//else messageQueue.push_back( Message("Stop. Those were expensive.") );

					
				}
			}
			isTimeToRender = true;
		}

		al_rest(0.0005);
	}

	
	imageAssets.FreeImages();
	ShutdownAllegro();
	
}

void Game::Flush()
{
	al_clear_to_color(al_map_rgb(0, 0, 0));

	//Draw a map if there's a map

	int drawX = playerActor->x - CON_WIDTH/2;
	int drawY = playerActor->y - CON_HEIGHT/2;

	if(drawX < 0) drawX=0;
	if(drawY < 0) drawY=0;
	if(drawX+CON_WIDTH  >  (int)currentMap->tile.size()) drawX = (int)currentMap->tile.size()-CON_WIDTH;
	if(drawY+CON_HEIGHT >  (int)currentMap->tile[0].size()) drawY = (int)currentMap->tile[0].size()-CON_HEIGHT;

	if(currentMap)
	{
		DrawCurrentMap(drawX, drawY);
		DrawCurrentMapActors(drawX, drawY);
	}

	//If there's a menu draw that

	//Else there's not a menu, draw the GUI
		//Which consists of immediate stats and message queue

		DrawMessageQueue();

	al_flip_display();
}

void Game::InitAllegro()
{
	//Start allegro systems
	al_init();
	al_install_keyboard();
	al_install_mouse();
	al_init_image_addon();
	al_init_font_addon();
	al_init_ttf_addon();
	al_init_primitives_addon();

	al_set_new_display_flags(ALLEGRO_RESIZABLE);

	//Create allegro components
	event_queue = al_create_event_queue();
	display = al_create_display(windowWidth, windowHeight);

	font = al_load_ttf_font("Courier Prime Bold.ttf", 18, 0);
	tileFont = al_load_ttf_font("Courier Prime Bold.ttf", TILE_WIDTH+4, 0);
	
	//register event sources with the event queue
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_mouse_event_source());
	al_register_event_source(event_queue, al_get_display_event_source(display));
}

void Game::ShutdownAllegro()
{
	al_destroy_event_queue(event_queue);
	al_destroy_font(font);
	al_destroy_display(display);

}

int Game::GetWindowHeight(){ return windowHeight; }
int Game::GetWindowWidth(){ return windowWidth; }
int Game::GetTileSize(){ return TILE_WIDTH; }

void Game::SetWindowTitle(const char *title)
{
	al_set_window_title(display, title);
}

bool Game::IsWindowClosed()
{
	return isWindowClosed;
}

void Game::CheckForEvents()
{
	ALLEGRO_EVENT ev;
	//al_get_next_event(event_queue, &ev);
	while(al_get_next_event(event_queue, &ev))
	{
		switch(ev.type)
		{
		//Keyboard
		case ALLEGRO_EVENT_KEY_UP:
			lastKey.ch=-1;
			lastKey.code=0;
			break;
		case ALLEGRO_EVENT_KEY_DOWN:
			lastKey.code = ev.keyboard.keycode;
			break;
		case ALLEGRO_EVENT_KEY_CHAR:
			if(ev.keyboard.repeat)
			{
				lastKey.code = ev.keyboard.keycode;
				lastKey.ch = ev.keyboard.unichar;
			}
			break;
		//Mouse
		case ALLEGRO_EVENT_MOUSE_AXES:
		case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
		case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
		case ALLEGRO_EVENT_MOUSE_WARPED:
			mouseState.UpdateState(ev);
			break;
		//Display
		case ALLEGRO_EVENT_DISPLAY_CLOSE:
			isWindowClosed=true;
			break;
		case ALLEGRO_EVENT_DISPLAY_RESIZE:
			//al_acknowledge_resize(display);
			break;
		default:break;
		}
	} //while(al_get_next_event(event_queue, &ev));
}

void Game::QueueMessage(Message message)
{
	messageQueue.push_back(message);
}

int Game::Print(int x, int y, Message message)
{
	al_draw_text(font, message.color, x, y, printAlignment, message.c_str());
	return y+al_get_font_line_height(font);
}

void Game::DrawCurrentMap(int xOffset, int yOffset) const
{
	Color tint[4];
	tint[0] = Color(255, 255, 255, 255);
	tint[1] = Color(245, 245, 245, 255);
	tint[2] = Color(235, 235, 235, 255);
	tint[3] = Color(225, 225, 225, 255);

	int tileSize = game->GetTileSize();
	int width = game->GetWindowWidth() / tileSize;
	int height = game->GetWindowHeight() / tileSize;

	for(int x = 0; x < width && xOffset+x < (int)currentMap->tile.size(); x++)
	{
		for(int y = 0; y < height && yOffset+y < (int)currentMap->tile[0].size(); y++)
		{
			auto m = materials[currentMap->tile[xOffset+x][yOffset+y].materialTypeIndex];
			auto variant = currentMap->tile[xOffset+x][yOffset+y].variant;
			auto texture = imageAssets.GetTileImage(m.image);
			if(texture)
			{
				al_draw_tinted_bitmap(texture, 
					tint[currentMap->tile[xOffset+x][yOffset+y].variant ], x*tileSize, y*tileSize, 0);
			}
		}
	}
}

void Game::DrawCurrentMapActors(int xOffset, int yOffset) const
{
	for(auto actor = currentMap->actors.begin(); actor != currentMap->actors.end(); actor++)
	{
		auto texture = imageAssets.GetActorImage((*actor)->image);
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
		}
	}
}

void Game::DrawMessageQueue()
{

	//Where the queue is
	int msgQueueWidth = windowWidth/2;
	int msgQueueHeight = 200;
	int MsgQueuePosX = msgQueueWidth; //for readability
	int MsgQueuePosY = windowHeight - msgQueueHeight;

	


	int messageQueueHeightLeft = msgQueueHeight;
	auto iter = messageQueue.rbegin();
	int lineHeight = al_get_font_line_height(font);
	while(messageQueueHeightLeft > 0 && iter != messageQueue.rend())
	{
		//How tall the full message is
		int messageHeight = PrintRect(0, 0, (*iter), msgQueueWidth, 0, 0, false);


		//Where said message would be drawn if drawn in its entirety
		int messagePosition = MsgQueuePosY + messageQueueHeightLeft - messageHeight;

		//Subtract message height from message queue height left? We may need the original position
		//messageQueueHeightLeft -= messageHeight;
		
		//negative beginHeights behave like 0 in PrintRect
		int beginHeight = messageHeight - messageQueueHeightLeft;

		PrintRect(msgQueueWidth, 
			messagePosition, 
			(*iter), 
			msgQueueWidth, 
		beginHeight);
		iter++;

		messageQueueHeightLeft -= messageHeight;
	}



}

#pragma warning(push)
#pragma warning(disable: 4996) //vsprintf is unsafe and depricated but vsprintf_s is nonstandard
int Game::PrintRect(int x, int y, Message message, int width, int beginHeight, int maxHeight, bool draw)
{
     char stext[2048]; // Copy of the passed text.


     char * pch; // A pointer to each word.
     char word[255]; // A string containing the word (for convienence)
     char breakchar[12]; // Contains the break line character "\n "
     char Lines[40][1024]; // A lovely array of strings to hold all the lines (40 max atm)
     char TempLine[1024]; // Holds the string data of the current line only.
     int CurrentLine = 0; // Counts which line we are currently using.
     int q; // Used for loops

     // Setup our strings
     strncpy(stext, message.c_str(), 2048);
     strncpy(breakchar,"\n ", 12);
     strncpy(TempLine,"", 1024);
     for(q = 0;q < 40;q+=1)
     {
          sprintf(Lines[q],"");
     }
     //-------------------- Code Begins

     pch = strtok (stext," ");                               // Get the first word.
     do
     {
          strcpy(word,"");                                  // Truncate the string, to ensure there's no crazy stuff in there from memory.
          sprintf(word,"%s ",pch);
          sprintf(TempLine,"%s%s",TempLine,word);             // Append the word to the end of TempLine
          // This code checks for the new line character.
          if(strcmp(word,breakchar) == 0)
          {
                CurrentLine+=1;                                 // Move down a Line
                strcpy(TempLine,"");                            // Clear the tempstring
          }
          else
          {
                if(al_get_text_width(font, TempLine) >= (width))   // Check if text is larger than the area.
                {
                      strcpy(TempLine,word);                      // clear the templine and add the word to it.
                      CurrentLine+=1;                             // Move to the next line.
                }
                if(CurrentLine < 40)
                {
                      strcat(Lines[CurrentLine],word);                // Append the word to whatever line we are currently on.
                }
          }
          pch = strtok (NULL, " ");                           // Get the next word.
     }while (pch != NULL);
     // ---------------------------------- Time to draw.
     if(draw == true)                                       //Check whether we are actually drawing the text.
     {
          for(q = 0;q <=CurrentLine;q+=1)                     // Move through each line and draw according to the passed flags.
          {
				if(	q*al_get_font_line_height(font) >= beginHeight)
				{
					if(printAlignment == ALLEGRO_ALIGN_LEFT)
						 al_draw_text(font, message.color, x, y + (q * al_get_font_line_height(font)), ALLEGRO_ALIGN_LEFT,Lines[q]);
					if(printAlignment == ALLEGRO_ALIGN_CENTRE)
						 al_draw_text(font, message.color, x + (width/2), y + (q * al_get_font_line_height(font)), ALLEGRO_ALIGN_CENTRE,Lines[q]);
					if(printAlignment == ALLEGRO_ALIGN_RIGHT)
						 al_draw_text(font,message.color, x + width, y + (q * al_get_font_line_height(font)), ALLEGRO_ALIGN_RIGHT,Lines[q]);
				}
				if( (maxHeight != 0 && CurrentLine+1)*al_get_font_line_height(font) > maxHeight) break;
          }

     }
     return((CurrentLine+1) * al_get_font_line_height(font));  // Return the actual height of the text in pixels.	
}
#pragma warning(pop)