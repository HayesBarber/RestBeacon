#ifndef REST_BEACON_MESSAGE_H
#define REST_BEACON_MESSAGE_H

#include <Arduino.h>
#include <map>

struct Message {
    std::map<String, String> properties;

    void addProperty(const String& key, const String& value) {
        properties[key] = value;
    }

    String getProperty(const String& key) const {
        auto it = properties.find(key);
        return it != properties.end() ? it->second : "";
    }

    String toJson() const {
        String json = "{";
        bool firstProperty = true;
        for (const auto& pair : properties) {
            if (!firstProperty) {
                json += ",";
            }
            json += "\"" + pair.first + "\": \"" + pair.second + "\"";
            firstProperty = false;
        }
        json += "}";
        return json;
    }
};

#endif
