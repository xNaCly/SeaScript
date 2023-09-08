package lexer

import (
	"bufio"
	"io"
	"log"
	"seacc/core/token"
	"strings"
	"unicode"
)

func MatchTokens(tt token.TokenType, t ...token.TokenType) bool {
	for _, tokt := range t {
		if tokt == tt {
			return true
		}
	}
	return false
}

var builder = strings.Builder{}

type Lexer struct {
	cc       rune
	pos      int
	line     int
	hasError bool
	scanner  *bufio.Reader
}

func New(reader io.Reader) *Lexer {
	scanner := bufio.NewReader(reader)
	l := Lexer{
		cc:      ' ',
		scanner: scanner,
	}
	return &l
}

func (l *Lexer) Lex() []token.Token {
	tok := make([]token.Token, 0)
	for l.cc != 0 {
		t := token.UNKNOWN

		switch l.cc {
		case ' ', '\t':
			l.advance()
			continue
		case '\n':
			l.pos = 0
			l.line++
			// add semicolons on newline, if last tok is available, not a semicolon, not a c
			// block or not a preprocessor instruction
			if len(tok) > 0 {
				if MatchTokens(
					tok[len(tok)-1].Type,
					token.CBLOCK,
					token.PREPROCESSOR,
					token.CURLY_LEFT,
					token.CURLY_RIGHT,
					token.SEMICOLON) {
					l.advance()
					continue
				}
			}
			t = token.SEMICOLON
		case '#':
			tok = append(tok, l.preprocessor())
			continue
		case '@':
			tok = append(tok, l.macro())
			continue
		case '(':
			t = token.BRACE_LEFT
		case '+', '-', '*', '/', '%':
			raw := []rune{l.cc, ' '}
			if l.peekMatch('+') || l.peekMatch('-') {
				l.advance()
				raw[1] = l.cc
			}
			tok = append(tok, token.Token{
				Pos:  l.pos,
				Line: l.line,
				Type: token.SYMBOL,
				Raw:  string(raw),
			})

			l.advance()
			continue
		case ')':
			t = token.BRACE_RIGHT
		case '[':
			t = token.BRACKET_LEFT
		case ']':
			t = token.BRACKET_RIGHT
		case '{':
			t = token.CURLY_LEFT
		case '}':
			t = token.CURLY_RIGHT
		case '=':
			t = token.EQUAL
		case '"':
			tok = append(tok, l.string())
			continue
		case ':':
			if l.peekMatch(':') {
				for l.cc != 0 && l.cc != '\n' {
					l.advance()
				}
				continue
			} else {
				t = token.COLON
			}
		default:
			if unicode.IsLetter(l.cc) || l.cc == '_' {
				tok = append(tok, l.ident())
				continue
			} else if unicode.IsDigit(l.cc) || l.cc == '.' {
				tok = append(tok, l.number())
				continue
			}
		}

		if t == token.UNKNOWN {
			log.Panicf("Unknown token %q at pos %d of line %d\n", l.cc, l.pos, l.line)
		} else {
			tok = append(tok, token.Token{
				Pos:  l.pos,
				Line: l.line,
				Type: t,
			})
		}
		l.advance()
	}
	if l.hasError {
		return nil
	}
	return append(tok, token.Token{
		Pos:  l.pos + 1,
		Line: l.line,
		Type: token.EOF,
	})
}

func (l *Lexer) unexpectedRune(wanted rune) {
	log.Panicf("Unexpected token %q at pos: %d in line %d, wanted %q\n", l.cc, l.pos, l.line+1, wanted)
}

func (l *Lexer) string() token.Token {
	l.advance() // skip "
	for l.cc != 0 && l.cc != '\n' && l.cc != '"' {
		builder.WriteRune(l.cc)
		l.advance()
	}

	str := builder.String()
	builder.Reset()

	if l.cc != '"' {
		log.Panicf("Unterminated string '\"%s' at pos %d of line %d\n", str, l.pos-len(str), l.line)
		return token.Token{}
	}

	l.advance() // skip closing

	return token.Token{
		Type: token.STRING,
		Pos:  l.pos - len(str),
		Line: l.line,
		Raw:  str,
	}
}

func (l *Lexer) number() token.Token {
	for unicode.IsDigit(l.cc) || l.cc == '.' || l.cc == '_' || l.cc == 'e' {
		builder.WriteRune(l.cc)
		l.advance()
	}

	str := builder.String()
	builder.Reset()

	return token.Token{
		Type: token.NUMBER,
		Pos:  l.pos - len(str),
		Line: l.line,
		Raw:  str,
	}
}

func (l *Lexer) macro() token.Token {
	l.advance() // skip @
	t := token.UNKNOWN
	mp := struct {
		r    rune
		pos  int
		line int
	}{r: l.cc, pos: l.pos, line: l.line}

	switch mp.r {
	case 'c':
		t = token.CBLOCK
	}
	l.advance() // skip c

	if l.cc != '{' {
		builder.Reset()
		l.unexpectedRune('{')
		return token.Token{}
	}

	l.advance() // skip {

	curly_amount := 1
	for l.cc != 0 {
		if l.cc == '\n' {
			l.pos = 0
			l.line++
		}
		if l.cc == '{' {
			curly_amount++
		} else if l.cc == '}' {
			curly_amount--
		}
		if curly_amount == 0 {
			l.advance()
			break
		}
		builder.WriteRune(l.cc)
		l.advance()
	}

	if curly_amount == 1 {
		log.Panicf("Macro %q starting at pos %d of line %d ist not closed correctly\n", mp.r, mp.pos, mp.line)
	}

	str := builder.String()
	builder.Reset()

	if t == token.UNKNOWN {
		log.Panicf("Unknown macro %q at pos %d of line %d\n", mp.r, mp.pos, mp.line)
		return token.Token{}
	}

	return token.Token{
		Type: t,
		Pos:  l.pos - len(str),
		Line: l.line,
		Raw:  str,
	}
}

func (l *Lexer) preprocessor() token.Token {
	for l.cc != 0 && l.cc != '\n' {
		builder.WriteRune(l.cc)
		l.advance()
	}

	str := builder.String()
	builder.Reset()

	return token.Token{
		Type: token.PREPROCESSOR,
		Pos:  l.pos - len(str),
		Line: l.line,
		Raw:  str,
	}
}

func (l *Lexer) ident() token.Token {
	for unicode.IsLetter(l.cc) || l.cc == '_' || unicode.IsDigit(l.cc) {
		builder.WriteRune(l.cc)
		l.advance()
	}

	str := builder.String()
	builder.Reset()

	t := token.IDENT
	if str == "true" || str == "false" {
		t = token.BOOL
	} else if _, ok := token.KEYWORDS[str]; ok {
		t = token.KEYWORD
	}

	return token.Token{
		Type: t,
		Pos:  l.pos - len(str),
		Line: l.line,
		Raw:  str,
	}
}

func (l *Lexer) advance() {
	if l.hasError {
		l.cc = 0
		return
	}

	c, _, err := l.scanner.ReadRune()

	if err != nil {
		l.cc = 0
		return
	}

	l.pos++
	l.cc = c
}

func (l *Lexer) peekMatch(c rune) bool {
	pr, _, err := l.scanner.ReadRune()
	if err != nil {
		l.hasError = true
	}
	err = l.scanner.UnreadRune()
	if err != nil {
		l.hasError = true
	}
	return pr == c
}
