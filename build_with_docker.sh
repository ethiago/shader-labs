
DOCKER_IMAGE_NAME=shaderlabs-link

sudo docker build --no-cache -t ${DOCKER_IMAGE_NAME} .

CONTAINER_ID=$(sudo docker run -d ${DOCKER_IMAGE_NAME})

rm -rf ./output
sudo docker cp ${CONTAINER_ID}:"/output" ./
sudo chown -R $(whoami):$(whoami) ./output

# Remove the temporary container and image
sudo docker stop ${CONTAINER_ID} || true
sudo docker rm -f ${CONTAINER_ID} || true
sudo docker rmi -f ${DOCKER_IMAGE_NAME} || true
