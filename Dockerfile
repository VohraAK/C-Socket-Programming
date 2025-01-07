# Use the official Ubuntu base image
FROM ubuntu:latest

# Set the working directory inside the container
WORKDIR /home/socket-env

# Install necessary packages
RUN apt update && \
    apt install -y build-essential iputils-ping

# Keep the container running by default
CMD ["bash"]

