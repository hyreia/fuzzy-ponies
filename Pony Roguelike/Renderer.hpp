#ifndef RENDERER_HPP
#define RENDERER_HPP

class Renderer
{
public:
	Renderer();
	virtual void RenderWorld(WorldStage *world, int focusX, int focusY) = 0;
	virtual ~Renderer(){}
private:

};

#endif


class TileRenderer: public Renderer
{
public:
	TileRenderer();
	virtual void RenderWorld(WorldStage *world, int focusX, int focusY);

};