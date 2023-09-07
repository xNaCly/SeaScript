package lexer

import (
	"bufio"
	"fmt"
	"io"
	"log"
	"seascript/core/token"
	"strings"
	"unicode"
)

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
		case ' ', '\t', '\n':
			if l.cc == '\n' {
				l.pos = 0
				l.line++
			}
			l.advance()
			continue
		case '(':
			t = token.BRACE_LEFT
		case '=':
			t = token.EQUAL
		case ')':
			t = token.BRACE_RIGHT
		case '"':
			tok = append(tok, l.string())
			continue
		case ':':
			if l.peekMatch(':') {
				for l.cc != 0 && l.cc != '\n' {
					l.advance()
				}
				continue
			}
		default:
			if unicode.IsLetter(l.cc) || l.cc == '_' {
				tok = append(tok, l.ident())
				continue
			}
		}

		if t == token.UNKNOWN {
			return l.error("tok")
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
	return tok
}

func (l *Lexer) error(str string) []token.Token {
	l.hasError = true
	if str == "tok" {
		log.Printf("Unknown token %q at pos: %d in line %d\n", l.cc, l.pos, l.line+1)
	} else {
		log.Printf(str)
	}
	return []token.Token{}
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
		l.error(fmt.Sprintf("Unterminated string '\"%s' at pos %d of line %d\n", str, l.pos-len(str), l.line))
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

func (l *Lexer) ident() token.Token {
	for unicode.IsLetter(l.cc) || l.cc == '_' || unicode.IsDigit(l.cc) {
		builder.WriteRune(l.cc)
		l.advance()
	}

	str := builder.String()
	builder.Reset()

	return token.Token{
		Type: token.IDENT,
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
