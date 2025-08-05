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

2. Supports persistent connections which contain the `Connection: keep-alive` header to allow clients to reuse TCP connections.

3. RESTful `/file` endpoint to ensure both `GET` and `POST` methods are supported, using `std::fstream` and restricting to a base directory.

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

- Reads files from a specified directory, assuming the directory exists. This is done by sending a `GET` request to the `/files` endpoint.

```
./run.sh --directory /tmp/
# Sets the working directory to /tmp/

echo -n 'You should see this!' > /tmp/example
curl http://localhost:4221/files/example
# Prints 'You should see this!' to the screen

curl -i http://localhost:4221/files/nonexistent
# Prints 'HTTP/1.1 404 Not Found'
```

- Write to files to a specified directory, assuming the directory exists. Files that do not exist will be automatically created. This is done by sending a `POST` request to the `/files` endpoint, with the --data flag to input information into the request body.

```
./run.sh --directory /tmp/
# Sets the working directory to /tmp/

curl --data "Wrote some data" -H "Content-Type: application/octet-stream" http://localhost:4221/files/test_file
# Should return 201 Created

curl http://localhost:4221/files/test_file
# Prints 'Wrote some data'
```

Thank you for looking through this project!
