list = ["Velocity", "Acceleration_X", "Acceleration_Y", "Acceleration_Z", "Voltage", "Current", "Brake_Pressure", "Temperature", "Ride_Height"]

for elem in list:
  lower = elem.lower()
  upper = elem.upper()
  print("Data " + lower + " = {\n  {\n    sizeof(" + elem + "_Calc),\n    (uint8_t *) malloc(sizeof(" + elem + "_Calc))\n  },\n  {\n    sizeof(" + elem + "_Raw),\n    (uint8_t *) malloc(sizeof(" + elem + "_Raw))\n  }\n};\ndata_map[Data_ID::"+upper+"] = " + lower + ";")
