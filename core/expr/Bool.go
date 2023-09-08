package expr

import (
	"seacc/core/token"
	"strings"
)

type Bool struct {
	Token token.Token
}

func (b *Bool) GetToken() token.Token {
	return b.Token
}

func (bo *Bool) CodeGen(b *strings.Builder) {
	boolean := '0'
	if bo.Token.Raw == "true" {
		boolean = '1'
	}
	b.WriteRune(boolean)
}

func (bo *Bool) Print(b *strings.Builder, ident int) {
	b.WriteString(strings.Repeat(" ", ident))
	b.WriteString(token.LOOKUP[token.BOOL])
	b.WriteRune('#')
	b.WriteString(bo.Token.Raw)
	b.WriteRune('\n')
}
