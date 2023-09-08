package expr

import (
	"seacc/core/token"
	"strings"
)

type Keyword struct {
	Token token.Token
}

func (k *Keyword) GetToken() token.Token {
	return k.Token
}

func (k *Keyword) CodeGen(b *strings.Builder) {
	b.WriteString(k.Token.Raw)
	b.WriteRune(' ')
}

func (k *Keyword) Print(b *strings.Builder, ident int) {
	b.WriteString(strings.Repeat(" ", ident))
	b.WriteString("KEYWORD")
	b.WriteRune('#')
	b.WriteString(k.Token.Raw)
	b.WriteRune('\n')
}
