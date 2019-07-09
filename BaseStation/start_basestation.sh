echo "Starting Backend...\n"
cd Backend
screen -dmSL backend python manage.py runserver
cd ..
echo "Starting Frontend...\n"
screen -dmSL frontend npm start
echo "Done!\n"