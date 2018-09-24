#!/bin/bash
#Authors: Aaron Graham (aaron.graham@unb.ca, aarongraham9@gmail.com),
#         Jean-Philippe Legault (jlegault@unb.ca, jeanphilippe.legault@gmail.com) and
#         Dr. Kenneth B. Kent (ken@unb.ca)
#          for the Reconfigurable Computing Research Lab at the
#           Univerity of New Brunswick in Fredericton, New Brunswick, Canada

echo -e "\n${0##*/}: Init:"

echo "${0##*/}: # arguments called with ---->  ${@}     "
echo "${0##*/}: # \$1 ---------------------->  $1       "
echo "${0##*/}: # \$2 ---------------------->  $2       "
echo "${0##*/}: # path to me --------------->  ${0}     "
echo "${0##*/}: # parent path -------------->  ${0%/*}  "
echo "${0##*/}: # my name ------------------>  ${0##*/} "

# TODO: Dynamically load in inputs and results from file(s) on disk.
# TODO: Loop and call-out to rtl_number in non-interactive mode
#        for each input/result record in file.

echo -e "\n${0##*/}: ${0%/*}/rtl_number 2 1 + 1:"

OUTPUT_AND_RESULT=`${0%/*}/rtl_number 2 1 + 1`

echo -e "${0##*/}: OUTPUT_AND_RESULT: \n\"\n$OUTPUT_AND_RESULT\n\""

# # trap ctrl-c and call ctrl_c()
# trap ctrl_c INT

# function ctrl_c() {
# #	echo "** Trapped CTRL-C"
# 	exit 0
# }

# file_in=$1
# case $file_in in
# -* )
# 	file_in=$2
# ;; *) ;;
# esac

# echo -e ""

# if [ "`du $file_in | cut -f1 `" -lt "1000000" ]
# then
# #	read -p "\e[30m\e[41m$file_in\e[49m\e[39m will be moved to trash. Continue? (Enter: C, c, Q, q, E, e or CTRL + C to Cancel): " continue
# 	read -p "\"$file_in\" will be moved to trash. Continue? (Enter: C, c, Q, q, E, e or CTRL + C to Cancel): " continue
# 	case $continue in
# 		[CcQqEe]* ) exit;;
# 		* )  ;;
# 	esac
# 	trash -v $file_in
# else
# #	read -p "\e[30m\e[41m$file_in\e[49m\e[39m will be permanently deleted! Continue? (Enter: C, c, Q, q, E, e or CTRL + C to Cancel): " continue
# 	read -p "\"$file_in\" will be permanently deleted! Continue? (Enter: C, c, Q, q, E, e or CTRL + C to Cancel): " continue
# 	case $continue in
# 		[CcQqEe]* ) exit;;
# 		* )  ;;
# 	esac
# 	rm -Rfv $file_in || rm -fv $file_in
# fi

echo -e "${0##*/}: End.\n"

exit 0


