class GradeCenter {
	public void saveGradesToStream(PrintStream stream, GradeSelection selection) {
		stream.println("Printing grades for " + selection.getNetid());
		if (selection.selectedMp0()) { // returns bool if mp0 is selected (has the value true)
			... // print mp0 grades
		}
		... // similarly for mp1, mp2, mp3, and mp4; code omitted for brevity
	}

	public void printGrades(GradeSelection selection) {
		saveGradesToStream(System.out, selection);
	}
}

class Report {
	public void processAllGradesForStudent(GradeCenter gradeCenter, String netid) {
		gradeCenter.printGrades(new GradeSelection(netid));
	}
}
