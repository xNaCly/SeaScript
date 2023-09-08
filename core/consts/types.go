package consts

type TypeEnum int8

const (
	NUMBER TypeEnum = iota
	STRING
	ARRAY
	BOOL
)

var TYPE_MAP = map[TypeEnum]string{
	NUMBER: "double",
	STRING: "const char*",
	BOOL:   "bool",
}
