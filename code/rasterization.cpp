void testRand() {
    Pixelator p( 512, 512, 64, 64 );

    drawTriangle(p
        ,  0,  5,  255,   0,   0
        ,  55, 1,    0, 255,   0
        ,  60, 60,   0,   0, 255);

    p.writeBMP("rand.bmp");
}

int main()
{
    testRand();
}
