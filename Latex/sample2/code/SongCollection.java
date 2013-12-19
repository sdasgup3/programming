import java.util.LinkedList;
import java.util.List;
public class SongCollection {
  List<SongInfo> bestSongs;
  public SongCollection() {
    bestSongs = new LinkedList<SongInfo>();
  }
  public void addSong(String songName, String bandName, int yearReleased) {
    SongInfo songInfo = new SongInfo(songName, bandName, yearReleased);
    bestSongs.add(songInfo);
  }
  private Iterator<SongInfo> createIterator() {
    return bestSongs.iterator();
  }
  public void apply(Command cmd) {
    %* \CodeToDoGeneral{} *)








  }
}
