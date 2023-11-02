#!/bin/bash

# Define the trace files
trace_files=("gcc.trace" "read01.trace" "read02.trace" "read03.trace" "swim.trace" "write01.trace" "write02.trace")

# Define cache policies
eviction_policies=("lru" "fifo")

# Define set sizes and blocks per set configurations
set_sizes=(1 2 4 8 16 32 64 128 256)
blocks_per_set=(1 2 4)

# Output file
output_file="cache_simulations_output2.txt"

# Clear the output file
> "$output_file"

# Function to calculate and echo percentages
calculate_percentages() {
    local total_loads=$1
    local load_hits=$2
    local total_stores=$3
    local store_hits=$4

    local load_hit_percentage="N/A"
    local store_hit_percentage="N/A"

    # Calculate load hit percentage if applicable
    if [ "$total_loads" -ne 0 ]; then
        load_hit_percentage=$(bc <<< "scale=2; ($load_hits/$total_loads)*100")
    fi

    # Calculate store hit percentage if applicable
    if [ "$total_stores" -ne 0 ]; then
        store_hit_percentage=$(bc <<< "scale=2; ($store_hits/$total_stores)*100")
    fi

    echo "Load Hit Percentage: $load_hit_percentage%"
    echo "Store Hit Percentage: $store_hit_percentage%"
}

# Loop over all combinations of configurations and trace files
for trace in "${trace_files[@]}"; do
    for set_size in "${set_sizes[@]}"; do
        for block_per_set in "${blocks_per_set[@]}"; do
            for eviction_policy in "${eviction_policies[@]}"; do
                # Run the simulation and capture the output
                simulation_output=$(./csim "$set_size" "$block_per_set" "16" "write-allocate" "write-back" "$eviction_policy" < "$trace")
                
                # Parse the output for the required values
                total_loads=$(echo "$simulation_output" | grep "Total loads" | awk '{print $3}')
                load_hits=$(echo "$simulation_output" | grep "Load hits" | awk '{print $3}')
                total_stores=$(echo "$simulation_output" | grep "Total stores" | awk '{print $3}')
                store_hits=$(echo "$simulation_output" | grep "Store hits" | awk '{print $3}')
                total_cycles=$(echo "$simulation_output" | grep "Total cycles" | awk '{print $3}')

                # Configuration string
                config_str="sets=$set_size, blocks=$block_per_set, policy=$eviction_policy, trace=$trace"

                # Store the results in an associative array with the cycle count as the key
                cycle_counts["$total_cycles"]="$(echo -e "Configuration: $config_str\n$simulation_output\n$(calculate_percentages $total_loads $load_hits $total_stores $store_hits)\n----------------------------------------")"
            done
        done
    done
done

# Sort the results based on cycle counts and write them to the output file
for key in "${!cycle_counts[@]}"; do
    echo -e "${cycle_counts[$key]}" >> "$output_file"
done | sort -n -k3 > "$output_file"
