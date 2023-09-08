package token

type TokenType int8

const (
	UNKNOWN TokenType = iota

	PREPROCESSOR
	CBLOCK
	FUNC
	SEMICOLON

	IDENT

	// symbols
	EQUAL

	// structure
	BRACE_LEFT
	BRACE_RIGHT
	CURLY_LEFT
	CURLY_RIGHT

	// constants
	NUMBER
	STRING

	EOF
)

var LOOKUP = map[TokenType]string{
	UNKNOWN:      "UNKNOWN",
	SEMICOLON:    "SEMICOLON",
	PREPROCESSOR: "PREPROCESSOR",
	CBLOCK:       "CBLOCK",
	FUNC:         "FUNC",
	IDENT:        "IDENT",
	EQUAL:        "EQUAL",
	BRACE_LEFT:   "BRACE_LEFT",
	BRACE_RIGHT:  "BRACE_RIGHT",
	CURLY_LEFT:   "CURLY_LEFT",
	CURLY_RIGHT:  "CURLY_RIGHT",
	NUMBER:       "NUMBER",
	STRING:       "STRING",
	EOF:          "EOF",
}
