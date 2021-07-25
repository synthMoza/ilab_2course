#! /bin/bash

# Declare variables
RED='\033[0;31m'
GREEN='\033[0;32m'
BLUE='\033[0;34m'
NC='\033[0m'
PURPLE='\033[0;35m'
EXEC=$1
DIR='tests'

count=0;
count_success=0;
count_failed=0;

if [ -z $1 ] ; then
    echo "Usage: ./launch_tests.sh <executable_file>"
    exit 1
fi

EXEC=$1

echo -e "-------------ParaCL Test Script-------------" 
for file in tests/*.test; do
	filename=$(basename -- "$file")
	echo -e ${BLUE}
    echo -e "[TEST_$count] \t ${NC} \n[NAME] \t\t ${PURPLE} $filename ${NC}"

    # Define input and output filenames
    input_file_name="${DIR}/${filename%.*}_input.txt";
    output_file_name="${DIR}/${filename%.*}_output.txt";
    output="${DIR}/output.txt";
    
    # Launch the test with this input
    >$output; # clear the output file
	valgrind --leak-check=full ./$EXEC $file < "$input_file_name" > $output
	# Check the output
	if cmp -s "$output" "$output_file_name"; then
		echo -e "[RESULT] \t ${GREEN} SUCCESS ${NC}"
		((count_success=count_success+1))
	else
		echo -e "[RESULT] \t ${RED} FAIL ${NC}"
		((count_failed=count_failed+1))
	fi

    ((count=count+1))
done

	echo -e "${PURPLE}"
	echo -e "Successful tests: ${count_success}, failed tests: ${count_failed}"

rm "$output"