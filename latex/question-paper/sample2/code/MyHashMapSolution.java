% FROM DARKO: rewrite this to move the text into method names
	-an empty collection
	-contains exactly one or some elements
	-reaching 3/4 of its current size
	-inserting duplicate key values
	
	 public class HashMapTest {
   	
	// [TODO] WRITE YOUR CODE BELOW
	@Test
		public void Test1()
		{
			MyHashMap test=new MyHashMap(4);
			AssertTrue(test.isEmpty());
			
		}
	

	
	//[TODO] WRITE YOUR CODE BELOW
	@Test
		public void Test2()
		{
			MyHashMap test=new MyHashMap(4);
			test.put(1,"A");
			test.put(2,"B");
			AssertFalse(test.isEmpty());
			AssertEquals(test.getCurSize(),2);
			
		}

	
	// [TODO] WRITE YOUR CODE BELOW
	@Test
		public void Test3()
		{
			MyHashMap test=new MyHashMap(4);
			test.put(1,"A");
			test.put(2,"B");
			test.put(3,"B");
			AssertEquals(test.getMaxSize(),8);
			
		}
	
	@Test
		public void Test4()
		{
			MyHashMap test=new MyHashMap(4);
			test.put(1,"A");
			test.put(1,"B");
			AssertEquals(test.get(1),"B");
			
		}
	
	
	
	
	
	
	
	
	
	
	
	
	
	
   }
