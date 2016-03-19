class ValueRange {
  int lo = Math.minInt();
  int hi = Math.maxInt();
  // Write the class invariant relating %*\textbf{lo}*) to %*\textbf{hi}*).
  %* \CodeToDoGeneral{} *)


  void setLo(int i) { if (i <= hi) lo = i; }
  void setHi(int i) { if (i >= lo) hi = i; }
}
