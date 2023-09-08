package generator

import (
	"fmt"
	"seacc/core/expr"
	"seacc/core/lexer"
	"seacc/core/token"
	"strings"
	"time"
)

const template = `// generated from seacc (%s)
#include <stdlib.h>
%s

int seacc_main(int argc, char **argv) {
%sreturn EXIT_SUCCESS; 
}

int main(int argc, char **argv) { 
    return seacc_main(argc, argv); 
}
`

func Gen(t []expr.Node) string {
	b := strings.Builder{}
	main := strings.Builder{}
	for _, c := range t {
		tt := c.GetToken().Type
		if lexer.MatchTokens(tt, token.FUNC, token.CBLOCK, token.PREPROCESSOR) {
			c.CodeGen(&b)
		} else {
			c.CodeGen(&main)
		}
	}
	return fmt.Sprintf(template, time.Now().Format(time.DateTime), b.String(), main.String())
}
