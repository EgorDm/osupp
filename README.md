# WIP
to build an executable add -DBUILD_EXEC=true to cmake

Reading osu!.db
```cpp
OsuDB reader("F:\\Osu"); // Pass osu dir
reader.read();
for(BeatmapEntry tp : reader.entries) {
  //todo smt
}
```


Reading beatmap
```cpp
Beatmap bm = BeatmapFile::read("F:\\Osu\\Songs\\407484 Demetori - Gensou, Yume no Owari\\Demetori - Gensou, Yume no Owari (GoldenWolf) [Genjitsu no Hajimari].osu");
```