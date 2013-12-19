	public class Reader{
		
		public String getKeyWord(){
			
		}
	}
	public class Writer{
		
		public void print(String word)
		{
			...
		}
	}
	public class KeyBoard extends Reader{
		
		public String getKeyword(){
			...
		}
	}
	public class Disk extends Writer{
		
		public void print(String word)
		{
			...
		}
	
	}
	public class Printer extends Writer{
		
		public void print(String word)
		{
			...
		}
	
	}
 public class Copy {
   private Reader reader;
   private Writer writer;
 
   public void copyIO()
   {
	   String k;
		while((k = reader.getKeyword())!=null)
			writer.print(k);
   }
 }
