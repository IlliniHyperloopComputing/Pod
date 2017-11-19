import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

#This tool is designed to parse the data output from propulsion disk testing
#It can take in multiple files to work on, assuming the tests are from the same distance

#all input files must have a consistent number of columns in them

distance = "0.5_inch"
test_names = np.array(["test2", "test3", "test4"])
file_names = np.array(["test2_raw.txt", "test3_raw.txt", "test4_raw.txt"])
data_start = np.array([51.550 , 41.3751, 105.9])
data_end   = np.array([103.0, 105.0, 195.0 ])
volt_real  = np.array([50.1, 49.9, 49.7   ])
volt_base  = np.array([900, 897, 895    ])
amp_base = 30
turns = 6
rpm_cutoff = 1450.0
force_lower_bandwidth = 5
force_upper_bandwidth = 20

all_times = []
all_rpm   = []
all_volts = []
all_amps  = []
all_force = []

spec_times = [0]*len(file_names)
spec_rpm   = [0]*len(file_names)
spec_volts = [0]*len(file_names)
spec_amps  = [0]*len(file_names)
spec_force = [0]*len(file_names)

# used to store same data as above, but windowed
wind_times = [list() for i in range(0, len(file_names))]
wind_rpm   = [list() for i in range(0, len(file_names))]
wind_volts = [list() for i in range(0, len(file_names))]
wind_amps  = [list() for i in range(0, len(file_names))]
wind_force = [list() for i in range(0, len(file_names))]

#this loop will input all of the data
for i in range(0, len(file_names)):
    obj_in = pd.read_csv(file_names[i], header=None, delim_whitespace=True)
    dd = obj_in.values
    start_idx = -1
    end_idx = -1
    times_array = np.array(dd[:,0], dtype="float")
    for j in range(0, len(times_array)):
        if(start_idx == -1 and times_array[j] >= data_start[i]):
            start_idx = j
        if(end_idx == -1 and times_array[j] >= data_end[i]):
            end_idx = j

    volt_slope = float(volt_real[i])/volt_base[i]
    amp_slope  = 300.0 / 0.625 / 1024.0 * 5.0 / turns

    spec_times[i] = dd[start_idx:end_idx,0]
    spec_rpm[i]   = dd[start_idx:end_idx,2]
    spec_volts[i] = (dd[start_idx:end_idx,3] * volt_slope )
    spec_amps[i]  = (((dd[start_idx:end_idx,4]) - amp_base) * amp_slope)
    spec_force[i] = dd[start_idx:end_idx,5]

    all_times = np.concatenate((all_times, dd[start_idx:end_idx,0]))
    all_rpm   = np.concatenate((all_rpm,   dd[start_idx:end_idx,2]))
    all_volts = np.concatenate((all_volts, 
                dd[start_idx:end_idx,3] * volt_slope ))
    all_amps  = np.concatenate((all_amps,  
                ((dd[start_idx:end_idx,4]) - amp_base) * amp_slope))
    all_force = np.concatenate((all_force, dd[start_idx:end_idx,5]))

#print(all_times)
#print(all_rpm)
#print(all_volts)
#print(all_amps)
#print(all_force)

# Now that we have all of the data, we can do something with it
# I'm thinking that a good strategy would be to create buckets for
# the force, with corresponding RPM and power values. 
# this way, for a given force, we know (within reason) what rpm and what power value 
# are necessary to get this force  
# I think this will be a good way of filterning down the values that matter, and from 
# that we can create some trend lines

#Force Buckets for "all" data
bucket = {}
#count, sum_rpm, sum_volts, sum_amps
bucket[0.0] = (0, 0.0, 0.0, 0.0)
for i in range(0, len(all_times)):
    if(all_rpm[i] < rpm_cutoff and all_force[i]>force_lower_bandwidth and all_force[i]<force_upper_bandwidth):
        if(all_rpm[i] in bucket and all_rpm[i] != 0.0):
            (a,b,c,d) = bucket[all_rpm[i]]

            #bullshit to average things on the go
            bucket[all_rpm[i]] = (a+1, (b+a*all_force[i])/(a+1), (c+a*all_volts[i])/(a+1), (d+a*all_amps[i])/(a+1))
        else:
            bucket[all_rpm[i]] = (1, all_force[i], all_volts[i], all_amps[i])

#Force Buckets for each specific data set
s_bucket = [dict() for i in range(0, len(file_names))]
for i in range(0, len(file_names)):
    for j in range(0, len(spec_times[i])):
        if(spec_rpm[i][j] < rpm_cutoff and spec_force[i][j]>force_lower_bandwidth and spec_force[i][j]<force_upper_bandwidth):
            #use the also as an opportunity to create a data set that is just windowed
            wind_rpm[i].append(spec_rpm[i][j])
            wind_force[i].append(spec_force[i][j])
            wind_volts[i].append(spec_volts[i][j])
            wind_amps[i].append(spec_volts[i][j])

            if(spec_rpm[i][j] in s_bucket[i] ):
                (a,b,c,d) = s_bucket[i][spec_rpm[i][j]]

                s_bucket[i][spec_rpm[i][j]] = (a+1, (b+a*spec_force[i][j])/(a+1), (c+a*spec_volts[i][j])/(a+1), (d+a*spec_amps[i][j])/(a+1))
            else:
                s_bucket[i][spec_rpm[i][j]] = (1, spec_force[i][j], spec_volts[i][j], spec_amps[i][j])

    wind_rpm[i] = np.array(wind_rpm[i])
    wind_force[i] = np.array(wind_force[i])
    wind_volts[i] = np.array(wind_volts[i])
    wind_amps[i] = np.array(wind_volts[i])
    

#create lists of "all" data after it has been bucketed
#im sure there is a "python" way of doing this, but whatever
all_rpm_o = np.array(list(bucket.keys()))
all_buckets = np.array(list(bucket.values()))
all_force_o   = []
all_volts_o = []
all_amps_o  = []
for i in range(0, len(all_rpm_o)):
    all_force_o.append(all_buckets[i][1])
    all_volts_o.append(all_buckets[i][2])
    all_amps_o.append(all_buckets[i][3])
all_force_o = np.array(all_force_o)
all_volts_o = np.array(all_volts_o)
all_amps_o = np.array(all_amps_o)

#create lists for the specific sets of data
spec_times_o = [list() for i in range(0, len(file_names))]
spec_rpm_o   = [list() for i in range(0, len(file_names))]
spec_volts_o = [list() for i in range(0, len(file_names))]
spec_amps_o  = [list() for i in range(0, len(file_names))]
spec_force_o = [list() for i in range(0, len(file_names))]
for i in range(0, len(file_names)):
    spec_rpm_o[i] = np.array(list(s_bucket[i].keys()))
    spec_bucket = np.array(list(s_bucket[i].values()))
    for j in range(0, len(spec_rpm_o[i])):
        spec_force_o[i].append(spec_bucket[j][1])
        spec_volts_o[i].append(spec_bucket[j][2])
        spec_amps_o[i].append(spec_bucket[j][3])
    spec_force_o[i] = np.array(spec_force_o[i])
    spec_volts_o[i] = np.array(spec_volts_o[i])
    spec_amps_o[i]  = np.array(spec_amps_o[i])

###
# Export Data to CSV
###

# All avg data
cols = ['Force (N)', 'RPM', 'Volts', 'Amps']
combined_data = np.vstack((all_force_o, all_rpm_o,all_volts_o, all_amps_o)).T
df_all = pd.DataFrame(combined_data, columns=cols) 
df_all.to_csv(distance+"_all_avg.csv")

# specific data avgs
# and specific data windowed
for i in range(0, len(file_names)):
    df_data = np.vstack((spec_force_o[i], spec_rpm_o[i], spec_volts_o[i], spec_amps_o[i])).T
    df_spec = pd.DataFrame(df_data, columns=cols)
    df_spec.to_csv(distance+"_"+test_names[i]+"_avg.csv")

    df_data = np.vstack((wind_force[i], wind_rpm[i], wind_volts[i], wind_amps[i])).T
    df_spec = pd.DataFrame(df_data, columns=cols)
    df_spec.to_csv(distance+"_"+test_names[i]+"_wind.csv")

###
# Plotting 
###
#plot general rpm vs force straight just windowed
clr = ['b','g','r','c','m','y','k','w']
for i in range(0, len(file_names)):
    plt.plot(wind_rpm[i], wind_force[i], marker='o', color=clr[i], 
                    linestyle="None", label=(test_names[i]))
    plt.title(distance+" "+test_names[i]+" Windowed RPM vs Force (N)")
    plt.xlabel("RPM")
    plt.ylabel("Force (N)")
    plt.legend()
    plt.figure()

#plot rpm vs force
for i in range(0, len(file_names)):
    plt.plot(spec_rpm_o[i], spec_force_o[i], marker='o', color=clr[i], 
                    linestyle="None", label=(test_names[i]))
    plt.title(distance+" "+test_names[i]+" Avg RPM vs Force (N)")
    plt.xlabel("RPM")
    plt.ylabel("Force")
    plt.legend()
    plt.figure()

#plot rpm vs force specific and all on the same graph
for i in range(0, len(file_names)):
    plt.plot(spec_rpm_o[i], spec_force_o[i], marker='o', color=clr[i], 
                    linestyle="None", label=(test_names[i]))
plt.plot(all_rpm_o, all_force_o, marker='o', color="black", linestyle="None", label=("Avg Data"))
plt.title(distance+" Avg RPM vs Force (N)")
plt.xlabel("RPM")
plt.ylabel("Force")
plt.legend()

#plot rpm vs amps
plt.figure()
for i in range(0, len(file_names)):
    plt.plot(spec_rpm_o[i], spec_amps_o[i], marker='o', color=clr[i], 
                    linestyle="None", label=(test_names[i]))
plt.plot(all_rpm_o, all_amps_o, marker='o', color="black", linestyle="None", label="Avg Data" )
plt.title(distance+" Avg RPM vs Amps (A)")
plt.xlabel("RPM")
plt.ylabel("Amps (A)")
plt.legend()

#plot rpm vs power
plt.figure()
for i in range(0, len(file_names)):
    plt.plot(spec_rpm_o[i], spec_amps_o[i] * spec_volts_o[i], marker='o', color=clr[i], 
                    linestyle="None", label=(test_names[i]))
plt.plot(all_rpm_o, all_amps_o * all_volts_o, marker='o', color="black", linestyle="None", label="Avg Data" )
plt.title(distance+" Avg RPM vs Power (W)")
plt.xlabel("RPM")
plt.ylabel("Power (W)")
plt.legend()
plt.show()


