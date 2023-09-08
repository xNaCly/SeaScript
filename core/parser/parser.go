package parser

import (
	"log"
	"seacc/core/expr"
	"seacc/core/token"
	"strings"
)

func todo(str string) {
	log.Panicln(str)
}

type Parser struct {
	tokens []token.Token
	pos    int
}

func New(tokens []token.Token) *Parser {
	return &Parser{
		tokens: tokens,
	}
}

func (p *Parser) Parse() []expr.Node {
	ast := make([]expr.Node, 0)
	for !p.peekEquals(token.EOF) {
		ast = append(ast, p.parseConstant())
	}
	return ast
}

func (p *Parser) parseConstant() expr.Node {
	var n expr.Node
	switch p.peek().Type {
	case token.CBLOCK:
		n = &expr.CBlock{Token: p.peek()}
	case token.PREPROCESSOR:
		n = &expr.Preprocessor{Token: p.peek()}
	case token.SYMBOL:
		n = &expr.Symbol{Token: p.peek()}
	case token.KEYWORD:
		n = &expr.Keyword{Token: p.peek()}
	case token.STRING:
		n = &expr.String{Token: p.peek()}
	case token.NUMBER:
		n = &expr.Number{Token: p.peek()}
	case token.BOOL:
		n = &expr.Bool{Token: p.peek()}
	case token.SEMICOLON:
		n = &expr.Semi{Token: p.peek()}
	case token.BRACKET_LEFT:
		return p.array()
	case token.IDENT:
		if p.peekNext().Type == token.BRACE_LEFT {
			return p.functionCall()
		} else if p.peekNext().Type == token.EQUAL {
			return p.variableDef()
		} else if p.peekNext().Type == token.BRACKET_LEFT {
			return p.functionDef()
		} else {
			n = &expr.Ident{Token: p.peek()}
		}
	default:
		todo("unimplemented: " + token.LOOKUP[p.peek().Type])
	}
	p.advance()
	return n
}

func (p *Parser) parseFunctionParam() expr.Node {
	t := p.peek()
	p.matchConsumeOrError(token.IDENT)
	p.matchConsumeOrError(token.COLON)
	ty := p.peek().Raw
	p.matchConsumeOrError(token.IDENT)

	return &expr.Arg{
		Token: t,
		Type:  ty,
	}
}

func (p *Parser) functionDef() expr.Node {
	t := p.peek()
	t.Type = token.FUNC
	n := &expr.Func{
		Token:     t,
		Arguments: make([]expr.Node, 0),
		Body:      make([]expr.Node, 0),
	}
	p.advance()
	p.matchConsumeOrError(token.BRACKET_LEFT)
	for !p.peekEquals(token.BRACKET_RIGHT) && !p.peekEquals(token.EOF) {
		n.Arguments = append(n.Arguments, p.parseFunctionParam())
	}
	p.matchConsumeOrError(token.BRACKET_RIGHT)
	p.matchConsumeOrError(token.COLON)
	n.Type = p.peek().Raw
	p.matchConsumeOrError(token.IDENT)
	p.matchConsumeOrError(token.CURLY_LEFT)
	for !p.peekEquals(token.CURLY_RIGHT) && !p.peekEquals(token.EOF) {
		n.Body = append(n.Body, p.parseConstant())
	}
	p.matchAndError(token.EOF)
	p.matchConsumeOrError(token.CURLY_RIGHT)
	return n
}

func (p *Parser) array() expr.Node {
	n := &expr.Array{Token: p.peek(), Elements: make([]expr.Node, 0)}
	p.matchConsumeOrError(token.BRACKET_LEFT)
	for !p.peekEquals(token.BRACKET_RIGHT) && !p.peekEquals(token.EOF) {
		n.Elements = append(n.Elements, p.parseConstant())
	}

	p.matchAndError(token.EOF)

	p.matchConsumeOrError(token.BRACKET_RIGHT)

	for i := 0; i < len(n.Elements); i++ {
		if i+1 < len(n.Elements) {
			ct := n.Elements[i].GetToken()
			nt := n.Elements[i+1].GetToken()
			if ct.Type != nt.Type {
				log.Panicf("Differing types in array, element at pos %d is of type %q, element at pos %d isn't (%q) (line: %d)", ct.Pos, token.LOOKUP[ct.Type], nt.Pos, token.LOOKUP[nt.Type], nt.Line)
			}
		}
	}

	if len(n.Elements) > 0 {
		n.Type = n.Elements[0].GetToken().Type
	}

	return n
}

func (p *Parser) variableDef() expr.Node {
	n := &expr.Var{Token: p.peek()}
	p.advance()
	p.matchConsumeOrError(token.EQUAL)
	n.Value = p.parseConstant()
	return n
}

func (p *Parser) functionCall() expr.Node {
	n := &expr.Call{
		Token:     p.peek(),
		Arguments: make([]expr.Node, 0),
	}

	p.advance() // skip ident

	p.matchConsumeOrError(token.BRACE_LEFT)

	for !p.peekEquals(token.BRACE_RIGHT) && !p.peekEquals(token.EOF) {
		n.Arguments = append(n.Arguments, p.parseConstant())
	}

	p.matchAndError(token.EOF)

	p.matchConsumeOrError(token.BRACE_RIGHT)

	return n
}

// if p.peek().Type matches any of tt consume that token, otherwise error
func (p *Parser) matchAnyOrError(tt ...token.TokenType) {
	matched := false
	ct := p.peek()
	for _, t := range tt {
		if ct.Type == t {
			matched = true
		}
	}
	if !matched {
		expected := make([]string, len(tt))
		for i, t := range tt {
			expected[i] = token.LOOKUP[t]
		}
		log.Panicf("expected any of %q, got %q at pos %d of line %d", strings.Join(expected, ","), token.LOOKUP[ct.Type], ct.Pos, ct.Line)
	}
}

// if p.peek().Type matches tt consume that token, otherwise panic
func (p *Parser) matchConsumeOrError(tt token.TokenType) {
	if p.peek().Type == tt {
		p.advance()
	} else {
		log.Panicf("expected %q, got %q at pos %d of line %d\n", token.LOOKUP[tt], token.LOOKUP[p.peek().Type], p.peek().Pos, p.peek().Line)
	}
}

// if p.peek().Type matches tt panic
func (p *Parser) matchAndError(tt token.TokenType) {
	if p.peekNext().Type == tt {
		log.Panicf("unexpected %q at pos %d of line %d\n", token.LOOKUP[tt], p.peek().Pos, p.peek().Line)
	}
}

func (p *Parser) peek() token.Token {
	return p.tokens[p.pos]
}

func (p *Parser) peekEquals(ttype token.TokenType) bool {
	return p.peek().Type == ttype
}

func (p *Parser) peekNext() token.Token {
	if p.pos+1 < len(p.tokens) {
		return p.tokens[p.pos+1]
	}
	return p.tokens[p.pos]
}

func (p *Parser) advance() {
	if p.peekEquals(token.EOF) {
		return
	}
	if p.pos+1 < len(p.tokens) {
		p.pos++
	}
}
