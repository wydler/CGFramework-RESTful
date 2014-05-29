#include "Pixelator.h"

#include <stdlib.h>

#define MIN( x, y ) ((x) <= (y) ? (x) :  (y))
#define MAX( x, y ) ((x) >= (y) ? (x) :  (y))
#define ABS( x )    ((x) >= 0.0 ? (x) : -(x))

void drawLineBresenham(Pixelator &p, unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1, unsigned char r, unsigned char g, unsigned char b)
{
	//todo: Use the bresenham algorithm to draw a line in the given color
	//use p.setPixel to draw a singel pixel

	//draw start and end point of the line


	int delta_x = x1 - x0;
	int decision_parameter = 0;
	// if x0 == x1, then it does not matter what we set here
	int const inc_X = ((delta_x > 0) - (delta_x < 0)); // If x is positive increment x, else decrement

	// right shift to get "2 delta x"
	delta_x = ABS(delta_x) << 1;

	int delta_y = y1 - y0;
	// if y0 == y1, then it does not matter what we set here
	int const inc_Y = ((delta_y > 0) - (delta_y < 0)); // If y is positive increment x, else decrement

	// right shift to get "2 delta y"
	delta_y = ABS(delta_y) << 1;

	p.setPixel( x0, y0, r, g, b );
	p.setPixel( x1, y1, r, g, b );

	if (delta_x >= delta_y)
	{
		// decision parameter may go below zero
		decision_parameter = delta_y - (delta_x >> 1); // left shift to get "delta x"

		int steps = abs((long)x1-(long)x0);
		for(int i = 0; i < steps; i++)
		{
			if ((decision_parameter >= 0) && (decision_parameter || (inc_X > 0)))
			{
				decision_parameter -= delta_x;
				y0 += inc_Y;
			}
			// else do nothing

			decision_parameter += delta_y;
			x0 += inc_X;

			p.setPixel( x0, y0, r, g, b );
			p.setPixel( x1, y1, r, g, b );
		}
	}
	else
	{
		// decision parameter may go below zero
		decision_parameter = delta_x - (delta_y >> 1); // left shift to get "delta y"

		int steps = abs((long)y1-(long)y0);
		for(int i = 0; i < steps; i++)
		{
			if ((decision_parameter >= 0) && (decision_parameter || (inc_Y > 0)))
			{
				decision_parameter -= delta_y;
				x0 += inc_X;
			}
			// else do nothing

			decision_parameter += delta_x;
			y0 += inc_Y;

			p.setPixel( x0, y0, r, g, b );
			p.setPixel( x1, y1, r, g, b );
		}
	}

	// Only if x0==x1
	if(delta_x == 0 && delta_y !=0){
		int steps = y1-y0;
		if(steps < 0){
			for(int i = steps; i < 0; i++){
				y0--;
				p.setPixel( x0, y0, r, g, b );
				p.setPixel( x1, y1, r, g, b );
			}
		}
		else
		{
			for(int i = 0; i < steps; i++){
				y0++;
				p.setPixel( x0, y0, r, g, b );
				p.setPixel( x1, y1, r, g, b );
			}
		}

	}

	//p.setPixel( x0, y0, r, g, b );

}

int main( int argc, char ** argv )
{
	//windowWidth, windowHeight, resolution in pixels
	Pixelator p( 512, 512, 64, 64 );

	//some test cases
	drawLineBresenham(p,  0,  0, 10, 10, 255, 255, 255);
	drawLineBresenham(p, 20, 20, 12, 13, 255, 0, 255);
	drawLineBresenham(p,  0, 15,  3, 11, 255, 0, 0);
	drawLineBresenham(p,  9,  5,  9,  5, 0, 255, 0);
	drawLineBresenham(p, 30,  5, 25,  6, 0, 0, 255);
	drawLineBresenham(p, 40,  2, 40, 22, 0, 255, 255);
	drawLineBresenham(p, 10,  2, 35,  2, 255, 255, 0);

	// some random lines to a random bluish redish color 
	for( int i = 0; i < 100; i++ )
		drawLineBresenham(p
		, rand()%p.getCanvasWidth(), p.getCanvasHeight() / 2 + rand()%(2 * p.getCanvasHeight())
		, rand()%p.getCanvasWidth(), p.getCanvasHeight() / 2 + rand()%(2 * p.getCanvasHeight())
		, 100, 0, rand()%256 );

	p.wait();
}
