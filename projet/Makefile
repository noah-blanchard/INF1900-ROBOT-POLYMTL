all: lib exec

lib:
	$(MAKE) -C lib

exec: lib
	$(MAKE) -C app

install: lib
	$(MAKE) -C app install

debug: lib
	$(MAKE) -C app debug

load: lib
	$(MAKE) -C app load

clean:
	$(MAKE) -C lib clean
	$(MAKE) -C app clean

.PHONY: all lib exec install clean
