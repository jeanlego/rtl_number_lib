#!/bin/bash
#Authors: Aaron Graham (aaron.graham@unb.ca, aarongraham9@gmail.com),
#         Jean-Philippe Legault (jlegault@unb.ca, jeanphilippe.legault@gmail.com) and
#          Dr. Kenneth B. Kent (ken@unb.ca)
#          for the Reconfigurable Computing Research Lab at the
#           Univerity of New Brunswick in Fredericton, New Brunswick, Canada

# trap ctrl-c and call ctrl_c()
trap ctrl_c INT

TOTAL_TEST_RAN=0
FAILURE_COUNT=0
DEBUG=0

function ctrl_c() {
    FAILURE_COUNT=$((FAILURE_COUNT+1))
	exit_code ${FAILURE_COUNT} "\n\n** EXITED FORCEFULLY **\n\n"
}

function exit_code() {
	#print passed in value
	echo -e $2
	my_failed_count=$1
	echo -e "$TOTAL_TEST_RAN Tests Ran; $my_failed_count Test Failures.\n"
	[ "$my_failed_count" -gt "127" ] && echo "WARNING: Return Code may be unreliable: More than 127 Failures!"
	echo "End."
	exit ${my_failed_count}
}

# Dynamically load in inputs and results from
#  file(s) on disk.
for INPUT in ${0%/*}/regression_tests/*.csv; do
	[ ! -f $INPUT ] && exit_code 99 "$INPUT file not found\n" 

	echo "Running Test File: $INPUT:"

	while IFS= read -r input_line; do

		#glob whitespace from line and remove everything after comment
		input_line=$(echo ${input_line} | tr -d '[:space:]' | cut -d '#' -f1) 

		#skip empty lines
		[  "_" ==  "_${input_line}" ] && continue

		#split csv
		IFS="," read -ra arr <<< ${input_line}
		len=${#arr[@]}


		if 	[ ${len} != "4" ] &&		# unary
			[ ${len} != "5" ] &&		# binary
			[ ${len} != "7" ]; then		# ternary
				[ ! -z ${DEBUG} ] && echo "Malformed line is csv file: ${input_line} Skipping"
				continue
		fi

		TEST_LABEL=${arr[0]}
		EXPECTED_RESULT=${arr[$(( len -1 ))]}
		# everything between is the operation to pipe in so we slice the array and concatenate with space

		#deal with multiplication
		set -f
		RTL_CMD_IN=$(printf "%s " "${arr[@]:1:$(( len -2 ))}")	
		

		TOTAL_TEST_RAN=$(( TOTAL_TEST_RAN+1 ))
		OUTPUT_AND_RESULT=$(${0%/*}/rtl_number ${RTL_CMD_IN})

		# Parse out any ERRORS from OUTPUT_AND_RESULT
		#  and handle accordingly
		if 	[ "_$OUTPUT_AND_RESULT" != "_" ] &&
			[ "_" == "_$(echo $OUTPUT_AND_RESULT | grep ERROR)" ]; then

				test_output_args="${OUTPUT_AND_RESULT} === ${EXPECTED_RESULT}"
				TEST_OUTPUT=$(${0%/*}/rtl_number ${test_output_args})

				if 	[ "_$TEST_OUTPUT" != "_" ] &&
				[ "1'b1" == "$TEST_OUTPUT" ]; then
					echo "--- PASSED == $TEST_LABEL"
				else
					FAILURE_COUNT=$((FAILURE_COUNT+1))
					echo -e "-X- FAILED == $TEST_LABEL\t  ./rtl_number ${RTL_CMD_IN}\t CMP=${TEST_OUTPUT} Output:<$OUTPUT_AND_RESULT> != <$EXPECTED_RESULT>"
				fi
		else
			FAILURE_COUNT=$((FAILURE_COUNT+1))
			echo -e "-X- FAILED == $TEST_LABEL\t  ./rtl_number ${RTL_CMD_IN}\t  Output:<$OUTPUT_AND_RESULT> != <$EXPECTED_RESULT>"
		fi

		#unset the multiplication token override
		unset -f

	done < "$INPUT"
	#  Re-Enable Bash Wildcard Expanstion '*' 
	set +f
done

exit_code ${FAILURE_COUNT} "Completed Tests\n"
