all: compile run
compile:
	python3 ./scripts/build.py --generate --build
run:
	python3 ./scripts/build.py --run
