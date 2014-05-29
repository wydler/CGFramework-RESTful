void testRand() {
    Pixelator p( 512, 512, 64, 64 );
    //initiate z-buffer
    for(int x = 0; x < 64; ++x)
    {
        for(int y = 0; y < 64; ++y)
        {
            zBuffer[x][y] = -1;
        }
    }

    drawTriangle(p, 10, 10, 30,  40, 3, 30,  30, 25, 30,    0, 255, 0);
    drawTriangle(p,  5,  5, 26,  45, 0,  1,  45, 60,  1,  255, 0, 255);
    drawTriangle(p,  0,  5, 10,  50, 5, 10,  50, 50, 10,  255, 255, 255);

    p.writeBMP("rand.bmp");
}

int main()
{
    testRand();
}
