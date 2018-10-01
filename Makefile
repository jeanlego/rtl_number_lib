.PHONY: all

#Build everything
all: 
	clang++ -Weverything -ggdb rtl_int.cpp main.cpp -o rtl_number

test:
	./verify_librtlnumber.sh
