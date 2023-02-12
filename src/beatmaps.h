#include <Beat.h>
#include <Beatmap.h>

Beat BEATS_0[17] = {
    Beat(-1, 0, 261, 1000),
    Beat(0, 1000, 261,1000),
    Beat(1, 2000, 392, 1000),
    Beat(2, 3000, 392, 1000),
    Beat(3, 4000, 440,1000),
    Beat(0, 5000, 440,1000),
    Beat(1, 6000, 392,500),
    Beat(2, 7000, 0,0),
    Beat(-1, 8000, 349,1000),
    Beat(3, 9000, 349,1000),
    Beat(0, 10000, 329,1000),
    Beat(1, 11000, 329,1000),
    Beat(2, 12000, 293,1000),
    Beat(3, 13000, 293,1000),
    Beat(0, 14000, 261,500),
    Beat(1, 15000, 0,0),
    Beat(-1, 16000, 0,0)
};

Beatmap BEATMAP_0 =Beatmap(0, "Twinkle Twinkle", 17, 16000, 1200, BEATS_0);

Beatmap *BEATMAPS[6] = {&BEATMAP_0,&BEATMAP_0,&BEATMAP_0,&BEATMAP_0,&BEATMAP_0,&BEATMAP_0};