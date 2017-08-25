#include <iostream>
#include <chrono>
#include "database.h"

int main() {
	std::ios_base::sync_with_stdio(false);

    OsuDB reader("F:\\Osu");
    reader.read();

    return 0;
}