void testRand() {
    const unsigned int w = p.getWidth();
    const unsigned int h = p.getHeight();
    //some test cases
    drawLineDDA(1, 2, w, h, 255, 255, 0);
    drawLineDDA(1, 3, 4, h-2, 255, 0, 255);
    drawLineDDA(1, h/2, w, h/4, 0, 200, 0);
    drawLineDDA(0, h-2, w-2, 0, 0, 0, 255);

    drawLineDDA(20, 0, 20, 20, 0, 0, 255);
    drawLineDDA(20, 20, 40, 20, 0, 0, 255);
    drawLineDDA(40, 0, 40, 20, 0, 0, 255);

    drawLineDDA(25, 0, 25, 10, 0, 255, 255);
    drawLineDDA(25, 10, 35, 10, 0, 255, 255);
    drawLineDDA(35, 0, 35, 10, 0, 255, 255);

    floodFill4(6, 10, Color(), Color(155, 0, 0));
    floodFill4(16, 52, Color(), Color(155, 155, 155));
    floodFill4(56, 12, Color(), Color(155, 55, 55));
    floodFill4(30, 1, Color(), Color(5, 85, 5));
    floodFill4(50, 30, Color(), Color(127, 0, 255));
    floodFill8(30, 30, Color(), Color(85, 95, 85));
    floodFill8(21, 0, Color(), Color(255, 255, 0));

    p.writeBMP("rand.bmp");
}

int main()
{
    testRand();
}
