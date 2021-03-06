#
# Main plumCore build file
#
# Copyright (c) 2017, Marek Koza (qyx@krtko.org)
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# 1. Redistributions of source code must retain the above copyright notice, this
#    list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright notice,
#    this list of conditions and the following disclaimer in the documentation
#    and/or other materials provided with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
# ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
# ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


import os
import time
import yaml

# Create default environment and export it. It will be modified later
# by port-specific and other build scripts.
env = Environment()
Export("env")

# Some helpers to make the build looks pretty
SConscript("pretty.SConscript")

env.Append(ENV = os.environ)

## @todo Check for the nanopb installation. Request download and build of the library
##       if it is not properly initialized.
def build_proto(target, source, env):
	for s in source:
		print "Building proto %s" % s
		d = os.path.dirname(str(s))
		os.system("protoc --plugin=lib/other/nanopb/generator/protoc-gen-nanopb --proto_path=%s --nanopb_out=%s --proto_path=lib/other/nanopb/generator/proto %s" % (d, d, s))



# Load build configuration
env["CONFIG"] = yaml.load(file("config/default.yaml", "r"))
env["PORT"] = env["CONFIG"]["build"]["port"];

# Examine the Git repository and build the version string
SConscript("version.SConscript")

env["PORTFILE"] = "bin/plumcore-" + env["PORT"] + "-" + env["VERSION"]


objs = []

objs.append(SConscript("ports/%s/SConscript" % env["PORT"]))



env["CC"] = "%s-gcc" % env["TOOLCHAIN"]
env["CXX"] = "%s-g++" % env["TOOLCHAIN"]
env["AR"] = "%s-ar" % env["TOOLCHAIN"]
env["AS"] = "%s-as" % env["TOOLCHAIN"]
env["LD"] = "%s-gcc" % env["TOOLCHAIN"]
env["OBJCOPY"] = "%s-objcopy" % env["TOOLCHAIN"]
env["OBJDUMP"] = "%s-objdump" % env["TOOLCHAIN"]
env["SIZE"] = "%s-size" % env["TOOLCHAIN"]
env["OOCD"] = "openocd"
env["CREATEFW"] = "tools/createfw.py"

# Add uMeshFw HAL
env.Append(CPPPATH = [
	Dir("hal/common"),
	Dir("hal/modules"),
	Dir("hal/interfaces"),
])
objs.append(env.Object(source = [
	File(Glob("hal/common/*.c")),
	File(Glob("hal/modules/*.c")),
	File(Glob("hal/interfaces/*.c")),
]))

objs.append(SConscript("uhal/SConscript"))
objs.append(SConscript("system/SConscript"))
objs.append(SConscript("protocols/SConscript"))
objs.append(SConscript("lib/SConscript"))
objs.append(SConscript("services/SConscript"))


# Required to allow including things like "services/cli.h"
env.Append(CPPPATH = [Dir(".")])

env.Append(LINKFLAGS = [
	env["CFLAGS"],
	"--static",
	"-nostartfiles",
	"--specs=nano.specs",
	"-T", env["LDSCRIPT"],
	"-Wl,-Map=%s.map" % env["PORTFILE"],
	"-Wl,--gc-sections",
])

env.Append(CFLAGS = [
	"-Os",
	"-g",
	#~ "-flto",
	"-fno-common",
	"-fdiagnostics-color=always",
	"-ffunction-sections",
	"-fdata-sections",
	"-fdiagnostics-color=always",
	"--std=gnu99",
	"-Wall",
	"-Wextra",
	"-pedantic",
	#~ "-Werror",
	"-Winit-self",
	"-Wreturn-local-addr",
	"-Wswitch-default",
	"-Wuninitialized",
	"-Wundef",
	#~ "-Wstack-usage=256",
	"-Wshadow",
	"-Wimplicit-function-declaration",
	"-Wcast-qual",
	# "-Wwrite-strings",
	# "-Wconversion",
	"-Wlogical-op",
	"-Wmissing-declarations",
	"-Wno-missing-field-initializers",
	"-Wstack-protector",
	"-Wredundant-decls",
	"-Wmissing-prototypes",
	"-Wstrict-prototypes",
])

# link the whole thing
elf = env.Program(
	source = objs,
	target = [
		File(env["PORTFILE"] + ".elf"),
		File(env["PORTFILE"] + ".map"),
	],
	LIBS = [
		env["LIBOCM3"],
		"c",
		"gcc",
		"nosys",
	]
)

# Create the firmware image file
env["FW_SIGNING_KEY"] = env["CONFIG"]["firmware"]["signing-key"];
env["FW_CREATE_KEY"] = env["CONFIG"]["firmware"]["create-key"];

SConscript("firmware.SConscript")

proto = env.Command(
	source = [
		File(Glob("protocols/umesh/proto/*.proto")),
		File(Glob("protocols/uxb/*.proto")),
	],
	target = "protoc",
	action = build_proto
)

elfsize = env.Command(source = elf, target = "elfsize", action = "$SIZE $SOURCE")

program = env.Command(
	source = env["PORTFILE"] + ".fw",
	target = "program",
	action = """
	$OOCD \
	-s /usr/share/openocd/scripts/ \
	-f interface/%s.cfg \
	-f target/%s.cfg \
	-c "init" \
	-c "reset init" \
	-c "flash write_image erase $SOURCE 0x08010000 bin" \
	-c "reset" \
	-c "shutdown"
	""" % (env["OOCD_INTERFACE"], env["OOCD_TARGET"])
)

# And do something by default.
env.Alias("umeshfw", source = env["PORTFILE"] + ".fw")
env.Alias("proto", proto);
Default(env["PORTFILE"] + ".fw")

