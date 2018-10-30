#Authors: Aaron Graham (aaron.graham@unb.ca, aarongraham9@gmail.com),
#         Jean-Philippe Legault (jlegault@unb.ca, jeanphilippe.legault@gmail.com) and
#          Dr. Kenneth B. Kent (ken@unb.ca)
#          for the Reconfigurable Computing Research Lab at the
#           Univerity of New Brunswick in Fredericton, New Brunswick, Canada

SRC=

.PHONY: all

#Build everything
all: 
	clang++ -Weverything -ggdb -I src/ src/*.cpp main.cpp -o rtl_number

clean:
	rm -Rf ./rtl_number

test:
	./verify_librtlnumber.sh
