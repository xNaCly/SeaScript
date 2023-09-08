package expr

import (
	"seacc/core/token"
	"strings"
)

type Array struct {
	Token    token.Token
	Elements []Node
	Type     token.TokenType
}

func (a *Array) GetToken() token.Token {
	return a.Token
}

// TODO: convert this to use sealib/list.h
func (a *Array) CodeGen(b *strings.Builder) {
	b.WriteRune('{')
	for i, c := range a.Elements {
		c.CodeGen(b)
		if i+1 < len(a.Elements) {
			b.WriteRune(',')
		}
	}
	b.WriteRune('}')
}

func (a *Array) Print(b *strings.Builder, ident int) {
	b.WriteString(strings.Repeat(" ", ident))
	b.WriteString(token.LOOKUP[token.BRACKET_LEFT])
	b.WriteRune('#')
	for _, c := range a.Elements {
		c.Print(b, ident+1)
	}
	b.WriteString(token.LOOKUP[token.BRACE_RIGHT])
	b.WriteRune('\n')
}
