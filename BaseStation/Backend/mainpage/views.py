from django.shortcuts import get_list_or_404
from django.http import HttpResponse, Http404, JsonResponse
from django.views.decorators.csrf import csrf_exempt

import time

from .models import Data
from .helpers import dataToDict, postRequestToData, generateCompleteList
# Create your views here.
@csrf_exempt
def index(request):
    if request.method == "GET":
        return JsonResponse(generateCompleteList())
    elif request.method == "POST":
        data = postRequestToData(request)
        data.recv_time = int(round(time.time() * 1000))
        data.save()
        return HttpResponse("Received")
    raise(Http404)