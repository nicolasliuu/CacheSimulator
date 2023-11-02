#!/bin/bash

# Define the trace files
trace_files=("gcc.trace" "read01.trace" "read02.trace" "read03.trace" "swim.trace" "write01.trace" "write02.trace")

# Define cache policies
eviction_policies=("lru" "fifo")
write_allocate_policies=("write-allocate" "no-write-allocate")
write_through_back_policies=("write-through" "write-back")

# Define set sizes and blocks per set configurations
set_sizes=(1 2 4 8 16 32 64 128 256)
blocks_per_set=(1 2 4)

# Output file
output_file="cache_simulations_output.txt"

# Clear the output file
> "$output_file"

# Function to calculate and echo percentages
calculate_percentages() {
    local total_loads=$1
    local load_hits=$2
    local total_stores=$3
    local store_hits=$4
    local load_hit_percentage
    local store_hit_percentage

    # Check for divide by zero
    if [ "$total_loads" -eq 0 ]; then
        load_hit_percentage="N/A"
    else
        load_hit_percentage=$(bc <<< "scale=2; ($load_hits/$total_loads)*100")
    fi

    if [ "$total_stores" -eq 0 ]; then
        store_hit_percentage="N/A"
    else
        store_hit_percentage=$(bc <<< "scale=2; ($store_hits/$total_stores)*100")
    fi

    echo "Load Hit Percentage: $load_hit_percentage%"
    echo "Store Hit Percentage: $store_hit_percentage%"
}

# Main loop for each trace file
for trace in "${trace_files[@]}"; do
    # Array to hold the outputs for sorting
    declare -A output_blocks

    # Loop over all combinations of configurations
    for set_size in "${set_sizes[@]}"; do
        for block_per_set in "${blocks_per_set[@]}"; do
            for eviction_policy in "${eviction_policies[@]}"; do
                for write_allocate in "${write_allocate_policies[@]}"; do
                    for write_policy in "${write_through_back_policies[@]}"; do
                        # Skip the invalid configuration
                        if [ "$write_allocate" == "no-write-allocate" ] && [ "$write_policy" == "write-back" ]; then
                            continue
                        fi
                        
                        # Run the simulation and capture the output
                        simulation_output=$(./csim "$set_size" "$block_per_set" "16" "$write_allocate" "$write_policy" "$eviction_policy" < "$trace")
                        
                        # Parse the output for the required values
                        total_loads=$(echo "$simulation_output" | grep "Total loads" | awk '{print $3}')
                        load_hits=$(echo "$simulation_output" | grep "Load hits" | awk '{print $3}')
                        total_stores=$(echo "$simulation_output" | grep "Total stores" | awk '{print $3}')
                        store_hits=$(echo "$simulation_output" | grep "Store hits" | awk '{print $3}')
                        total_cycles=$(echo "$simulation_output" | grep "Total cycles" | awk '{print $3}')
                        
                        # Calculate percentages and construct the output block
                        output_block="$(echo "Configuration: sets=$set_size, blocks=$block_per_set, eviction=$eviction_policy, write_alloc=$write_allocate, write_policy=$write_policy, trace=$trace")\n"
                        output_block+="$(echo "$simulation_output")\n"
                        output_block+="$(calculate_percentages $total_loads $load_hits $total_stores $store_hits)\n"
                        output_block+="$(echo "Total Cycles: $total_cycles")\n"
                        output_block+="$(echo "----------------------------------------")\n"

                        # Store the block with the cycle count for sorting
                        output_blocks["$total_cycles"]="$output_block"
                    done
                done
            done
        done
    done

    # Sort the output blocks by total cycle count and append to the final output file
    for cycle_count in $(echo "${!output_blocks[@]}" | tr ' ' '\n' | sort -n); do
        echo -e "${output_blocks[$cycle_count]}" >> "$output_file"
    done

    # Clear the output_blocks array
    unset output_blocks
done

# The final output file is now sorted by cycle counts for each trace file configuration
