public void apply(Command cmd) {
  for (SongInfo songInfo: bestSongs.createIterator()) {
    cmd.execute(songInfo);
  }
}
