#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>
#include <map>

struct Message {
  std::map<String, String> properties;

  void addProperty(const String &key, const String &value) {
    properties[key] = value;
  }

  String getProperty(const String &key) const {
    auto it = properties.find(key);
    return it != properties.end() ? it->second : "";
  }

  String toJson() const {
    String json = "{";
    bool firstProperty = true;
    for (const auto &pair : properties) {
      if (!firstProperty) {
        json += ",";
      }
      json += "\"" + pair.first + "\": \"" + pair.second + "\"";
      firstProperty = false;
    }
    json += "}";
    return json;
  }

  static bool fromJson(const String &jsonStr, Message &outMsg) {
    JsonDocument doc;
    DeserializationError err = deserializeJson(doc, jsonStr);
    if (err) {
      return false;
    }
    for (JsonPair kv : doc.as<JsonObject>()) {
      if (kv.value().is<JsonArray>()) {
        String combined;
        for (JsonVariant elem : kv.value().as<JsonArray>()) {
          if (!combined.isEmpty())
            combined += "|";
          combined += elem.as<String>();
        }
        outMsg.addProperty(kv.key().c_str(), combined);
      } else {
        outMsg.addProperty(kv.key().c_str(), kv.value().as<String>());
      }
    }
    return true;
  }
};
