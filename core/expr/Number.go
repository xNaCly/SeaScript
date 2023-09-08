package expr

import (
	"seacc/core/token"
	"strings"
)

type Number struct {
	Token token.Token
}

func (n *Number) GetToken() token.Token {
	return n.Token
}

func (n *Number) CodeGen(b *strings.Builder) {
	b.WriteString(n.Token.Raw)
}

func (n *Number) Print(b *strings.Builder, ident int) {
	b.WriteString(strings.Repeat(" ", ident))
	b.WriteString(token.LOOKUP[token.NUMBER])
	b.WriteRune('#')
	b.WriteString(n.Token.Raw)
	b.WriteRune('\n')
}
