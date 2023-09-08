package expr

import (
	"seacc/core/token"
	"strings"
)

type Ident struct {
	Token token.Token
}

func (i *Ident) GetToken() token.Token {
	return i.Token
}

func (i *Ident) CodeGen(b *strings.Builder) {
	b.WriteString(i.Token.Raw)
}

func (i *Ident) Print(b *strings.Builder, ident int) {
	b.WriteString(strings.Repeat(" ", ident))
	b.WriteString(token.LOOKUP[i.Token.Type])
	b.WriteRune('#')
	b.WriteString(i.Token.Raw)
	b.WriteRune('\n')
}
