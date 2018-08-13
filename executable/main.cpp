#include <iostream>
#include "database.h"
#include "maths.h"
#include "curves.h"
//#include "beatmap.h"
#include "utils.h"
#include "beatmap_io.h"

using namespace osupp;


int main() {
    std::ios_base::sync_with_stdio(false);

    std::string test_file = "F:\\Osu\\Songs\\295784 Yooh - LegenD\\Yooh - LegenD. (Musty) [ADVANCED].osu";
    Beatmap bm = osupp::read_beatmap(test_file);
    
    return 0;
}