void testRand() {
    Pixelator p( 512, 512, 64, 64 );

    drawTriangle(p
        ,  10,  60,  255, 255,   0
        ,  55,  10,    0, 255, 255
        ,  60,  50,  255,   0, 255);

    p.writeBMP("rand.bmp");
}

int main()
{
    testRand();
}
