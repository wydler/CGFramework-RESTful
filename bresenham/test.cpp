#include "Pixelator.h"

void testRand() {
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

    p.writeBMP("rand.bmp");
}

void testRays() {
    Pixelator p( 512, 512, 64, 64 );

    const unsigned int w = p.getWidth();
    const unsigned int h = p.getHeight();
    //some test cases
    for(int y = 0; y < h; y += 8)
    {
        drawLineBresenham(p, w/2, h/2, w - 1, y, 255, 255, (255 * y) / h);
    }
    for(int x = 0; x < w; x += 8)
    {
        drawLineBresenham(p, w/2, h/2, x, h - 1, 0, (255 * x) / h, 255);
    }
    for(int y = 0; y < h; y += 8)
    {
        drawLineBresenham(p, w/2, h/2, 0, y, (255 * y) / h, 255, 255);
    }
    for(int x = 0; x < w; x += 8)
    {
        drawLineBresenham(p, w/2, h/2, x, 0, 255, (255 * x) / h, 0);
    }

    p.writeBMP("rays.bmp");
}

int main( int argc, char ** argv )
{
    testRand();
    testRays();
}