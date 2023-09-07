
package core

import (
	"flag"
	"log"
	"os"
	"seascript/core/debug"
	"seascript/core/lexer"
)

func Compile() {
	source := flag.String("i", "", "specify source file")
	flag.Parse()

	if *source == "" {
		log.Fatalln("missing source file")
	}

	file, err := os.Open(*source)

	if err != nil {
		log.Fatalf("opening %q failed: %s", *source, err)
	}

	l := lexer.New(file)
	token := l.Lex()

	debug.Token(token)
}
