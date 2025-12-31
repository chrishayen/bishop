# http Module

```bishop
import http;
```

## Structs

### Request

Represents an incoming HTTP request.

**Fields:**

| Field | Type | Description |
|-------|------|-------------|
| `method` | `str` | HTTP method (GET, POST, PUT, DELETE, etc.) |
| `path` | `str` | Request path (e.g., "/users/123") |
| `body` | `str` | Request body content |

**Example:**
```bishop
fn handle(http.Request req) -> http.Response {
    if req.method == "POST" {
        return http.text("Received: " + req.body);
    }
    return http.not_found();
}
```

### Response

Represents an HTTP response to send back to the client.

**Fields:**

| Field | Type | Description |
|-------|------|-------------|
| `status` | `int` | HTTP status code (200, 404, 500, etc.) |
| `content_type` | `str` | Content-Type header value |
| `body` | `str` | Response body content |

**Example:**
```bishop
resp := http.Response { status: 200, content_type: "text/html", body: "<h1>Hello</h1>" };
```

### App

HTTP application with routing support.

**Example:**
```bishop
app := http.App {};
app.get("/", home_handler);
app.post("/submit", submit_handler);
app.listen(8080);
```

## Functions

### text

Creates a 200 OK response with text/plain content type.

```bishop
fn text(str content) -> http.Response
```

**Parameters:**

- `content` (`str`): Response body text

**Returns:** `http.Response` - A text response

**Example:**
```bishop
return http.text("Hello, World!");
```

### json

Creates a 200 OK response with application/json content type.

```bishop
fn json(str content) -> http.Response
```

**Parameters:**

- `content` (`str`): JSON string to send

**Returns:** `http.Response` - A JSON response

**Example:**
```bishop
return http.json("{\"status\": \"ok\"}");
```

### not_found

Creates a 404 Not Found response.

```bishop
fn not_found() -> http.Response
```

**Returns:** `http.Response` - A 404 response

**Example:**
```bishop
if path == "/unknown" {
    return http.not_found();
}
```

### serve

Starts an HTTP server on the specified port with a single handler function.

```bishop
fn serve(int port, fn(http.Request) handler)
```

**Parameters:**

- `port` (`int`): Port number to listen on
- `handler` (`fn(http.Request)`): Handler function for all requests

**Example:**
```bishop
fn handle(http.Request req) -> http.Response {
    return http.text("Hello!");
}
http.serve(8080, handle);
```

