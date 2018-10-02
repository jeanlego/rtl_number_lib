.PHONY: all

#Build everything
all: 
	clang++ -Weverything -ggdb -I src/ src/rtl_int.cpp main.cpp -o rtl_number

clean:
	rm -Rf ./rtl_number

test:
	./verify_librtlnumber.sh
