package token

type Token struct {
	Pos  int
	Line int
	Type TokenType
	Raw  string
}
