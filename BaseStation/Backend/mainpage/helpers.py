from django.http import Http404
import json, time

from .models import Data

def dataToDict(data):
    try:
        toRet = {
            "Position": data.position,
            "Velocity": data.velocity,
            "Acceleration": data.acceleration,
            "CPU_Temp": data.cpu_temp,
            "Recv_Time": data.recv_time
        }
        return toRet
    except:
        return {"Error": "dataToDict wrong type or none present"}

def postRequestToDict(requestData):
    return json.loads(requestData.body.decode('utf-8'))

def postRequestToData(requestData):
    toRet = Data()
    data = postRequestToDict(requestData)
    if ("Acceleration" in data):
        toRet.acceleration = data["Acceleration"]
    else:
        toRet.acceleration = None

    if ("Position" in data):
        toRet.position = data["Position"]
    else:
        toRet.position = None
    
    if ("Velocity" in data):
        toRet.velocity = data["Velocity"]
    else:
        toRet.velocity = None
    
    if ("CPU_Temp" in data):
        toRet.cpu_temp = data["CPU_Temp"]
    else:
        toRet.cpu_temp = data["CPU_Temp"]
    
    return toRet

def generateCompleteList():
    try:
        latestDataLists = list(Data.objects.order_by("-recv_time"))
    except Data.DoesNotExist:
        raise(Http404)

    for i in range(len(latestDataLists)):
        latestDataLists[i] = dataToDict(latestDataLists[i])
    listsToReturn = [latestDataLists[0]]
    for key in latestDataLists[0]:
        if (latestDataLists[0][key] == None):
            for dataList in latestDataLists:
                if (dataList[key] != None):
                    listsToReturn.append(dataList)
                    break
    
    toRet = {}
    for i in range(len(listsToReturn)):
        toRet[listsToReturn[i]["Recv_Time"]] = listsToReturn[i]
    return toRet
        