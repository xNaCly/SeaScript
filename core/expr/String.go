package expr

import (
	"seascript/core/token"
	"strings"
)

type String struct {
	Token token.Token
}

func (s *String) GetToken() token.Token {
	return s.Token
}

func (s *String) CodeGen(b *strings.Builder) {
	b.WriteRune('"')
	b.WriteString(s.Token.Raw)
	b.WriteRune('"')
}

func (s *String) Print(b *strings.Builder, ident int) {
	b.WriteString(strings.Repeat(" ", ident))
	b.WriteString(token.LOOKUP[token.STRING])
	b.WriteRune('#')
	b.WriteString(s.Token.Raw)
	b.WriteRune('\n')
}
