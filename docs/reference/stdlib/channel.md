# Channel Methods

## send

Sends a value through the channel.

```bishop
s.send(T value)
```

**Parameters:**

- `value` (`T`): The value to send

**Example:**
```bishop
ch.send(42);
```

## recv

Receives a value from the channel.

```bishop
s.recv() -> T
```

**Returns:** `T` - The received value

**Example:**
```bishop
val := ch.recv();
```

