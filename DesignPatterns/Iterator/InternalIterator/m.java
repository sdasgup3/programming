public class m {
	
	public static void main(String[] args){

        SongCollection l = new SongCollection();
        Command c = new ConcreteCommand();  
		
          l.apply(c);
		
	}
	
}
