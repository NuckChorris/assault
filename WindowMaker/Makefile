program_NAME := BatteryIndicator.app
program_INCLUDE_DIRS := ../common_c
program_SRCS := $(wildcard *.c)
program_SRCS += $(foreach includedir,$(program_INCLUDE_DIRS),$(wildcard $(includedir)/*.c))
program_OBJS := ${program_SRCS:.c=.o}

CFLAGS += `pkg-config --cflags gtk+-3.0`
CFLAGS += $(foreach includedir,$(program_INCLUDE_DIRS),-I$(includedir))
LDFLAGS += `pkg-config --libs gtk+-3.0`
LDFLAGS += $(foreach includedir,$(program_INCLUDE_DIRS),-L$(includedir))

.PHONY: all clean distclean

all: $(program_NAME)

$(program_NAME): $(program_OBJS)
	$(LINK.cc) $(program_OBJS) -o $(program_NAME)

clean:
	@- $(RM) $(program_NAME)
	@- $(RM) $(program_OBJS)

distclean: clean
