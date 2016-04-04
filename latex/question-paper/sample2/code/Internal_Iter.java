public interface Command {
  public void execute(SongInfo s);
} 

public class MyCommand implements Command {
  public void execute(SongInfo s) {
    findOnYouTube(s);
  }
}

public class Client {
  public static void main(String[] args) {
    SongCollection sc = new SongCollection();
    sc.addSong("Imagine", "John Lennon", 1971);
    sc.addSong("American Pie", "Don McLean", 1971);
    sc.addSong("I Will Survive", "Gloria Gaynor", 1979);
    Command cmd = new MyCommand();
    sc.apply(cmd);
  }
}
