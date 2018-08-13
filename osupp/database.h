//
// Created by egordm on 24-8-2017.
//

#ifndef OSUPP_DATABASE_H
#define OSUPP_DATABASE_H

#include <fstream>
#include <iostream>
#include "models.h"

namespace osupp {
    class DatabaseReader {
    public:
        explicit DatabaseReader(std::string filePath);

        template<typename T>
        void readVal(T &value) {
            file.read(reinterpret_cast<char *>(&value), sizeof(value));
        }

        int readInt() {
            int ret;
            readVal(ret);
            return ret;
        }

        void readString(std::string &value);

        void skipString();

    protected:
        unsigned long decodeULeb128();

        std::ifstream file;
    };


    class OsuDB : public DatabaseReader {
    public:
        explicit OsuDB() : DatabaseReader("\\osu!.db") {}

        explicit OsuDB(const std::string &filePath) : DatabaseReader(filePath + "\\osu!.db") {};

        int version;
        std::string user;
        std::vector<BeatmapEntry> entries;

        void read();

    protected:
        BeatmapEntry readBeatmap();

        TimingPoint *readTimingPoint();

        std::map<int, double> readDiffPairs();
    };
}

#endif //OSUPP_DATABASE_H
