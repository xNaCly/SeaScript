package expr

import (
	"seascript/core/token"
	"strings"
)

type Call struct {
	Token     token.Token
	Arguments []Node
}

func (c *Call) GetToken() token.Token {
	return c.Token
}

func (c *Call) CodeGen(b *strings.Builder) {
	b.WriteString(c.Token.Raw)
	b.WriteRune('(')
	for i, a := range c.Arguments {
		a.CodeGen(b)
		if i+1 < len(c.Arguments) {
			b.WriteRune(',')
		}
	}
	b.WriteRune(')')
}

func (c *Call) Print(b *strings.Builder, ident int) {
	b.WriteString(strings.Repeat(" ", ident))
	b.WriteString(token.LOOKUP[token.FUNC])
	b.WriteRune('#')
	b.WriteString(c.Token.Raw)
	b.WriteRune('\n')
	for _, a := range c.Arguments {
		a.Print(b, ident+1)
	}
}
