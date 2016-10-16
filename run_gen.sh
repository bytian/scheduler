#! /usr/bin/env bash

rm -r testcase1
mkdir testcase1

for num_trans in 20000 # number of total transactions
do
	for num_obj in 10 25 50 # number of objects
	do
		for num_trial in 3 # number of actions in each transaction
		do
			for use_power_law in "true" "false" # use power law distribution or unifrom distribution for the popularity of each object
			do
				for lock_type_prob in 0.25 0.5 0.75 1 # the probability for a trasaction to require a write lock
				do
					for power_law_param in 1.5 # the gama parameter for the power law distribution
					do
						for lambda_t in 0.01 0.015 0.005 0.0075 0.0025 # the lambda parameter for the possion distribution of transaction's arrival rate 
						do
							for lambda_a in 0.01 # the lambda parameter for the possion distribution of transaction's next action 
							do
								for version in 1 2 3 4 # generate 10 testcase for each combination of above parameter
								do
									if [ "$use_power_law" = "true" ]; then
										filename="trans_TX${num_trans}O${num_obj}TR${num_trial}LA${lambda_a}LT${lambda_t}LP${lock_type_prob}PL${power_law_param}POWLAW_${version}"
										echo ${filename}
										./gen $num_obj $num_trans $num_trial $lambda_t $lambda_a $use_power_law $lock_type_prob $power_law_param >> testcase1/${filename}
									else
										filename="trans_TX${num_trans}O${num_obj}TR${num_trial}LA${lambda_a}LT${lambda_t}LP${lock_type_prob}PL${power_law_param}UNIFORM_${version}"
										echo ${filename}
										./gen $num_obj $num_trans $num_trial $lambda_t $lambda_a $use_power_law $lock_type_prob $power_law_param >> testcase1/${filename}
									fi
									sleep 1.1
								done
							done
						done
					done
				done
			done
		done
	done	
done

