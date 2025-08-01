#include "RestBeacon.h"
#include <ArduinoJson.h>

RestBeacon::RestBeacon(uint16_t httpPort, uint16_t udpPort)
    : _server(httpPort), _udpPort(udpPort), _httpPort(httpPort) {}

void RestBeacon::onMessage(MessageCallback cb) {
    _messageCallback = cb;
}

void RestBeacon::onDiscovery(DiscoveryCallback cb) {
    _discoveryCallback = cb;
}

void RestBeacon::begin() {
    if (_messageCallback) {
        Serial.printf("[RestBeacon] Starting HTTP server on port %u...\n", _httpPort);
        _server.on("/message", HTTP_POST, [this]() { handleHttpMessage(); });
        _server.begin();
    } else {
        Serial.println("[RestBeacon] HTTP server not started (no message callback set).");
    }

    if (_discoveryCallback) {
        Serial.printf("[RestBeacon] Starting UDP listener on port %u...\n", _udpPort);
        _udp.begin(_udpPort);
    } else {
        Serial.println("[RestBeacon] UDP listener not started (no discovery callback set).");
    }
}

void RestBeacon::loop() {
    listenForMessage();
    listenForBroadcast();
}

void RestBeacon::loopHttp() {
    listenForMessage();
}

void RestBeacon::loopUdp() {
    listenForBroadcast();
}

void RestBeacon::handleHttpMessage() {
    if (!_messageCallback) {
        _server.send(404, "text/plain", "No method");
        return;
    }

    if (_server.method() != HTTP_POST) {
        _server.send(405, "text/plain", "Method Not Allowed");
        return;
    }

    String body = _server.arg("plain");

    if (body.isEmpty()) {
        _server.send(400, "text/plain", "Empty body");
        return;
    }

    JsonDocument doc;
    DeserializationError err = deserializeJson(doc, body);
    if (err) {
        _server.send(400, "text/plain", "Invalid JSON");
        return;
    }

    Message msg;

    for (JsonPair kv : doc.as<JsonObject>()) {
        String key = kv.key().c_str();
        msg.addProperty(key, kv.value().as<String>());
    }

    String reply = _messageCallback(msg);

    _server.send(200, "text/plain", reply);
}

void RestBeacon::listenForMessage() {
    if (!_messageCallback) return;
    _server.handleClient();
}

void RestBeacon::listenForBroadcast() {
    if (!_discoveryCallback) return;

    int packetSize = _udp.parsePacket();
    if (packetSize <= 0) return;

    char incoming[256];
    int len = _udp.read(incoming, sizeof(incoming) - 1);
    if (len <= 0) return;

    incoming[len] = '\0';

    String message = String(incoming);

    _discoveryCallback(_udp.remoteIP(), _udp.remotePort(), message);
}
