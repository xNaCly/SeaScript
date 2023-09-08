package consts

type TypeEnum int8

const (
	NUMBER TypeEnum = iota
	UNKNOWN
	CUSTOM
	STRING
	ARRAY
	BOOL
)

var TYPE_MAP = map[TypeEnum]string{
	NUMBER: "double",
	STRING: "const char*",
	BOOL:   "bool",
}

func ComputeType(in string) (t TypeEnum, isArray bool) {
	t = UNKNOWN
	if in[0] == '[' && in[1] == ']' {
		in = in[1:]
		isArray = true
	}
	switch in {
	case "string":
		t = STRING
	case "number":
		t = NUMBER
	default:
		t = CUSTOM
	}
	return
}
