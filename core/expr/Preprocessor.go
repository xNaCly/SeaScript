package expr

import (
	"seacc/core/token"
	"strings"
)

type Preprocessor struct {
	Token token.Token
}

func (p *Preprocessor) GetToken() token.Token {
	return p.Token
}

func (p *Preprocessor) CodeGen(b *strings.Builder) {
	b.WriteString(p.Token.Raw)
	b.WriteRune('\n')
}

func (p *Preprocessor) Print(b *strings.Builder, ident int) {
	b.WriteString(strings.Repeat(" ", ident))
	b.WriteString(token.LOOKUP[token.PREPROCESSOR])
	b.WriteRune('\n')
}
