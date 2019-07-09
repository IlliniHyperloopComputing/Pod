# How to start the backend manually
* First navigate to the Backend folder
* Make sure all requirements are installed by running `pip install -r requirements.txt`
* Then run `python manage.py runserver`

This will spawn the HTTP server.
By default the HTTP server will run on port 8000.

# How to start the frontend manually3
* Navigate to the frontend folder
* Make sure you have npm installed
* Run `npm install` (you might need to run `npm install -g @angular/cli` as well)
* Run `npm start`

# Information about data storage
* Data is stored in a sqlite3 database named db.sqlite3. This database will store every tcp command received and will 
* To delete all data simply `rm db.sqlite3`, then run `python manage.py migrate`. Then you can start the server again with no issues

# ToDo
* UDP port is not detecting if it is in use. Needs to be fixed.
    * [Issue 101 on github](https://github.com/IlliniHyperloopComputing/Pod/issues/101) has data that needs to be processed.
* Warning needs to have real data or be deleted
* Frontend does not detect if connection is lost
* State needs to be shown more prominently
* More stats need to be shown
* Stat danger calculations
* This file needs to be updated
* Battery percentage needs to be calculated/ obtained from the BMS
* Testing on heavy load
* Create Launcher
* Beautify