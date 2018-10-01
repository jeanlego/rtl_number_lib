#!/bin/bash
#Authors: Aaron Graham (aaron.graham@unb.ca, aarongraham9@gmail.com),
#         Jean-Philippe Legault (jlegault@unb.ca, jeanphilippe.legault@gmail.com) and
#         Dr. Kenneth B. Kent (ken@unb.ca)
#          for the Reconfigurable Computing Research Lab at the
#           Univerity of New Brunswick in Fredericton, New Brunswick, Canada

# trap ctrl-c and call ctrl_c()
trap ctrl_c INT

TOTAL_TEST_RAN=0
ANY_FAILURES="false"
FAILURE_COUNT=0

function ctrl_c() {
    echo -e "\n\n** EXITED FORCEFULLY **\n\n"
    echo -e "${0##*/}: $TOTAL_TEST_RAN Tests Ran; $FAILURE_COUNT Test Failures.\n"
    exit $((0-FAILURE_COUNT))
}

echo -e "\n${0##*/}: Init:"

# Dynamically load in inputs and results from
#  file(s) on disk.
for INPUT in ${0%/*}/regression_tests/*.test.csv; do
    [ ! -f $INPUT ] && { echo "$INPUT file not found"; exit 99; }

    echo -e "${0##*/}: Running Test File: $INPUT:"

    # Need to escape any special characters in RTL_NUMBER_ARGUMENTS
    #        (e.g. * (multiplication), etc.)
    set -f

    # Set "," as the field separator using $IFS 
    # and read line by line using while read combo 
    while IFS=',' read -a arr
    do
        # for i in ${arr[@]}; do echo $i; done
        # TEST_LABEL=${arr[0]}
        # echo -e "${0##*/}: TEST_LABEL: \n\"\n$TEST_LABEL\n\""
        # EXPECTED_RESULT=${arr[-1]}
        # echo -e "${0##*/}: EXPECTED_RESULT: \n\"\n$EXPECTED_RESULT\n\""

        ARR_LENGTH=${#arr[@]}

        # echo -e "${0##*/}: ARR_LENGTH: $ARR_LENGTH"

        ARR_END_INDEX=$((ARR_LENGTH-1))

        # echo -e "${0##*/}: ARR_END_INDEX: $ARR_END_INDEX"

        TEST_LABEL="" # TODO: Handle Labels with spaces
        EXPECTED_RESULT=""
        RTL_NUMBER_ARGUMENTS=""

        EXECUTE_CONTINUE="false"

        INDEX=0
        for i in ${arr[@]}; do
            # echo ${INDEX}_$i

            # Skip Comments
            if [[ "$i" =~ ^#.*$ ]]
            then
                # echo -e "${0##*/}: Ignoring Comment Line. Continuing..."
                EXECUTE_CONTINUE="true"
                break # continue to next outer loop iteration
            elif [ "$INDEX" -eq "0" ]
            then
                TEST_LABEL=$i

                # echo -e "${0##*/}: TEST_LABEL: \n\"\n$TEST_LABEL\n\""
            elif [ "$INDEX" -eq "$ARR_END_INDEX" ]
            then
                EXPECTED_RESULT=${arr[-1]}

                # echo -e "${0##*/}: EXPECTED_RESULT: \n\"\n$EXPECTED_RESULT\n\""
            else
                RTL_NUMBER_ARGUMENTS+=" "
                RTL_NUMBER_ARGUMENTS+=$i
            fi
            let INDEX=${INDEX}+1
        done

        # continue to next loop iteration
        if [ "$EXECUTE_CONTINUE" == "true" ]
        then
            # echo -e "${0##*/}: Ignoring Comment Line. Continuing..."
            continue
        fi

        # echo -e "${0##*/}: RTL_NUMBER_ARGUMENTS: \n\"\n$RTL_NUMBER_ARGUMENTS\n\""

        TOTAL_TEST_RAN=$((TOTAL_TEST_RAN+1))

        # echo -e "${0##*/}: ${0%/*}/rtl_number$RTL_NUMBER_ARGUMENTS:"

        OUTPUT_AND_RESULT=`${0%/*}/rtl_number$RTL_NUMBER_ARGUMENTS`

        # echo -e "${0##*/}: OUTPUT_AND_RESULT: \n\"\n$OUTPUT_AND_RESULT\n\""

        # Parse out any ERRORS from OUTPUT_AND_RESULT
        #  and handle accordingly
        ERRORS=`echo "$OUTPUT_AND_RESULT" | grep -i error`

        # echo -e "${0##*/}: ERRORS: \n\"\n$ERRORS\n\""

        # Parse out result from OUTPUT_AND_RESULT
        #  and compare to expected reults from file
        RESULT=`echo "$OUTPUT_AND_RESULT" | grep -i $EXPECTED_RESULT`

        # echo -e "${0##*/}: RESULT: \n\"\n$RESULT\n\""

        SUCCESS="false"

        if [ ! -z "$ERRORS" ]
        then
            # echo -e "${0##*/}: Errors detected!"
            SUCCESS="false"
        elif [ -z "$RESULT" ]
        then
            SUCCESS="false"
        # Check that Expected Result isn't empty
        elif [ -z "$EXPECTED_RESULT" ]
        then
            echo -e "${0##*/}: ERROR: Expected Result Empty!"
            SUCCESS="false"
        elif [ "$EXPECTED_RESULT" == "$RESULT" ]
        then
            SUCCESS="true"
        # This last case shouldn't happen
        else
            SUCCESS="false"
        fi

        # echo -e "${0##*/}: SUCCESS?: $SUCCESS"

        if [ "$SUCCESS" == "true" ]
        then
            echo -e " --- PASSED == $TEST_LABEL"
        else
            ANY_FAILURES="true"
            FAILURE_COUNT=$((FAILURE_COUNT+1))
            echo -e " -X- FAILED == $TEST_LABEL"
            echo -e "$TEST_LABEL: Called: ${0%/*}/rtl_number$RTL_NUMBER_ARGUMENTS:"
            echo -e "$TEST_LABEL: Expected: \"$EXPECTED_RESULT\""
            echo -e "$TEST_LABEL: Got: \"$OUTPUT_AND_RESULT\""
        fi
    done < "$INPUT"
    #  Re-Enable Bash Wildcard Expanstion '*' 
    set +f
done

echo -e "${0##*/}: $TOTAL_TEST_RAN Tests Ran; $FAILURE_COUNT Test Failures."

if [ "$ANY_FAILURES" == "false" ]
then
    echo -e "${0##*/}: End.\n"
    exit 0
else
    if [ "$FAILURE_COUNT" -gt "127" ]
    then
        echo -e"\n${0##*/}: WARNING: Return Code may be unreliable: More than 127 Failures!\n"
    fi
    echo -e "${0##*/}: End.\n"
    exit $((0-FAILURE_COUNT))
fi
