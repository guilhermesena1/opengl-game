SRC_ROOT=$(shell pwd)
all:
	@make -C src SRC_ROOT=$(SRC_ROOT) all

install:
	@make -C src SRC_ROOT=$(SRC_ROOT) install

clean:
	@make -C src clean
.PHONY: clean
