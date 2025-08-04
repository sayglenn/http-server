# http-server

A minimal HTTP server that listens on port `4221` and returns appropriate responses based on the given request.

## Build

Simply run the build script in order to start the server.

```
./run.sh
```

## Design Considerations

1. Implemented a custom `ThreadPool` class that uses basic C++ libraries such as `std::mutex`, `std::atomic`, `std::condition_variable`. This was done for a few reasons:
   - Allow for concurrent requests to be handled at the same time without crashing using `std::thread`.
   - Reduces overhead from creating a new thread per HTTP Request and destroying it afterwards, making it more memory efficient.

## Sending Requests

- To send a basic test request

```
curl http://localhost:4221
# Returns a 200 OK response code
```

- To echo a string, send a request with path `/echo/{string}`.

```
curl http://localhost:4221/echo/hello
# Prints 'hello' to the screen
```

- Amend the `User-Agent` header and print it.

```
curl --header "User-Agent: new-agent/1.0" http://localhost:4221/user-agent
# Prints new-agent/1.0
```

- Reads files from a specified directory, assuming it exists.

```
./run.sh --directory /tmp/
# sets the working directory to /tmp/

echo -n 'You should see this!' > /tmp/example
curl http://localhost:4221/files/example
# Prints 'You should see this!' to the screen

curl -i http://localhost:4221/files/nonexistent
# Prints 'HTTP/1.1 404 Not Found'
```

Thank you for looking through this project!
