echo "Starting Backend..."
cd Backend
sudo screen -dmSL backend python3 manage.py runserver
cd ..
echo "Starting Frontend..."
sudo screen -dmSL frontend npm start
echo "Done!"