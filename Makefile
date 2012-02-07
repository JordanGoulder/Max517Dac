all: docs

.PHONY: docs
docs:
	doxygen ./docs/Doxyfile

clean:
	rm -rf ./docs/html
