package main

import (
	"fmt"
	"sort"
)

type directly []int

func (s directly) Len() int           { return len(s) }
func (s directly) Swap(i, j int)      { s[i], s[j] = s[j], s[i] }
func (s directly) Less(i, j int) bool { return s[i] < s[j] }

type view struct {
	orig []int
	perm []int
}

func newView(s []int) *view {
	perm := make([]int, len(s))
	for i := 0; i < len(perm); i++ {
		perm[i] = i
	}
	return &view{orig: s, perm: perm}
}

func (s *view) sortedCopy() []int {
	c := make([]int, len(s.orig))
	for i := 0; i < len(s.orig); i++ {
		c[i] = s.orig[s.perm[i]]
	}
	return c
}

func (s *view) Len() int           { return len(s.orig) }
func (s *view) Swap(i, j int)      { s.perm[i], s.perm[j] = s.perm[j], s.perm[i] }
func (s *view) Less(i, j int) bool { return s.orig[s.perm[i]] < s.orig[s.perm[j]] }

func main() {
	x := []int{3, 1, 2}
	v := newView(x)
	sort.Sort(v)
	fmt.Println("sorted copy", v.sortedCopy())
	fmt.Println("orig is unchanged", x)
	sort.Sort(directly(x))
	fmt.Println("in-place sorted", x)
}

