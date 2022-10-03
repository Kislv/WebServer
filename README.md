# WebServer

**Type**
C++, thread pool. Thread pool - создается набор (пул) потоков принимающих соединения, принятое соединение обрабатывается потоком в блокирующем режиме

## Build and run webserver Docker image 
```
sudo docker rm -vf webservercontainer || true && sudo docker build -f ./docker/Dockerfile --tag webserver . && sudo  docker run -p 8080:8080 --name webservercontainer -t webserver
```

## Benchmarks

### Nginx
```
  8 threads and 100 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency     2.67ms   12.35ms 178.09ms   98.55%
    Req/Sec     8.86k     1.89k   16.13k    83.38%
  706557 requests in 10.03s, 65.05GB read
Requests/sec:  70433.43
Transfer/sec:      6.48GB
```

### Webserver
```
  8 threads and 100 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency    20.18ms  132.81ms   1.71s    97.20%
    Req/Sec     1.84k     1.27k    7.20k    64.59%
  76665 requests in 10.01s, 7.05GB read
  Socket errors: connect 0, read 0, write 0, timeout 6
Requests/sec:   7657.37
Transfer/sec:    721.26MB
```
