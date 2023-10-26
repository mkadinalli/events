echo "updating apt..."
echo
sudo apt-get update -y
echo
echo "installing dependencies..."

sudo apt-get install -y libjson-c-dev # json-lib

sudo apt install libmysqlclient-dev # mysql-api

sudo apt install make


DIRECTORY="./config"
FILES_DIR="./files"
LOG_DIR="./logs"

if [ ! -d ${DIRECTORY} ]; then
  mkdir ${DIRECTORY}
fi

if [ ! -d ${FILES_DIR} ]; then
  mkdir ${FILES_DIR}
fi

if [ ! -d ${LOG_DIR} ]; then
  mkdir ${LOG_DIR}
fi

rm -r ${DIRECTORY}/*

FILE="${DIRECTORY}/config.json"

#if [ -f $FILE ]; then
#    echo "deleting old config..."
#    rm $FILE
#fi

touch ${FILE}

read -p "Enter MySQL username:  " MySQL_U_NAME
read -s -p "Enter MySQL password:  " MySQL_PASS
echo
read -p "Enter MySQL host:  " MySQL_HOST

configuration="{\
    \"username\": \"${MySQL_U_NAME}\",\
    \"password\": \"${MySQL_PASS}\",\
    \"host\": \"${MySQL_HOST}\",\
    \"database\": \"events\",\
    \"port\": 2000\
}"

echo $configuration > $FILE

echo "Writing schema..."
echo

# mysql -h ${MySQL_HOST} -u ${MySQL_U_NAME} -p"${MySQL_PASS}" < ./schema/scema.sql

echo "Schema written..."
echo

echo "generating crt..."

echo

openssl genrsa -aes128 -out ./config/fd.key 2048

openssl req -new -key ./config/fd.key -out ./config/fd.csr

openssl x509 -req -days 365 -in ./config/fd.csr -signkey ./config/fd.key -out ./config/fd.crt

echo "crt generated."
echo

echo "compiling project..."
make clean
make
echo 
echo "project compilation complete."
echo
