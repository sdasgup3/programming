class GradeCenter {
  void printAllGrades(String netid) {
    printGrades(netid, true, true, true, true);
  }
  void printGrades(String netid,
      boolean mp0, boolean mp1, boolean mp2, boolean mp3) {
    saveGrades(System.out, netid, mp0, mp1, mp2, mp3);
  }
  void saveGrades(PrintStream stream, String netid,
      boolean mp0, boolean mp1, boolean mp2, boolean mp3) {
    stream.println("Printing grades for " + netid);
    if (mp0) {
      // print mp0 grades
    }
    ... // similarly for mp1, mp2, and mp3; code omitted for brevity
  }
  ...
}
