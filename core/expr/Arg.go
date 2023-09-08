package expr

import (
	"seacc/core/consts"
	"seacc/core/token"
	"strings"
)

type Arg struct {
	Token token.Token
	Type  string
}

func (a *Arg) GetToken() token.Token {
	return a.Token
}

func (a *Arg) CodeGen(b *strings.Builder) {
	t, arr := consts.ComputeType(a.Type)
	if val, ok := consts.TYPE_MAP[t]; ok {
		b.WriteString(val)
	} else {
		b.WriteString(a.Type)
	}
	b.WriteRune(' ')
	b.WriteString(a.Token.Raw)
	if arr {
		b.WriteString("[]")
	}
}

func (a *Arg) Print(b *strings.Builder, ident int) {
	b.WriteString(a.Token.Raw)
	b.WriteRune(':')
	b.WriteRune(' ')
	t, arr := consts.ComputeType(a.Type)
	if val, ok := consts.TYPE_MAP[t]; ok {
		b.WriteString(val)
	} else {
		b.WriteString(a.Type)
	}
	if arr {
		b.WriteString("[]")
	}
}
