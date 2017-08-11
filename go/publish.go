// publish implements rendering of the static html file created by
// havocdetection_main to localhost ona specified port.
package main

import (
	"fmt"
	"net/http"
	"os"
	"path"
	"regexp"

	"google3/base/go/flag"
	"google3/base/go/google"
	"google3/base/go/log"
	"google3/base/go/runfiles"
)

var (
	clientPath = flag.String("path", "google3/experimental/users/sdasgup/havocdetection", "path of the client files directory")
	validPath  = regexp.MustCompile("^/(report)/$")
)

func reportHandler(w http.ResponseWriter, r *http.Request) {
	staticPath := runfiles.Path(*clientPath)
	m := validPath.FindStringSubmatch(r.URL.Path)
	if m == nil {
		fmt.Println("Not valid URL path")
		http.NotFound(w, r)
		return
	}

	fileName := m[1] + ".html"
	filePath := path.Join(staticPath, fileName)
	fmt.Println(filePath)
	if _, err := os.Stat(filePath); os.IsNotExist(err) {
		http.Redirect(w, r, "/report/", http.StatusFound)
		return
	}
	http.ServeFile(w, r, filePath)
}

func main() {
	google.Init()

	http.HandleFunc("/report/", reportHandler)
	log.Fatal(http.ListenAndServe(":8080", nil))
}
