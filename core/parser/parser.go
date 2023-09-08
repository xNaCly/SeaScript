package parser

import (
	"log"
	"seacc/core/expr"
	"seacc/core/token"
	"strings"
)

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
		var n expr.Node
		switch p.peek().Type {
		case token.CBLOCK:
			n = &expr.CBlock{Token: p.peek()}
		case token.PREPROCESSOR:
			n = &expr.Preprocessor{Token: p.peek()}
		case token.SEMICOLON:
			n = &expr.Semi{Token: p.peek()}
		default:
			ast = append(ast, p.parseConstant())
			continue
		}
		ast = append(ast, n)
		p.advance()
	}
	return ast
}

func (p *Parser) parseConstant() expr.Node {
	var n expr.Node
	switch p.peek().Type {
	case token.STRING:
		n = &expr.String{Token: p.peek()}
	case token.NUMBER:
		n = &expr.Number{Token: p.peek()}
	case token.IDENT:
		if p.peekNext().Type == token.BRACE_LEFT {
			return p.functionCall()
		} else if p.peekNext().Type == token.EQUAL {
			return p.variableDef()
		} else {
			n = &expr.Ident{Token: p.peek()}
		}
	default:
		log.Panic("unimplemented")
	}
	p.advance()
	return n
}

// TODO: parse arrays
func (p *Parser) variableDef() expr.Node {
	n := &expr.Var{Token: p.peek()}
	p.advance()
	p.matchConsumeOrError(token.EQUAL)
	p.matchAnyOrError(token.STRING, token.IDENT, token.NUMBER)
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
		p.matchAnyOrError(token.STRING, token.IDENT, token.NUMBER)
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
