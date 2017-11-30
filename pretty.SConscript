Import("env")

class term_format:
	default = "\x1b[0m"
	bold = "\x1b[1m"
	black = "\x1b[30m"
	red = "\x1b[31m"
	green = "\x1b[32m"
	yellow = "\x1b[33m"
	blue = "\x1b[34m"
	magenta = "\x1b[35m"
	cyan = "\x1b[36m"
	white = "\x1b[37m"

def cformat(s):
	return s.format(c = term_format)

if ARGUMENTS.get('VERBOSE') != "1":
	env['CCCOMSTR'] = cformat("{c.bold}{c.blue}Compiling{c.white} $TARGET{c.default}")
	env['LINKCOMSTR'] = cformat("{c.bold}{c.blue}Linking{c.white} $TARGET{c.default}")
	env['ARCOMSTR'] = cformat("{c.blue}{c.bold}Creating library{c.white} $TARGET{c.default}")
	env['FWCREATECOMSTR'] = cformat("{c.blue}{c.bold}Creating firmware image{c.white} $TARGET{c.default}")
	env['CREATEKEYCOMSTR'] = cformat("{c.blue}{c.bold}Generating  new firmware signing key{c.white} $TARGET{c.default}")
	env['CREATEBINCOMSTR'] = cformat("{c.blue}{c.bold}Creating binary firmware{c.white} $TARGET{c.default}")