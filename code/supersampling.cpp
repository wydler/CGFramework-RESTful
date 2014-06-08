void testRand() {
    {
        std::vector<Coord> points;
        points.push_back(Coord( 1,  1));
        points.push_back(Coord(120,  20));
        points.push_back(Coord(80, 80));
        points.push_back(Coord(60, 60));
        drawConvexPolyAA(points, Color(255, 255, 255));
    }
    {
        std::vector<Coord> points;
        points.push_back(Coord( 5,  5));
        points.push_back(Coord(60,  2));
        points.push_back(Coord(40, 40));
        points.push_back(Coord(20, 60));
        drawConvexPolyAA(points, Color(0, 255, 255));
    }
    {
        std::vector<Coord> points;
        points.push_back(Coord(65,  5));
        points.push_back(Coord(120,  2));
        points.push_back(Coord(100, 100));
        points.push_back(Coord(120, 60));
        drawConvexPolyAA(points, Color(255, 0, 50));
    }

    drawAABuffer(p);

    p.writeBMP("rand.bmp");
}

int main()
{
    testRand();
}
