package core

import (
	"errors"
	"flag"
	"log"
	"os"
	"seacc/core/debug"
	"seacc/core/generator"
	"seacc/core/lexer"
	"seacc/core/parser"
)

func Compile() {
	source := flag.String("i", "", "specify source file for compiler")
	out := flag.String("o", "", "specify output file for generated code")
	force := flag.Bool("force", false, "override file contents with generated code")
	dbg := flag.Bool("debug", false, "print lexed tokens and ast")
	flag.Parse()

	if *source == "" {
		log.Fatalln("missing source file")
	}
	if *out == "" {
		*out = "a.c"
	}

	if *force {
		log.Println("got -force, watch out for file overrides")
	}

	file, err := os.Open(*source)

	if err != nil {
		log.Fatalf("opening %q for reading failed: %s", *source, err)
	}

	l := lexer.New(file)
	tokens := l.Lex()
	if *dbg {
		debug.Token(tokens)
	}

	p := parser.New(tokens)
	ast := p.Parse()
	if *dbg {
		debug.Ast(ast)
	}
	codeGen := generator.Gen(ast)

	if _, err := os.Stat(*out); err == nil {
		if !*force {
			log.Fatalf("file %q already exists, consider using -force to override this check and write the generated code to the file\n", *out)
		}
	} else if !errors.Is(err, os.ErrNotExist) {
		log.Fatalf("failed stat for %q, something went wrong: ", err)
	}

	file, err = os.Create(*out)

	if err != nil {
		log.Fatalf("creating %q for writing failed: %s", *out, err)
	}

	file.WriteString(codeGen)

	err = file.Close()
	if err != nil {
		log.Fatalf("failed to close %q: ", err)
	} else {
		log.Printf("wrote %d characters to %q, generated from %q", len(codeGen), *out, *source)
	}
}
