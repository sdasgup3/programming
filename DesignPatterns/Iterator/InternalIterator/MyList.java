import java.util.ArrayList;
import java.util.Iterator;

public class MyList  {
	
	ArrayList<Integer> bestSongs;
	
	public MyList() {
		
		bestSongs = new ArrayList<Integer>();
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
