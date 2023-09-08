package token

var KEYWORDS = map[string]struct{}{
	"return": {},
}

type Token struct {
	Pos  int
	Line int
	Type TokenType
	Raw  string
}
