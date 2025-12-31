# str Methods

## length

Returns the number of characters in the string.

```bishop
s.length() -> int
```

**Returns:** `int` - The string length

**Example:**
```bishop
s := "hello";
len := s.length();  // 5
```

## empty

Returns true if the string has no characters.

```bishop
s.empty() -> bool
```

**Returns:** `bool` - True if empty, false otherwise

**Example:**
```bishop
s := "";
if s.empty() {
    print("String is empty");
}
```

## contains

Checks if the string contains the given substring.

```bishop
s.contains(str substr) -> bool
```

**Parameters:**

- `substr` (`str`): The substring to search for

**Returns:** `bool` - True if found, false otherwise

**Example:**
```bishop
s := "hello world";
if s.contains("world") {
    print("Found it!");
}
```

## starts_with

Checks if the string starts with the given prefix.

```bishop
s.starts_with(str prefix) -> bool
```

**Parameters:**

- `prefix` (`str`): The prefix to check

**Returns:** `bool` - True if string starts with prefix

**Example:**
```bishop
path := "/api/users";
if path.starts_with("/api") {
    print("API route");
}
```

## ends_with

Checks if the string ends with the given suffix.

```bishop
s.ends_with(str suffix) -> bool
```

**Parameters:**

- `suffix` (`str`): The suffix to check

**Returns:** `bool` - True if string ends with suffix

**Example:**
```bishop
file := "image.png";
if file.ends_with(".png") {
    print("PNG image");
}
```

## find

Returns the index of the first occurrence of a substring, or -1 if not found.

```bishop
s.find(str substr) -> int
```

**Parameters:**

- `substr` (`str`): The substring to find

**Returns:** `int` - Index of first occurrence, or -1

**Example:**
```bishop
s := "hello world";
idx := s.find("world");  // 6
```

## substr

Extracts a portion of the string.

```bishop
s.substr(int start, int length) -> str
```

**Parameters:**

- `start` (`int`): Starting index (0-based)
- `length` (`int`): Number of characters to extract

**Returns:** `str` - The extracted substring

**Example:**
```bishop
s := "hello world";
sub := s.substr(0, 5);  // "hello"
```

## at

Returns the character at the specified index as a single-character string.

```bishop
s.at(int index) -> str
```

**Parameters:**

- `index` (`int`): The index (0-based)

**Returns:** `str` - A single-character string at that position

**Example:**
```bishop
s := "hello";
c := s.at(0);  // "h"
```

