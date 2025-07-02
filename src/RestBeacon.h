#ifndef REST_BEACON_H
#define REST_BEACON_H

#include <WebServer.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <functional>

class RestBeacon {
public:
    using MessageCallback = std::function<void(const String&)>;
    using DiscoveryCallback = std::function<String()>;

    RestBeacon(uint16_t httpPort = 80, uint16_t udpPort = 4210);

    void begin();
    void loop();

    void onMessage(MessageCallback cb);
    void onDiscovery(DiscoveryCallback cb);
    void setDeviceName(const String& name);

private:
    WebServer _server;
    WiFiUDP _udp;
    MessageCallback _messageCallback;
    DiscoveryCallback _discoveryCallback;

    String _deviceName;
    uint16_t _udpPort;

    void handleHttpMessage();
    void listenForBroadcast();
};

#endif
