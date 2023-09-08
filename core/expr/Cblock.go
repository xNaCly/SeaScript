package expr

import (
	"seascript/core/token"
	"strings"
)

type CBlock struct {
	Token token.Token
}

func (c *CBlock) GetToken() token.Token {
	return c.Token
}

func (c *CBlock) CodeGen(b *strings.Builder) {
	b.WriteString(c.Token.Raw)
}

func (c *CBlock) Print(b *strings.Builder, ident int) {
	b.WriteString(strings.Repeat(" ", ident))
	b.WriteString(token.LOOKUP[c.Token.Type])
	b.WriteRune('\n')
}
