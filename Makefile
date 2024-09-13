# compiler setup
CC=gcc
CFLAGS=-Wall -Wextra -std=c99

# define targets
TARGETS = image_editor

build: $(TARGETS)

image_editor: image_editor.c
	$(CC) $(CFLAGS) image_editor.c -lm -o image_editor
pack:
	zip -FSr 314CA_CucuViorel-Cosmin_Tema3.zip README Makefile *.c *.h

clean:
	rm -f $(TARGETS)

.PHONY: pack clean