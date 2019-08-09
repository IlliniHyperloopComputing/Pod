# How to automatically start everything
* Make sure you have pip3 and nodejs installed
* run `./install_requirements.sh` to install requirements
* run `./start_basestation.sh` to start everything
* run `./kill_basestation.sh` to kill everything

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
* More stats need to be shown
* Name the state
* Stat danger calculations
* Finish command code implementation
* Testing on heavy load
* Beautify
