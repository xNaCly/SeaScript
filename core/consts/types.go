package consts

type TypeEnum int8

const (
	NUMBER TypeEnum = iota
	STRING
)

var TYPE_MAP = map[TypeEnum]string{
	NUMBER: "double",
	STRING: "const char*",
}
