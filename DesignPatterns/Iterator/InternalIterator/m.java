public class m {
	
	public static void main(String[] args){

        MyList l = new MyList();
        Command c = new ConcreteCommand();  
		
          l.apply(c);
		
	}
	
}
