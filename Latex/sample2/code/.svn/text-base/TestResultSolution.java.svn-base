public class ResultTest {
  // TODO: WRITE A COVERAGE TEST HERE:
  @Test
  public void testBoundary(){
    //performing any of thes should do
    Student a=new Student("A", 101);
    assertEquals(a.getResult(),"Not a valid input!");
      
    a=new Student("A",-1);
    assertEquals(a.getResult(),"Not a valid input!");
      
    a=new Student("A",80);
    assertEquals(a.getResult(),"Good");
      
    a=new Student("A",60);
    assertEquals(a.getResult(),"Passed");

    a=new Student("A",50);
    assertEquals(a.getResult(),"Failed");
  }
}
