package expr

import (
	"seacc/core/consts"
	"seacc/core/token"
	"strings"
)

type Func struct {
	Token     token.Token
	Type      string
	Arguments []Node
	Body      []Node
}

func (f *Func) GetToken() token.Token {
	return f.Token
}

func (f *Func) CodeGen(b *strings.Builder) {
	t, arr := consts.ComputeType(f.Type)
	if val, ok := consts.TYPE_MAP[t]; ok {
		b.WriteString(val)
	} else {
		b.WriteString(f.Type)
	}
	if arr {
		b.WriteString("[]")
	}
	b.WriteRune(' ')
	b.WriteString(f.Token.Raw)
	b.WriteRune('(')
	for i, a := range f.Arguments {
		a.CodeGen(b)
		if i+1 < len(f.Arguments) {
			b.WriteRune(',')
		}
	}
	b.WriteString("){")
	for _, a := range f.Body {
		a.CodeGen(b)
	}
	b.WriteString("}")
}

func (f *Func) Print(b *strings.Builder, ident int) {
	b.WriteString(strings.Repeat(" ", ident))
	b.WriteString(token.LOOKUP[token.FUNC])
	b.WriteRune('#')
	b.WriteString(f.Token.Raw)
	b.WriteRune('(')
	for i, a := range f.Arguments {
		a.Print(b, 0)
		if i+1 < len(f.Arguments) {
			b.WriteRune(',')
		}
	}
	b.WriteString("):")
	t, arr := consts.ComputeType(f.Type)
	if val, ok := consts.TYPE_MAP[t]; ok {
		b.WriteString(val)
	} else {
		b.WriteString(f.Type)
	}
	if arr {
		b.WriteString("[]")
	}
	b.WriteRune('\n')
	for _, a := range f.Body {
		a.Print(b, ident+1)
	}
}
