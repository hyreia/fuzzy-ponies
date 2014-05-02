#ifndef RENDERER_HPP
#define RENDERER_HPP

class Renderer
{
public:
	Renderer();
	virtual void RenderWorld(WorldStage *world, int focusX, int focusY, Map *currentMap) = 0;
	virtual ~Renderer(){}
private:

};

#endif


class TileRenderer: public Renderer
{
public:
	TileRenderer(ImageAssets *assets, 

};