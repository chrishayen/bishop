# http.App Methods

## get

Registers a handler for GET requests at the specified path.

```bishop
s.get(str path, fn(http.Request) handler)
```

**Parameters:**

- `path` (`str`): URL path to match
- `handler` (`fn(http.Request)`): Handler function

**Example:**
```bishop
app.get("/", home);
app.get("/about", about);
```

## post

Registers a handler for POST requests at the specified path.

```bishop
s.post(str path, fn(http.Request) handler)
```

**Parameters:**

- `path` (`str`): URL path to match
- `handler` (`fn(http.Request)`): Handler function

**Example:**
```bishop
app.post("/submit", handle_submit);
```

## listen

Starts the HTTP server and begins listening for requests.

```bishop
s.listen(int port)
```

**Parameters:**

- `port` (`int`): Port number to listen on

**Example:**
```bishop
app.listen(8080);
```

