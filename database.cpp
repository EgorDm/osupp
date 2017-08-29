//
// Created by egordm on 24-8-2017.
//

#include "database.h"

using namespace std;

DatabaseReader::DatabaseReader(std::string filePath) {
    file.open(filePath, ios::in | ios::binary | ios::ate);
    file.seekg(0, ios::beg);
}


DatabaseReader::~DatabaseReader() {
    if (file.is_open()) file.close();
}

void DatabaseReader::readString(std::string &value) {
    char not_empty;
    readVal(not_empty);
    if (not_empty == 0x00) return;
    unsigned long length = decodeULeb128();
    value.resize(length);
    file.read(&value[0], length);
}

unsigned long DatabaseReader::decodeULeb128() {
    unsigned long ret = 0;
    unsigned shift = 0;
    unsigned char byte;
    while (true) {
        readVal(byte);
        ret |= ((byte & 0x7F) << shift);
        if ((byte & (1 << 7)) == 0) break;
        shift += 7;
    }
    return ret;
}

void DatabaseReader::skipString() {
    char not_empty;
    readVal(not_empty);
    if (not_empty == 0x00) return;
    auto length = decodeULeb128();
    file.ignore(length);
}

void OsuDB::read() {
    readVal(version);
    file.ignore(13);
    readString(user);

    int n_beatmaps;
    readVal(n_beatmaps);

    printf("Reading %s's database. Expecting %i maps.\n", user.c_str(), n_beatmaps); //TODO: C, not cool m8

    for (auto i = 0; i < n_beatmaps; ++i) {
        try {
            auto bm = readBeatmap();
            entries.push_back(bm);
        } catch (...) {}
    }
}

BeatmapEntry OsuDB::readBeatmap() {
    auto planB = readInt();
    planB += static_cast<int>(file.tellg());

    try {
        BeatmapEntry bm = BeatmapEntry();
        readString(bm.artist);
        skipString();
        readString(bm.title);
        skipString();
        readString(bm.creator);
        readString(bm.version);
        readString(bm.audio_file);


        skipString();
        readString(bm.osu_file);
        readVal(bm.ranked);
        file.ignore(14);

        readVal(bm.ar);
        readVal(bm.cs);
        readVal(bm.hp);
        readVal(bm.od);
        file.ignore(8);

        if (version >= 20140609) {
            bm.std_diffs = readDiffPairs();
            bm.taiko_diffs = readDiffPairs();
            bm.ctb_diffs = readDiffPairs();
            bm.mania_diffs = readDiffPairs();
        }

        readVal(bm.time_drain);
        readVal(bm.time_total);
        file.ignore(4);

        int n_tps = readInt();
        for (int i = 0; i < n_tps; ++i) {
            bm.timingpoints.push_back(std::shared_ptr<TimingPoint>(readTimingPoint()));
            //bm.timingpoints.push_back(readTimingPoint());
        }

        readVal(bm.id);
        readVal(bm.set_id);
        file.ignore(14);

        readVal(bm.mode);
        skipString();
        skipString();
        file.ignore(2);
        skipString();
        file.ignore(10);

        readString(bm.folder_name);
        file.ignore(18);
        if (static_cast<int>(file.tellg()) != planB)
            throw std::runtime_error("Offsets are not equal. Entry corrupted?");
        return bm;
    }
    catch (std::runtime_error &e) {
        std::cerr << "exception: " << e.what() << std::endl;
    }
    file.seekg(planB, ios::beg);
    throw std::runtime_error("Could not read the beatmap entry.");
}

TimingPoint *OsuDB::readTimingPoint() {
    double mpb, offset;
    readVal(mpb);
    readVal(offset);
    file.ignore(1);
    if (mpb > 0) {
        return new KeyTimingPoint(offset, mpb);
    }

    return new InheritedTimingPoint(offset, static_cast<float>(mpb / -100));
}

std::map<int, double> OsuDB::readDiffPairs() {
    map<int, double> ret;
    int n_diffs = readInt();
    for (int i = 0; i < n_diffs; ++i) {
        file.ignore(1);
        int mod = readInt();
        file.ignore(1);
        double rating;
        readVal(rating);
        ret[mod] = rating;
    }
    return ret;
}

OsuDB::~OsuDB() {
}
