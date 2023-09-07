package token

type TokenType int8

const (
	UNKNOWN TokenType = iota
	IDENT
	BRACE_LEFT
	BRACE_RIGHT
	EQUAL
	NUMBER
	STRING
	VAR
	EOF
)

var LOOKUP = map[TokenType]string{
	UNKNOWN:     "UNKNOWN",
	IDENT:       "IDENT",
	BRACE_LEFT:  "BRACE_LEFT",
	BRACE_RIGHT: "BRACE_RIGHT",
	EQUAL:       "EQUAL",
	NUMBER:      "NUMBER",
	STRING:      "STRING",
	VAR:         "VAR",
	EOF:         "EOF",
}
