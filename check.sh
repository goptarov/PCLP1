#!/bin/bash

NUM_TASKS=7
NUM_TESTS_PER_TASK=(2 3 3 5 5 6 5)
TOTAL=0
WORKING_DIR=`pwd`
array_points=(0 0 0 0 0 0 0)
max_array_points=(20 15 15 20 20 30 10)
make

function check_test() {
	in_path=$1
	out_path=$2
	ref_path=$3
	test_num=$4
	task_num=$5
	exec=pachete

	if test $4 -le 9; then 
		echo -n "Test:  $test_num ...................... "
	else echo -n "Test: $test_num ...................... "
	fi

	./$exec < $in_path > $out_path

	# Check the result
	diff $ref_path $out_path 2>&1 1> my_diff

	if test $? -eq 0; then
		echo "PASS"
		array_points[$((task_num-1))]=$(echo "scale=4; ${array_points[$((task_num-1))]} + $((max_array_points[task_num - 1] / NUM_TESTS_PER_TASK[task_num - 1]))" | bc)
	else
		echo "FAILED"
	fi
}

function run_tests_for_task() {
	TASK_NUM=$1
	NUM_TESTS=${NUM_TESTS_PER_TASK[$((TASK_NUM-1))]}
		
	if test ! -x "pachete"; then
		echo "Executable pachete is missing"
		return 1
	fi

	input="$WORKING_DIR/tests/postasi/in"
	ref="$WORKING_DIR/tests/postasi/ref"
	output="$WORKING_DIR/tests/postasi/out"

	if [ ! -d "$output" ]; then
		mkdir $output
	fi

	echo "===== Running tests for task $TASK_NUM ======="
	for ((i=1;i<=$NUM_TESTS;i++)); do
		test_input="$input/test"$i"_task"$TASK_NUM".in"
		test_output="$output/test"$i"_task"$TASK_NUM".out"
		test_ref="$ref/test"$i"_task"$TASK_NUM".ref"
		check_test $test_input $test_output $test_ref $i $TASK_NUM
	done
	echo "====== You got ${array_points[$((TASK_NUM-1))]} out of ${max_array_points[$((TASK_NUM-1))]} for task $TASK_NUM ======="

	echo ""
}

function run_all() {
	for ((j=1;j<=$NUM_TASKS;j++)); do
		run_tests_for_task $j
	done
}

if [ $# -eq 0 ]; then
	run_all
else 
	if [ "$1" -gt "$NUM_TASKS" ] || [ "$1" -lt "1" ]; then
		echo "Invalid problem number"
		exit 1
	fi

	run_tests_for_task $1
fi

# And the result is ...

for ((k=0;k<$NUM_TASKS;k++)); do
	TOTAL=$(echo "scale=4; ${array_points[$k]} + $TOTAL" | bc)
done


README_FILE=$(find "$WORKING_DIR" -maxdepth 1 -type f -name 'README*')

if [ -z "$README_FILE" ]; then
    echo -e "-10: README file is missing.\n"
    TOTAL=$(echo "scale=4; $TOTAL - 10" | bc)
elif [ ! -s "$README_FILE" ]; then
    echo -e "-10: README file is empty.\n"
    TOTAL=$(echo "scale=4; $TOTAL - 10" | bc)
fi


# And the result is ...
echo "TOTAL: $TOTAL/130"
echo ""
make clean
rm my_diff

