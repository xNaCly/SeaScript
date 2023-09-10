package expr

import (
	"seacc/core/token"
	"strings"
)

type Semi struct {
	Token token.Token
}

func (s *Semi) GetToken() token.Token {
	return s.Token
}

func (s *Semi) CodeGen(b *strings.Builder) {
	b.WriteRune(';')
}

func (s *Semi) Print(b *strings.Builder, ident int) {
	b.WriteString(strings.Repeat(" ", ident))
	b.WriteString(token.LOOKUP[token.SEMICOLON])
	b.WriteRune('\n')
}
