#!/bin/bash

# Define the trace files
trace_files=("gcc.trace" "read01.trace" "read02.trace" "read03.trace" "swim.trace" "write01.trace" "write02.trace")

# Define cache policies
eviction_policies=("lru" "fifo")

# Define set sizes and blocks per set configurations
set_sizes=(1 2 4 8 16 32 64 128 256)
blocks_per_set=(1 2 4)

# Output file
output_file="cache_simulations_output.txt"

# Clear the output file
> "$output_file"

# Loop over all combinations of configurations and trace files
for trace in "${trace_files[@]}"; do
    for set_size in "${set_sizes[@]}"; do
        for block_per_set in "${blocks_per_set[@]}"; do
            for eviction_policy in "${eviction_policies[@]}"; do
                # Write-allocate with write-back
                echo "Configuration: sets=$set_size, blocks=$block_per_set, policy=write-allocate, method=write-back, eviction=$eviction_policy, trace=$trace" >> "$output_file"
                ./csim "$set_size" "$block_per_set" "16" "write-allocate" "write-back" "$eviction_policy" < "$trace" >> "$output_file"
                echo "----------------------------------------" >> "$output_file"
                
                # Write-allocate with write-through
                echo "Configuration: sets=$set_size, blocks=$block_per_set, policy=write-allocate, method=write-through, eviction=$eviction_policy, trace=$trace" >> "$output_file"
                ./csim "$set_size" "$block_per_set" "16" "write-allocate" "write-through" "$eviction_policy" < "$trace" >> "$output_file"
                echo "----------------------------------------" >> "$output_file"
                
                # No-write-allocate with write-through
                echo "Configuration: sets=$set_size, blocks=$block_per_set, policy=no-write-allocate, method=write-through, eviction=$eviction_policy, trace=$trace" >> "$output_file"
                ./csim "$set_size" "$block_per_set" "16" "no-write-allocate" "write-through" "$eviction_policy" < "$trace" >> "$output_file"
                echo "----------------------------------------" >> "$output_file"
            done
        done
    done
done

echo "Simulation runs complete. Output is saved in $output_file."
