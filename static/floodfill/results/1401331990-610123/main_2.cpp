#include "Pixelator.h"

#include <stdlib.h>

class Color { 
public:
	unsigned char r, g, b; 
	Color(): r(0), g(0), b(0) { } 
	Color(unsigned char r_, unsigned char g_, unsigned char b_): r(r_), g(g_), b(b_) { } 
	unsigned char& operator[](int index) { return 0 == index ? r : 1 == index ? g : b; }
	bool operator==(const Color& c) const { return c.r == r && c.g == g && c.b == b; }
	bool operator!=(const Color& c) const { return !operator==(c); }
};

const int resolution = 64;
//windowWidth, windowHeight, resolution in pixels
Pixelator p( 512, 512, resolution, resolution );

void drawLineDDA(int x0, int y0, int x1, int y1, unsigned char r, unsigned char g, unsigned char b)
{
	float dx = x1-x0;
	float dy = y1-y0;
	int steps = abs(dy);
	if(abs(dx) > abs(dy))
		steps=abs(dx);

	float x_inc = dx / (float)steps;
	float y_inc = dy / (float)steps;

	float x = x0;
	float y = y0;

	p.setPixel(x, y, r, g, b);

	for(int count = 1; count <= steps; ++count)
	{
		x += x_inc;
		y += y_inc;

		p.setPixel((int)(x+0.5), (int)(y+0.5), r, g, b);
	}
}

void floodFill4(int x, int y, const Color& oldColor, const Color& newColor)
{
	//todo: implement flood fill for 4-connected areas
	unsigned char backgroundR, backgroundG, backgroundB;

	//returns if pixel is outside frame-buffer
	//otherwise returns in parameters r,g,b the color components
	if(!p.getPixel(x, y, backgroundR, backgroundG, backgroundB)) 
    return;
  else
  {
    if(backgroundR == oldColor.r && backgroundG == oldColor.g && backgroundB == oldColor.b)
    {
      p.setPixel(x,y, newColor.r, newColor.g, newColor.b);

      floodFill4(x, y + 1, oldColor, newColor); // unten
      floodFill4(x, y - 1, oldColor, newColor); // oben
      floodFill4(x - 1, y, oldColor, newColor); // links 
      floodFill4(x + 1, y, oldColor, newColor); // rechts
    }
    return;
  }
}

void floodFill8(int x, int y, const Color& oldColor, const Color& newColor)
{
	//todo: implement flood fill for 8-connected areas
	unsigned char backgroundR, backgroundG, backgroundB;

	//returns if pixel is outside frame-buffer
	//otherwise returns in parameters r,g,b the color components
	if(!p.getPixel(x, y, backgroundR, backgroundG, backgroundB)) 
    return;

  else
  {
    if(backgroundR == oldColor.r && backgroundG == oldColor.g && backgroundB == oldColor.b)
    {
      p.setPixel(x,y, newColor.r, newColor.g, newColor.b);

      floodFill8(x, y + 1, oldColor, newColor);    //down
      floodFill8(x, y - 1, oldColor, newColor);    //up
      floodFill8(x + 1, y, oldColor, newColor);    //right
      floodFill8(x - 1, y, oldColor, newColor);    //left
      floodFill8(x + 1, y + 1, oldColor, newColor);//down-right
      floodFill8(x + 1, y - 1, oldColor, newColor);//up-right
      floodFill8(x - 1, y + 1, oldColor, newColor);//down-left
      floodFill8(x - 1, y - 1, oldColor, newColor);//up-right
    }
    return;
  }
}

int main( int argc, char ** argv )
{
	const unsigned int w = p.getWidth();
	const unsigned int h = p.getHeight();
	//some test cases
	drawLineDDA(1, 2, w, h, 255, 255, 0);
	drawLineDDA(1, 3, 4, h-2, 255, 0, 255);
	drawLineDDA(1, h/2, w, h/4, 0, 200, 0);
	drawLineDDA(0, h-2, w-2, 0, 0, 0, 255);

	floodFill4(6, 10, Color(), Color(155, 0, 0));
	floodFill4(16, 52, Color(), Color(155, 155, 155));
	floodFill4(56, 12, Color(), Color(155, 55, 55));
	floodFill4(30, 1, Color(), Color(5, 85, 5));
	floodFill8(30, 30, Color(), Color(85, 95, 85));

	p.wait();
}
