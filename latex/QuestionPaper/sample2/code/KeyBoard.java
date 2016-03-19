public class KeyBoard{
    public String getKeyword(){
	...
    }
}

public class Printer{
    public void print(String word){
	...
    }
}

public class Copy {
    private KeyBoard keyboard;
    private Printer printer;
    public void copyIO(){
        String k;
        while((k = keyboard.getKeyword())!=null){
            printer.print(k);
        }
    }
}