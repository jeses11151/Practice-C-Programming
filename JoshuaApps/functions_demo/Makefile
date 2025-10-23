ENV_INIT := $(HOME)/ql-ol-sdk/ql-ol-crosstool/ql-ol-crosstool-env-init

build: 
	. $(ENV_INIT) && \
	$(MAKE) real-build

real-build: my_stdio_demo

my_stdio_demo: my_stdio_demo.c
	gcc my_stdio_demo.c -o my_stdio_demo

clean:
	rm -f my_stdio_demo
