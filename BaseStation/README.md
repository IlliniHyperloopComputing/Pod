# How to start the backend

In the backend folder run `python manage.py runserver`
* This requires python3, Django, and django-cors-headers to be installed
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

# Dev Page
To start the dev page perform the following steps:
1. Navigate to the Backend folder and run the following commands
    * `pip3 install Django`
    * `pip3 install django-cors-headers`
    * `python3 manage.py makemigrations`
    * `python3 manage.py migrate`
    * `python3 manage.py runserver`
2. Open a new shell and navigate to the frontend folder. Execute the following commands (Make sure you have npm and nodejs installed)
    * `npm install`
    * `npm start`
3. After a while you should see the main page of the frontend open in your browser. This is broken in its current state. Navigate to localhost:3000/dev to reach the dev page.
4. In a new tab navigate to localhost:8000/api/commands/servers to activate tcp communication with the pod.
5. You are now ready to go. Send commands by selecting one from the drop down and enter a value if needed. Data should be displaying assuming there is at least one row in the database.

# ToDo
* UDP port is not detecting if it is in use. Needs to be fixed.
    * [Issue 101 on github](https://github.com/IlliniHyperloopComputing/Pod/issues/101) has data that needs to be processed.
* Warning needs to have real data
* Ready button needs to change text for current state
* State needs to be shown more prominently
* Buttons clicks need to result in tcp/udp commands
* Battery percentage needs to be calculated
* New launcher needs to be created
* Testing on heavy load