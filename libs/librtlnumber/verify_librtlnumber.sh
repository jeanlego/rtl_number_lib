#!/bin/bash
#Authors: Aaron Graham (aaron.graham@unb.ca, aarongraham9@gmail.com),
#         Jean-Philippe Legault (jlegault@unb.ca, jeanphilippe.legault@gmail.com) and
#         Dr. Kenneth B. Kent (ken@unb.ca)
#          for the Reconfigurable Computing Research Lab at the
#           Univerity of New Brunswick in Fredericton, New Brunswick, Canada

echo -e "\n${0##*/}: Init:"

# echo "${0##*/}: # arguments called with ---->  ${@}     "
# echo "${0##*/}: # \$1 ---------------------->  $1       "
# echo "${0##*/}: # \$2 ---------------------->  $2       "
# echo "${0##*/}: # path to me --------------->  ${0}     "
# echo "${0##*/}: # parent path -------------->  ${0%/*}  "
# echo "${0##*/}: # my name ------------------>  ${0##*/} "



# TODO: Dynamically load in inputs and results from
#        file(s) on disk.

# TODO: Loop and call-out to rtl_number in non-interactive
#        mode for each input/result record in file.

echo -e "\n${0##*/}: ${0%/*}/rtl_number 2 1 + 1:"

OUTPUT_AND_RESULT=`${0%/*}/rtl_number 2 1 + 1`

echo -e "${0##*/}: OUTPUT_AND_RESULT: \n\"\n$OUTPUT_AND_RESULT\n\""

# TODO: Parse out any ERRORS from OUTPUT_AND_RESULT
#        and hand accordingly

# TODO: Parse out result from OUTPUT_AND_RESULT
#        and compare to expected reults from file








# # trap ctrl-c and call ctrl_c()
# trap ctrl_c INT

# function ctrl_c() {
# #	echo "** Trapped CTRL-C"
# 	exit 0
# }

echo -e "${0##*/}: End.\n"

exit 0


