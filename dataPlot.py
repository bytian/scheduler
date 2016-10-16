import sys, os
import matplotlib.pyplot as plt

version_num = 4.0
random_repeat = 10.0

LatencyCollector = {}
WaitTimeCollector = {}
schedulerCollector = {}

Latency = {}
WaitTime = {}

# read the file and gather the data for each scheduler
with open('result') as file:
	for line in file:
		if (line != "\n"):
			words = line.split(' ')
			testcase = words[1].split('/')[1]
			scheduler = words[2]

			# use only the first 4 verstions for each testcase if there are more than 4
			if (float(testcase.split('_')[1]) > 4) 
				continue

			latency = float(words[3])
			waittime = float(words[4])

			# gather the data into each testcase/scheduler combo
			if (scheduler.split('/')[0] == "random"):
				if ((testcase + "/random") in LatencyCollector):
					LatencyCollector[testcase + "/random"] += latency
				else:
					LatencyCollector[testcase + "/random"] = latency
				if ((testcase + "/random") in WaitTimeCollector):
					WaitTimeCollector[testcase + "/random"] += waittime
				else:
					WaitTimeCollector[testcase + "/random"] = waittime
			else:
				if ((testcase + "/" + scheduler) in LatencyCollector):
					LatencyCollector[testcase + "/" + scheduler] += latency
				else:
					LatencyCollector[testcase + "/" + scheduler] = latency
				if ((testcase + "/" + scheduler) in WaitTimeCollector):
					WaitTimeCollector[testcase + "/" + scheduler] += waittime
				else:
					WaitTimeCollector[testcase + "/" + scheduler] = waittime


# calculate the mean for random scheduler
for k, v in LatencyCollector.items():
	if (k.split('/')[1] == "random"):
		LatencyCollector[k] = v / random_repeat

for k, v in WaitTimeCollector.items():
	if (k.split('/')[1] == "random"):
		WaitTimeCollector[k] = v / random_repeat

# reduce 10 versions into average for each testcase
for k, v in LatencyCollector.items():
	testcase_version = k.split('/')[0]
	scheduler = k.split('/')[1]
	testcase = testcase_version.split('_')[1]
	key = testcase + "/" + scheduler
	print(key)
	if (key in Latency):
		Latency[key] += v
	else:
	 	Latency[key] = v


for k, v in WaitTimeCollector.items():
	testcase_version = k.split('/')[0]
	scheduler = k.split('/')[1]
	testcase = testcase_version.split('_')[1]
	key = testcase + "/" + scheduler
	print(key)
	if (key in WaitTime):
		WaitTime[key] += v
	else:
	 	WaitTime[key] = v


for k, v in Latency.items():
	Latency[k] = v / version_num

for k, v in WaitTime.items():
	WaitTime[k] = v / version_num

print(len(LatencyCollector))
print(len(Latency))

datax = [0.01]
datay = []
for lamda_t in datax:
	datapoint = Latency["TX20000O10Tr5LA0.01LT" + str(lamda_t) + "LP0.75PL1.5UNIFORM/fifo"]
	datay.append(datapoint)

plt.plot(datax, datay, 'yo')
plt.show()





