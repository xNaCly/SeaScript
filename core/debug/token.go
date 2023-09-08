package debug

import (
	"fmt"
	"seascript/core/token"
	"strings"
)

func Token(t []token.Token) {
	fmt.Printf("+ %-25s + %-100s +\n", strings.Repeat("-", 25), strings.Repeat("-", 100))
	fmt.Printf("| %-25s | %-100s |\n", "type", "raw")
	fmt.Printf("| %-25s + %-100s |\n", strings.Repeat("-", 25), strings.Repeat("-", 100))
	for _, c := range t {
		fmt.Printf("| %-25s | %-100q |\n", token.LOOKUP[c.Type], c.Raw)
	}
	fmt.Printf("+ %-25s + %-100s +\n", strings.Repeat("-", 25), strings.Repeat("-", 100))
}
