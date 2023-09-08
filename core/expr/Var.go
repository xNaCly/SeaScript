package expr

import (
	"seacc/core/consts"
	"seacc/core/token"
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
	arr := false
	switch v.Value.(type) {
	case *Bool:
		variableType = consts.BOOL
	case *String:
		variableType = consts.STRING
	case *Array:
		if val, _ := v.Value.(*Array); val.Type != 0 {
			switch val.Type {
			case token.STRING:
				variableType = consts.STRING
			}
		}
		arr = true
	}
	b.WriteString(consts.TYPE_MAP[variableType])
	b.WriteRune(' ')
	b.WriteString(v.Token.Raw)
	if arr {
		b.WriteString("[]")
	}
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
