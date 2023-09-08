package expr

import (
	"seascript/core/token"
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
	b.WriteRune('\n')
}

func (s *Semi) Print(b *strings.Builder, ident int) {
	b.WriteString(strings.Repeat(" ", ident))
	b.WriteString(token.LOOKUP[token.SEMICOLON])
	b.WriteRune('\n')
}
