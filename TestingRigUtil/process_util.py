import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

#This tool is designed to parse the data output from propulsion disk testing
#It can take in multiple files to work on, assuming the tests are from the same distance

#all input files must have a consistent number of columns in them

input_directory = "raw_data/"
output_directory = "output_data/"

use = [29,30,31,32,33]

test_names = list()
file_names = list()
data_start = list()
data_end   = list()

#read in meta.txt file
metadata_p = pd.read_csv(input_directory+"meta.txt", 
                    header=None, delim_whitespace=True, comment='#' )
metadata = metadata_p.values
for i in range(0,len(metadata[:,0])):
    print(int(metadata[i,0]))
    if(int(metadata[i,0]) in use):
        test_names.append((metadata[i,1]))
        file_names.append(("test"+ ("%d"%metadata[i,0]) + "_raw.txt") )
        data_start.append(metadata[i,3])
        data_end.append(metadata[i,4])

print(test_names)
print(file_names)
print(data_start)
print(data_end)

turns = 6
amp_base = 877
amp_slope  = 300.0 / 0.625 / 32768.0 * 6.144 / turns
volt_slope = 0.0021394 #multiply recorded 16bit ADC values by this 

rpm_cutoff = 3400.0
force_lower_bandwidth = 0
force_upper_bandwidth = 60

spec_times = [0]*len(use)
spec_rpm   = [0]*len(use)
spec_volts = [0]*len(use)
spec_amps  = [0]*len(use)
spec_force = [0]*len(use)
spec_temp  = [0]*len(use)

# used to store same data as above, but windowed
wind_times = [list() for i in range(0, len(use))]
wind_rpm   = [list() for i in range(0, len(use))]
wind_volts = [list() for i in range(0, len(use))]
wind_amps  = [list() for i in range(0, len(use))]
wind_force = [list() for i in range(0, len(use))]
wind_temp  = [list() for i in range(0, len(use))]

#this loop will input all of the data
for i in range(0, len(use)):
    obj_in = pd.read_csv(input_directory + file_names[i], header=None, delim_whitespace=True)
    dd = obj_in.values
    start_idx = -1
    end_idx = -1
    times_array = np.array(dd[:,0], dtype="float")
    for j in range(0, len(times_array)):
        if(start_idx == -1 and times_array[j] >= data_start[i]):
            start_idx = j
        if(end_idx == -1 and times_array[j] >= data_end[i]):
            end_idx = j

    spec_times[i] = dd[start_idx:end_idx,0]
    spec_rpm[i]   = dd[start_idx:end_idx,2]
    spec_volts[i] = (dd[start_idx:end_idx,3] * volt_slope )
    spec_amps[i]  = (((dd[start_idx:end_idx,4]) - amp_base) * amp_slope)
    spec_force[i] = dd[start_idx:end_idx,5]
    spec_temp[i]  = dd[start_idx:end_idx,7]

def smlbkt(val):
    tmp = int(val)
    tmp = int(tmp / 20)
    tmp = int(tmp * 20)
    return (tmp + (20/2))

#Force Buckets for each specific data set
s_bucket = [dict() for i in range(0, len(use))]
for i in range(0, len(use)):
    for j in range(0, len(spec_times[i])):
        if(spec_rpm[i][j] < rpm_cutoff and spec_force[i][j]>force_lower_bandwidth and spec_force[i][j]<force_upper_bandwidth):
            #use the also as an opportunity to create a data set that is just windowed
            wind_times[i].append(spec_times[i][j])
            wind_rpm[i].append(spec_rpm[i][j])
            wind_force[i].append(spec_force[i][j])
            wind_volts[i].append(spec_volts[i][j])
            wind_amps[i].append(spec_amps[i][j])
            wind_temp[i].append(spec_temp[i][j])

            if(smlbkt(spec_rpm[i][j]) in s_bucket[i] ):
                (a,b,c,d) = s_bucket[i][smlbkt(spec_rpm[i][j])]

                s_bucket[i][smlbkt(spec_rpm[i][j])] = (a+1, (b+a*spec_force[i][j])/(a+1), (c+a*spec_volts[i][j])/(a+1), (d+a*spec_amps[i][j])/(a+1))
            else:
                s_bucket[i][smlbkt(spec_rpm[i][j])] = (1, spec_force[i][j], spec_volts[i][j], spec_amps[i][j])

    wind_rpm[i] = np.array(wind_rpm[i])
    wind_force[i] = np.array(wind_force[i])
    wind_volts[i] = np.array(wind_volts[i])
    wind_amps[i] = np.array(wind_amps[i])
    wind_temp[i] = np.array(wind_temp[i])

#create lists for the specific sets of data
spec_times_avg  = [list() for i in range(0, len(use))]
spec_rpm_avg    = [list() for i in range(0, len(use))]
spec_force_avg  = [list() for i in range(0, len(use))]
spec_amps_avg   = [list() for i in range(0, len(use))]
spec_volts_avg  = [list() for i in range(0, len(use))]

for i in range(0, len(use)):
    #get keys (which are rpm values)
    spec_rpm_avg[i] = np.array(list(s_bucket[i].keys()))
    #get values (which are the force,volts,amps)
    spec_bucket = np.array(list(s_bucket[i].values()))
    for j in range(0, len(spec_rpm_avg[i])):
        spec_force_avg[i].append(spec_bucket[j][1])
        spec_volts_avg[i].append(spec_bucket[j][2])
        spec_amps_avg[i].append(spec_bucket[j][3])
    #convert to numpy array
    spec_force_avg[i] = np.array(spec_force_avg[i])
    spec_volts_avg[i] = np.array(spec_volts_avg[i])
    spec_amps_avg[i]  = np.array(spec_amps_avg[i])

###
# Export Data to CSV
###

cols = ['Force(N)', 'RPM', 'Volts', 'Amps']

# specific data avgs
# and specific data windowed
for i in range(0, len(use)):
    df_data = np.vstack((spec_force_avg[i], spec_rpm_avg[i], spec_force_avg[i], spec_amps_avg[i])).T
    df_spec = pd.DataFrame(df_data, columns=cols)
    df_spec.to_csv(output_directory+test_names[i]+"_avg.csv")

    cols_wind = ['TimeStamp', 'Force(N)', 'RPM', 'Volts', 'Amps']
    df_data = np.vstack((wind_times[i], wind_force[i], wind_rpm[i], wind_volts[i], wind_amps[i])).T
    df_spec = pd.DataFrame(df_data, columns=cols_wind)
    df_spec.to_csv(output_directory+test_names[i]+"_wind.csv")

###
# Plotting 
###

clr = ['b','g','r','c','m','y','k','w',   'b','g','r','c','m','y','k','w']
#plot specific data sets vs time
for i in range(0, len(use)):
    f, (ax1, ax2, ax3, ax4) = plt.subplots(4,1, sharex=True)

    ax1.plot(wind_times[i], wind_force[i], marker='o', color=clr[i], 
                    linestyle="None", label=(test_names[i]))
    ax1.set_title(test_names[i]+" Windowed Time vs Force(N)")
    #ax1.xlabel("Time Stamp")
    #ax1.ylabel("Force(N)")
    ax1.legend(loc='best')

    ax2.plot(wind_times[i], wind_rpm[i], marker='o', color=clr[i], 
                    linestyle="None", label=(test_names[i]))
    ax2.set_title(test_names[i]+" Windowed Time vs RPM")
    #ax2.xlabel("Time Stamp")
    #ax2.ylabel("RPM")
    ax2.legend(loc='best')

    ax3.plot(wind_times[i], wind_amps[i], marker='o', color=clr[i], 
                    linestyle="None", label=(test_names[i]))
    ax3.set_title(test_names[i]+" Windowed Time vs Amps")
    #ax3.xlabel("Time Stamp")
    #ax3.ylabel("Amps")
    ax3.legend(loc='best')

    ax4.plot(wind_times[i], wind_temp[i], marker='o', color=clr[i], 
                    linestyle="None", label=(test_names[i]))
    ax4.set_title(test_names[i]+" Windowed Time vs Temp of Tube")
    ax4.legend(loc='best')

    plt.show()
    plt.figure()


#plot general rpm vs force straight just windowed
for i in range(0, len(use)):
    plt.plot(wind_rpm[i], wind_force[i], marker='o', color=clr[i], 
                    linestyle="None", label=(test_names[i]))
    plt.title(test_names[i]+" Windowed RPM vs Force (N)")
    plt.xlabel("RPM")
    plt.ylabel("Force (N)")
    plt.legend(loc='best')
    plt.show()
    plt.figure()

#plot rpm vs force
for i in range(0, len(use)):
    plt.plot(spec_rpm_avg[i], spec_force_avg[i], marker='o', color=clr[i], 
                    linestyle="None", label=(test_names[i]))
    plt.title(test_names[i]+" Avg RPM vs Force (N)")
    plt.xlabel("RPM")
    plt.ylabel("Force")
    plt.legend(loc='best')
    plt.show()
    plt.figure()

#plot rpm vs force specific and all on the same graph
for i in range(0, len(use)):
    plt.plot(spec_rpm_avg[i], spec_force_avg[i], marker='o', color=clr[i], 
                    linestyle="None", label=(test_names[i]))
plt.title("Avg RPM vs Force (N)")
plt.xlabel("RPM")
plt.ylabel("Force")
plt.legend(loc='best')
plt.show()
plt.figure()

##plot rpm vs amps
#plt.figure()
#for i in range(0, len(use)):
#    plt.plot(spec_rpm_avg[i], spec_amps_avg[i], marker='o', color=clr[i], 
#                    linestyle="None", label=(test_names[i]))
#plt.title("Avg RPM vs Amps (A)")
#plt.xlabel("RPM")
#plt.ylabel("Amps (A)")
#plt.legend(loc='best')
#
##plot rpm vs power
#plt.figure()
#for i in range(0, len(use)):
#    plt.plot(spec_rpm_avg[i], spec_amps_avg[i] * spec_volts_avg[i], marker='o', color=clr[i], 
#                    linestyle="None", label=(test_names[i]))
#plt.title("Avg RPM vs Power (W)")
#plt.xlabel("RPM")
#plt.ylabel("Power (W)")
#plt.legend()
#plt.show()


