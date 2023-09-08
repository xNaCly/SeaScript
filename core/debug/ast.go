package debug

import (
	"fmt"
	"seacc/core/expr"
	"strings"
)

func Ast(ast []expr.Node) {
	b := strings.Builder{}
	for _, n := range ast {
		n.Print(&b, 0)
	}
	fmt.Println(b.String())
}
