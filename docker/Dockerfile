FROM ubuntu:latest as executor

USER root

# TODO delete updating, add static release
# RUN 
RUN apt-get update && \
    apt-get -y install --no-install-recommends \
    make\
    g++\
    cmake
    
COPY . /app
COPY . /var/www

WORKDIR /app/build

RUN cmake .. && cmake --build .

EXPOSE 8080

CMD ./WebServer
