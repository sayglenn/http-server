# http-server

A minimal HTTP server that listens on port `4221` and returns `200 OK` or `404 Not Found`.

## ⚙️ Build

```
make
```

## Sending requests

```
curl -v http://localhost:4221
```

## Features

- To echo a string, send a request with path `/echo/{string}`.

```
curl -v http://localhost:4221/echo/hello
# prints hello
```
