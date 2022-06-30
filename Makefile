SRCS   := $(shell find ./src -maxdepth 1 -name "*.c")
DEPS   := $(shell find ./include -maxdepth 1 -name "*.h") $(SRCS)
CFLAGS += -O1 -ggdb -std=gnu11 -Wall -Werror -Wno-unused-result -Wno-unused-value -Wno-unused-variable

.PHONY: clean

run: $(DEPS) # 64bit binary
	gcc -m64 $(CFLAGS) $(SRCS) -o build/RE

clean:
	rm build/*
