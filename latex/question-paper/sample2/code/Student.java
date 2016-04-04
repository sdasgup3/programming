public class Student {
  private String name; private int score;
  public Student(String name, int score) {
    this.name = name; this.score = score;
  }
  public String result() {
    if (score > 100 || score < 0) { return "Not a valid input!"; }
    if (score >= 80) { return "Good"; }
    if (score < 80 && score >= 60) { return "Pass"; } else { return "Fail"; }
  }
}

