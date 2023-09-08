package expr

import (
	"seascript/core/consts"
	"seascript/core/token"
	"strings"
)

type Var struct {
	Token token.Token
	Value Node
}

func (v *Var) GetToken() token.Token {
	return v.Token
}

func (v *Var) CodeGen(b *strings.Builder) {
	variableType := consts.NUMBER
	switch v.Value.GetToken().Type {
	case token.STRING:
		variableType = consts.STRING
	}
	b.WriteString(consts.TYPE_MAP[variableType])
	b.WriteRune(' ')
	b.WriteString(v.Token.Raw)
	if v.Value != nil {
		b.WriteRune('=')
		v.Value.CodeGen(b)
	}
}

func (v *Var) Print(b *strings.Builder, ident int) {
	b.WriteString(strings.Repeat(" ", ident))
	b.WriteString(token.LOOKUP[v.Token.Type])
	b.WriteRune('#')
	b.WriteString(v.Token.Raw)
	b.WriteRune('\n')
}
