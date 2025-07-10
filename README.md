# RestBeacon

**RestBeacon** is a lightweight C++ library for ESP32 devices that enables two key features:

- **RESTful messaging** via HTTP POST to a `/message` endpoint.
- **UDP-based discovery**, allowing devices to respond to broadcast queries.

## Features

- Easily handle JSON messages sent via HTTP POST
- Respond to discovery broadcasts over UDP
- Custom callbacks for both message handling and discovery
- Customizable passphrase for UDP discovery

## Discovery Passphrase

By default, the library listens for the UDP message `"WHO_IS_THERE"`. You can override this by passing a custom passphrase to the `RestBeacon` constructor:

```cpp
RestBeacon beacon(80, 4210, "MY_DISCOVERY_KEY");
```

## Example

```cpp
#include <RestBeacon.h>

RestBeacon beacon;

void setup() {
  WiFi.begin("your-ssid", "your-pass");
  while (WiFi.status() != WL_CONNECTED) delay(100);

  beacon.onMessage([](const Message& msg) {
    Serial.println("Got a message!");
    Serial.println(msg.getProperty("hello"));
    return "OK";
  });

  beacon.onDiscovery([](IPAddress sender, uint16_t port) {
    Serial.print("Discovery from: ");
    Serial.println(sender);
  });

  beacon.begin();
}

void loop() {
  beacon.loop();
}
```

## Message Format

Send a JSON object as the body of an HTTP POST request to the `/message` endpoint.

The structure of the JSON is entirely up to you. The library will parse it into key-value string pairs and pass them to your callback via the `Message` object.

Example:

```json
{
  "foo": "bar",
  "device": "sensor1"
}
```
