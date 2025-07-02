#ifndef REST_BEACON_H
#define REST_BEACON_H

#include <WiFi.h>
#include <WiFiUdp.h>
#include <WebServer.h>
#include <functional>

class RestBeacon {
public:
    using MessageCallback = std::function<void(const String&)>;

    RestBeacon(uint16_t httpPort = 80, uint16_t udpPort = 4210);

    void begin();
    void loop();

    void onMessage(MessageCallback cb);
    void setDeviceName(const String& name);

private:
    WebServer _server;
    WiFiUDP _udp;
    MessageCallback _messageCallback;

    String _deviceName;
    uint16_t _udpPort;

    void handleHttpMessage();
    void listenForBroadcast();
    void sendDiscoveryResponse(IPAddress remoteIP, uint16_t remotePort);
};

#endif
