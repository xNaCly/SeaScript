package expr

import (
	"seascript/core/token"
	"strings"
)

type Node interface {
	GetToken() token.Token
	CodeGen(b *strings.Builder)
	Print(b *strings.Builder, ident int)
}
