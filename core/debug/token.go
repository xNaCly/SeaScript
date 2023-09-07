package debug

import (
	"fmt"
	"seascript/core/token"
	"strings"
)

func Token(t []token.Token) {
	fmt.Printf("+ %-25s + %-25s +\n", strings.Repeat("-", 25), strings.Repeat("-", 25))
	fmt.Printf("| %-25s | %-25s |\n", "type", "raw")
	fmt.Printf("| %-25s + %-25s |\n", strings.Repeat("-", 25), strings.Repeat("-", 25))
	for _, c := range t {
		fmt.Printf("| %-25s | %-25q |\n", token.LOOKUP[c.Type], c.Raw)
	}
	fmt.Printf("+ %-25s + %-25s +\n", strings.Repeat("-", 25), strings.Repeat("-", 25))
}

