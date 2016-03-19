class GradeSelection {
    // [TODO] WRITE IN ALL THE FIELDS AND TWO CONSTRUCTORS HERE
    private String netid;
    private boolean mp0;
    private boolean mp1;
    private boolean mp2;
    private boolean mp3;
    private boolean mp4;
    public GradeSelection(String netid) {
      this(netid, true, true, true, true, true);
    }
    public GradeSelection(String netid,
        boolean mp0, boolean mp1, boolean mp2, boolean mp3, boolean mp4) {
      this.netid = netid;
      this.mp0 = mp0;
      this.mp1 = mp1;
      this.mp2 = mp2;
      this.mp3 = mp3;
      this.mp4 = mp4;
    }
    // getters, setters, and other methods omitted
 }
