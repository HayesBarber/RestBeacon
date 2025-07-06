#ifndef REST_BEACON_H
#define REST_BEACON_H

#include "Message.h"
#include <WebServer.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <functional>

/**
 * @class RestBeacon
 * @brief Provides a RESTful interface and UDP-based discovery mechanism for ESP32 devices.
 *
 * This class allows an ESP32 to listen for HTTP POST messages at a specific "/message" endpoint
 * and also notifies about UDP discovery broadcasts. It enables both direct messaging and
 * device discovery on a local network.
 */
class RestBeacon {
public:
    using MessageCallback = std::function<String(const Message&)>;
    using DiscoveryCallback = std::function<void(IPAddress, uint16_t)>;

    /**
     * @brief Constructs a RestBeacon server.
     * @param httpPort The HTTP port to listen on for POST messages.
     * @param udpPort The UDP port to listen on for discovery broadcasts.
     */
    RestBeacon(uint16_t httpPort = 80, uint16_t udpPort = 4210);

    /**
     * @brief Starts the HTTP server and UDP listener.
     *
     * Call this method in setup() to initialize the RestBeacon functionality.
     */
    void begin();


    /**
     * @brief Handles HTTP requests and checks for incoming UDP broadcasts.
     *
     * Call this method in loop() to continuously process HTTP and UDP communication.
     */
    void loop();

    /**
     * @brief Registers a callback to handle incoming HTTP POST requests on the /message endpoint.
     * @param cb The callback to invoke when a valid message is received, returning a string response to the request.
     */
    void onMessage(MessageCallback cb);


    /**
     * @brief Registers a callback to handle UDP discovery messages.
     * @param cb The callback to invoke when a WHO_IS_THERE message is received. Receives the sender's IP address and port as parameters.
     */
    void onDiscovery(DiscoveryCallback cb);

private:
    WebServer _server;
    WiFiUDP _udp;
    MessageCallback _messageCallback;
    DiscoveryCallback _discoveryCallback;

    uint16_t _udpPort;
    uint16_t _httpPort;

    void handleHttpMessage();
    void listenForBroadcast();
    void listenForMessage();
};

#endif
