all: lib exec

lib:
	$(MAKE) -C lib_dir

exec: lib
	$(MAKE) -C exec_dir

install: lib
	$(MAKE) -C exec_dir install

debug: lib
	$(MAKE) -C exec_dir debug

load: lib
	$(MAKE) -C exec_dir load

clean:
	$(MAKE) -C lib_dir clean
	$(MAKE) -C exec_dir clean

.PHONY: all lib exec install clean
