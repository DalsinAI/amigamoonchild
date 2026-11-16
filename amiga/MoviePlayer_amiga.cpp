/* Moon Child's cut scenes (the intro, the bumpers between worlds, the end
 * and game over) are MP4 files the desktop version plays with FFmpeg. GFOS
 * has no H.264 decoder the game can call yet, so on AmigaOS each movie
 * "ends" as soon as it starts: the game's done-callback runs at once and
 * play goes on to what follows the movie, as when a player skips it.
 * The movies are not shipped. Same interface as src/MoviePlayer.h.
 * Copyright (c) 2026 AmiCradle Team. MIT, like Moon Child's code. */
#include "MoviePlayer.h"

#include <cstdio>

struct MoviePlayer::Impl {};

MoviePlayer::MoviePlayer()
    : impl(nullptr), doneCallback(nullptr), doneUserData(nullptr), playing(false),
      streamEnded(false), movieDurationSec(0), movieStartTicksMs(0), lastPresentedPtsSec(0) {}

MoviePlayer::~MoviePlayer() {}

bool MoviePlayer::playFile(char *filePath, MovieDoneCallback cb, void *userData) {
  (void)filePath;
  doneCallback = cb;
  doneUserData = userData;
  playing = false;
  invokeDoneCallback(true);
  return false;
}

void MoviePlayer::stop(bool naturalEnd) {
  if (playing) {
    playing = false;
    invokeDoneCallback(naturalEnd);
  }
}

bool MoviePlayer::isPlaying() { return playing; }

void MoviePlayer::update(uint8_t *pixels, int width, int height, int pitch) {
  (void)pixels; (void)width; (void)height; (void)pitch;
}

void MoviePlayer::clearState() {}
bool MoviePlayer::decodeNextFrame() { return false; }
void MoviePlayer::presentCurrentFrame(uint8_t *pixels, int width, int height, int pitch) {
  (void)pixels; (void)width; (void)height; (void)pitch;
}

void MoviePlayer::invokeDoneCallback(bool naturalEnd) {
  MovieDoneCallback cb = doneCallback;
  doneCallback = nullptr;
  if (cb) {
    cb(naturalEnd, doneUserData);
  }
}
