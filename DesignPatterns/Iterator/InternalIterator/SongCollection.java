import java.util.ArrayList;
import java.util.Iterator;

public class SongCollection  {
	
	MyList bestSongs;
	
	public SongCollection() {
		
		bestSongs = new MyList();
                bestSongs.add(1);
                bestSongs.add(2);
                bestSongs.add(3);
		
		
	}
	
        public void apply(Command c) {
          Iterator i = bestSongs.iterator();
          while(i.hasNext()){
            int songInfo = (Integer) i.next();
            c.execute(songInfo);
          }
        }

}
