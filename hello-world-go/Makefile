all: clean build

build:
	go generate
	go build

clean:
	rm -f *.o hello-world-go

run: build
	sudo ./hello-world-go

.PHONY: all build clean run
