#ifndef GAME_HPP
#define GAME_HPP

struct ALLEGRO_DISPLAY;
struct ALLEGRO_EVENT_QUEUE;
#include "main.hpp"
#include <list>

class Game
{
public:
	Game();
	~Game();
	void Init();
	void Run();

	KeyEvent GetLastKey();

	

	//No get, if you need to ask you should just do it
	void SetPrintAlignment(int alignment);
	//Returns the y of where a next line would be printed
	int Print(int x, int y, Message message);
	//Returns the y of where a next line would be printed
	int PrintRect(int x, int y, Message message, int width, int beginHeight=0, int maxHeight=0, bool draw = true);

	void QueueMessage(Message message);

	int GetWindowWidth();
	int GetWindowHeight();
	int GetTileSize();

	ImageAssets imageAssets;
	WorldStage world;

private:
	int windowWidth, windowHeight;
	//Actor *playerActor; 
	//Map *currentMap;

	Renderer *renderer;
	
	void SetWindowTitle(const char *title);
	inline void DrawCurrentMap(int xOffset, int yOffset) const;
	inline void DrawCurrentMapActors(int xOffset, int yOffset) const;
	//inline void DrawCurrentMapItems(int xOffset, int yOffset) const;
	inline void DrawMessageQueue();
	void Flush();
	bool isDrawingItems, isDrawingActors;

	bool IsWindowClosed();
	bool isWindowClosed;

	//Allegro methods
	void InitAllegro();
	void ShutdownAllegro();
	void CheckForEvents();

	//Allegro members
	ALLEGRO_EVENT_QUEUE *event_queue;
	ALLEGRO_DISPLAY *display;
	ALLEGRO_TIMER *timer;
	ALLEGRO_FONT *font, *tileFont;
	KeyEvent lastKey;
	MouseState mouseState;

	std::list<Message> messageQueue;
	int printAlignment;
};

extern std::unique_ptr<Game> game;


#endif