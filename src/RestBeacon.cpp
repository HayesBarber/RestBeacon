#include "RestBeacon.h"

RestBeacon::RestBeacon(uint16_t httpPort, uint16_t udpPort)
    : _server(httpPort), _udpPort(udpPort), _deviceName("RestBeacon") {}

void RestBeacon::setDeviceName(const String& name) {
    _deviceName = name;
}

void RestBeacon::onMessage(MessageCallback cb) {
    _messageCallback = cb;
}

void RestBeacon::onDiscovery(DiscoveryCallback cb) {
    _discoveryCallback = cb;
}

void RestBeacon::begin() {
    _server.on("/message", HTTP_POST, [this]() { handleHttpMessage(); });
    _server.begin();

    _udp.begin(_udpPort);
}

void RestBeacon::loop() {
    _server.handleClient();
    listenForBroadcast();
}

void RestBeacon::handleHttpMessage() {
    if (_server.method() != HTTP_POST) {
        _server.send(405, "text/plain", "Method Not Allowed");
        return;
    }

    String body = _server.arg("plain");

    if (body.isEmpty()) {
        _server.send(400, "text/plain", "Empty body");
        return;
    }

    if (_messageCallback) {
        _messageCallback(body);
    }

    _server.send(200, "text/plain", "OK");
}

void RestBeacon::listenForBroadcast() {
    int packetSize = _udp.parsePacket();
    if (packetSize <= 0) return;

    char incoming[256];
    int len = _udp.read(incoming, sizeof(incoming) - 1);
    if (len <= 0) return;

    incoming[len] = '\0';

    if (String(incoming) == "WHO_IS_THERE") {
        if (_discoveryCallback) {
            _discoveryCallback();
        }
    }
}
