#include "Pixelator.h"

#include <stdlib.h>
#include <stdio.h>
#include <iostream>

void drawLineBresenham(Pixelator &p, unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1, unsigned char r, unsigned char g, unsigned char b)
{
	// Uses the bresenham algorithm to draw a line in the given color

	// Version mit Punkten vertauschen!

	int tmp = 0;

	// In folgenden drei Fällen sollen der Start- und Endepunkt vertauscht werden:
	// 1.) Die Linie geht von oben rechts nach unten links.
	// 2.) Die Linie geht von unten rechts nach oben links.
	// 3.) Die Linie geht von oben nach unten.
	if (x0 > x1){
		tmp = x0;
		x0 = x1;
		x1 = tmp;

		tmp = y0;
		y0 = y1;
		y1 = tmp;
	}

	else if (x0 == x1){
		if (y0 > y1){
			tmp = y0;
			y0 = y1;
			y1 = tmp;
		}
	}

	// Dank dem Vertauschen der Start- und Endepunkte ist deltaX ist immer positiv.
	int  deltaX = 0;
	deltaX = (x1 - x0);

	// Dank dem Vertauschen der Start- und Endepunkte ist deltaY bei vertikalen Linien immer positiv.
	// Bei schrägen Linien kann deltaY positiv und negativ sein.
	// Wenn deltaY positiv ist, ist die Steigung positiv.
	// Wenn deltaY negativ ist, ist die Steigung negativ.
	int deltaY = 0;
	deltaY = (y1 - y0);

	// Der Absolutwert von deltaY ist positiv
	int absolutwertDeltaY = 0;

	if (deltaY < 0){
		absolutwertDeltaY = deltaY * (-1);
	}
	else{
		absolutwertDeltaY = deltaY;
	}

	int pk = 0;
	pk = (2 * absolutwertDeltaY) - deltaX;

	int xk = 0;
	xk = x0;

	int yk = 0;
	yk = y0;

	// draw start and end point of the line
	p.setPixel( x0, y0, r, g, b );
	p.setPixel( x1, y1, r, g, b );

	// Linie geht von links unten nach rechts oben
	if (x0 < x1 && y0 < y1){

		// Steigung ist groesser als 1
		if (absolutwertDeltaY > deltaX){
			for (yk; yk < (y1 - 1); yk++)
			{
				if (pk <= 0 && yk < (y1 - 1))
				{
					p.setPixel(xk+1, yk+1, r, g, b);
					pk = pk + (2 * absolutwertDeltaY);
					xk = xk + 1;
				}

				else if (pk > 0 && yk < (y1 - 1))
				{
					p.setPixel(xk, yk+1, r, g, b);
					pk = pk + (-2 * absolutwertDeltaY) - (2 * deltaX);
				}
			}
		}

		// Steigung ist groesser als 0 und kleiner als 1
		else if (absolutwertDeltaY < deltaX){
			for (xk; xk < (x1 - 1); xk++)
			{
				if (pk <= 0 && xk < (x1 - 1))
				{
					p.setPixel(xk+1, yk, r , g, b);
					pk = pk + (2 * absolutwertDeltaY);
				}

				else if (pk > 0 && xk < (x1 - 1))
				{
					p.setPixel(xk+1, yk+1, r, g, b);
					pk = pk + (2 * absolutwertDeltaY) - (2 * deltaX);
					yk = yk + 1;
				}
			}
		}

		// Steigung ist genau 1
		else if (absolutwertDeltaY == deltaX){
			for (xk; xk < (x1 - 1); xk++)
			{
				p.setPixel(xk+1, yk+1, r, g, b);
				yk = yk + 1;
			}
		}
	}

	// Linie geht von links oben nach rechts unten
	else if (x0 < x1 && y0 > y1){

		// Steigung ist groesser als 1
		if (absolutwertDeltaY > deltaX){
			for (yk; yk > (y1 + 1); yk--)
			{
				if (pk <= 0 && yk > (y1 + 1))
				{
					p.setPixel(xk, yk-1, r, g, b);
					pk = pk + (2 * absolutwertDeltaY);
				}

				else if (pk > 0 && yk > (y1 + 1))
				{
					p.setPixel(xk+1, yk-1, r, g, b);
					pk = pk + (2 * absolutwertDeltaY) - (2 * deltaX);
					xk = xk + 1;
				}
			}
		}

		// Steigung ist groesser als 0 und kleiner als 1
		else if (absolutwertDeltaY < deltaX){
			for (xk; xk < (x1 - 1); xk++)
			{
				if (pk <= 0 && xk < (x1 - 1))
				{
					p.setPixel(xk+1, yk-1, r , g, b);
					pk = pk + (2 * absolutwertDeltaY);
					yk = yk - 1;
				}

				else if (pk > 0 && xk < (x1 - 1))
				{
					p.setPixel(xk+1, yk, r, g, b);
					pk = pk + (2 * absolutwertDeltaY) - (2 * deltaX);
				}
			}
		}

		// Steigung ist genau 1
		else if (absolutwertDeltaY == deltaX){
			for (xk; xk < (x1 - 1); xk++)
			{
				p.setPixel(xk+1, yk-1, r , g, b);
				yk = yk - 1;
			}
		}
	}

	// Linie geht von links nach rechts
	else if (x0 == x1 && y0 < y1){
		for (yk; yk < y1; yk++){
			p.setPixel(xk, yk, r, g, b);
		}
	}

	// Linie geht von unten nach oben
	else if (x0 < x1 && y0 == y1){
		for (xk; xk < x1; xk++){
			p.setPixel(xk, yk, r, g, b);
		}
	}
}

int main( int argc, char ** argv )
{
	//windowWidth, windowHeight, resolution in pixels
	Pixelator p(512, 512, 64, 64);

	//some test cases
	drawLineBresenham(p,  0,  0, 10, 10, 255, 255, 255);  
	drawLineBresenham(p, 20, 20, 12, 13, 255,   0, 255);  
	drawLineBresenham(p,  0, 15,  3, 11, 255,   0,   0);  
	drawLineBresenham(p,  9,  5,  9,  5,   0, 255,   0);  
	drawLineBresenham(p, 30,  5, 25,  6,   0,   0, 255);  
	drawLineBresenham(p, 40,  2, 40, 22,   0, 255, 255);  
	drawLineBresenham(p, 10,  2, 35,  2, 255, 255,   0);  

	// some random lines to a random bluish redish color
	int i = 0;

	for( i = 0; i < 100; i++ ){
	drawLineBresenham(p, rand()%p.getCanvasWidth(), p.getCanvasHeight() / 2 + rand()%(2 * p.getCanvasHeight()), rand()%p.getCanvasWidth(), p.getCanvasHeight() / 2 + rand()%(2 * p.getCanvasHeight()), 100, 0, rand()%256 );
	}

	p.wait();
}