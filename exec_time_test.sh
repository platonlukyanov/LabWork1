#!/bin/bash

LC_NUMERIC=C
INPUT_FILE="image.bmp"

# Function to calculate statistics
calculate_stats() {
    local -n arr=$1
    local sum=0
    local min=${arr[0]}
    local max=${arr[0]}
    
    for t in "${arr[@]}"
    do
        sum=$(echo "$sum + $t" | bc)
        # Update minimum
        if (( $(echo "$t < $min" | bc -l) )); then
            min=$t
        fi
        # Update maximum
        if (( $(echo "$t > $max" | bc -l) )); then
            max=$t
        fi
    done
    local average=$(echo "scale=6; $sum / ${#arr[@]}" | bc)
    echo "$average $min $max"
}

declare -a times

# Average out single attempt
for i in {1..100}
do
    start=$(date +%s.%N)
    ./main rotate-clockwise $INPUT_FILE output.bmp
    end=$(date +%s.%N)
    time=$(echo "$end - $start" | bc)
    times+=($time)
    echo "Attempt $i: $time seconds"
done

read average min max < <(calculate_stats times)
echo "Average time: $average seconds"
echo "Minimum time: $min seconds"
echo "Maximum time: $max seconds"

# Function to run 100 attempts
run_100_attempts() {
    for i in {1..100}
    do
        ./main rotate-clockwise $INPUT_FILE output.bmp
    done
}

# Measure time for 10 sets of 100 attempts
declare -a batch_times
for batch in {1..10}
do
    start=$(date +%s.%N)
    run_100_attempts
    end=$(date +%s.%N)
    time=$(echo "$end - $start" | bc)
    batch_times+=($time)
    echo "Batch $batch (100 attempts): $time seconds"
done

# Calculate statistics for batches
read average min max < <(calculate_stats batch_times)
echo "Batch statistics:"
echo "Average time per 100 attempts: $average seconds"
echo "Minimum time per 100 attempts: $min seconds"
echo "Maximum time per 100 attempts: $max seconds"
