FROM ubuntu:latest as executor

USER root

# TODO delete updating, add static release
RUN 
# RUN apt-get update && \
    apt-get -y install --no-install-recommends \
    make 4.3\
    g++ 12.1.1\
    cmake
    
COPY . /app
COPY . /var/www

WORKDIR /app/build

RUN cmake .. && cmake --build .

EXPOSE 8080

CMD ./WebServer
