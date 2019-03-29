# How to start the backend
In the backend folder run `python manage.py runserver`
* This requires python3, and Django to be installed
* Follow this tutorial https://realpython.com/python-virtual-environments-a-primer/ to setup a python3 virtualenv
* Once inside the virtualenv, run `pip install Django`
* then run `python manage.py runserver`

This will spawn both the TCP server and the HTTP server.
By default the HTTP server will run on port 8000, the TCP server will print its port for now since on error the address may be in use.


# How to send data
Right now you have to send packets in the form int1,int2,int3 (this is just for test purposes and will be changed later).

# How to recieve the data
Navigate to localhost:8000/podconnect/data to see all data in xml format (open the inspector in your web browser to see the xml field names).
Navigate to localhost:8000/podconnect/data/latest too see the latest data in xml format.
* To delete all data simply `rm db.sqlite3`, then run `python manage.py migrate`. Then you can start the server again with no issues

# ToDo
UDP port is not detecting if it is in use. Needs to be fixed.
