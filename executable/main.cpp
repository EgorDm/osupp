#include <iostream>
#include "../library/database.h"
#include "../library/maths.h"
#include "../library/curves.h"
//#include "beatmap.h"
#include "../library/utils.h"
#include "../library/beatmap_io.h"

using namespace osupp;

void test2() {
    std::ifstream ifile(R"(D:\Developent\CPP\osupp\test.txt)"); // TODO: close
    BeatmapReader reader(ifile);
    auto section = reader.readSection("[Test]");
    ifile.close();

    for(auto &line : section) {
        std::cout << line << std::endl;
    }

    std::ofstream ofile(R"(D:\Developent\CPP\osupp\test.txt)");
    BeatmapWriter writer(ofile);
    writer.writeSection("[Test]", section);
    ofile.close();
}


int main() {
    std::ios_base::sync_with_stdio(false);
    test2();



    return 0;
}