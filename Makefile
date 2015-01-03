# Include the basic Makefile template
include $(SKULL_SRCTOP)/.Makefile.inc.c

INC = \
    -Isrc \
    -I../../common/c/src

DEPS_LDFLAGS +=
DEPS_LIBS =
TEST_DEPS_LIBS +=

# Objs and deployment related items
SRCS = \
    src/mod.c

TEST_SRCS =

# Include the basic Makefile targets
include $(SKULL_SRCTOP)/.Makefile.targets.c
