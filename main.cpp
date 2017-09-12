#include <iostream>
#include "database.h"
#include "maths.h"
#include "curves.h"
//#include "beatmap.h"
#include "utils.h"
#include "beatmap_file.h"

using namespace osupp;

void test() {
    BeatmapFile file(R"(D:\Developent\CPP\osupp\test.txt)", BeatmapFile::Mode::Read);
    auto section = file.readSection("[Test]");

    for(auto &line : section) {
        line = "hello " + line;
        std::cout << line << std::endl;
    }

    file.setMode(BeatmapFile::Mode::Write);
    file.writeSection("[Test]", section);
}

int main() {
    std::ios_base::sync_with_stdio(false);
    test();
    return 0;

    auto bm = BeatmapFilez::read("F:\\Osu\\Songs\\407484 Demetori - Gensou, Yume no Owari\\Demetori - Gensou, Yume no Owari (GoldenWolf) [Genjitsu no Hajimari].osu");
    //std::cout << bm.id << " " << bm.artist << " - " << bm.title << std::endl;
    std::cout << bm.timingpoints.size() << std::endl;
    std::cout << bm.hitobjects.size() << std::endl;

    OsuDB reader("F:\\Osu");
    reader.read();
    /*for(auto tp : reader.entries[0]->timingpoints) {
        std::cout << "TP: " << tp->offset << std::endl;
    }*/
    //Coordinate points[] = {Coordinate(194, 96), Coordinate(187, 68), Coordinate(213, 26), Coordinate(213, 26), Coordinate(105, 151)};
   /* Coordinate points[] = {Coordinate(251, 358), Coordinate(250, 324), Coordinate(274, 301), Coordinate(274, 301),
                           Coordinate(270, 243)};
    //Coordinate points[] = {Coordinate(111, 108), Coordinate(81, 108), Coordinate(51, 104)};
    //Coordinate points[] = {Coordinate(111, 90), Coordinate(81, 108), Coordinate(51, 98), Coordinate(61, 150)};
    std::vector<Coordinate> vecPt(points, points + sizeof(points) / sizeof(points[0]));

    Curve curve(CurveType::Bezier, vecPt, 115.50000352478);
    for (float t = 0; t <= 1.05; t += 0.1) {
        Coordinate point = curve.positionAt(t);
        std::cout << ", Coordinate(" << point.x << "," << point.y << ")";
    }*/

    return 0;
}