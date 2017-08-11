package main

import (
	"fmt"
)

func main() {
	valSlice := []int{1, 2, 3, 4, 5, 6, 7, 8, 9}
	done := make(chan bool)
	for i, _ := range valSlice {
		
		val := valSlice[i]
		go func() {
			fmt.Println(val)
			done <- true
		}()
	}

	for _, _ = range valSlice {
		<-done
	}

	messages := make(chan string)

	go func() {
		messages <- "buffered"
		messages <- "channel"
	}()

	// Later we can receive these two values as usual.
	fmt.Println(<-messages)
	fmt.Println(<-messages)

}
