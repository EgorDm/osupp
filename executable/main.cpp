#include <iostream>
#include "database.h"
#include "maths.h"
#include "curves.h"
#include "utils.h"
#include "beatmap_io.h"

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