package expr

import (
	"seacc/core/token"
	"strings"
)

type Symbol struct {
	Token token.Token
}

func (s *Symbol) GetToken() token.Token {
	return s.Token
}

func (s *Symbol) CodeGen(b *strings.Builder) {
	b.WriteString(strings.TrimSpace(s.Token.Raw))
}

func (s *Symbol) Print(b *strings.Builder, ident int) {
	b.WriteString(strings.Repeat(" ", ident))
	b.WriteString(token.LOOKUP[s.Token.Type])
	b.WriteRune('#')
	b.WriteString(s.Token.Raw)
	b.WriteRune('\n')
}
