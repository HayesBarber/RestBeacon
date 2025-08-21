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

  static Message fromJson(const String &jsonStr) {
    Message msg;
    JsonDocument doc;
    DeserializationError err = deserializeJson(doc, jsonStr);
    if (err) {
      return msg;
    }
    for (JsonPair kv : doc.as<JsonObject>()) {
      String key = kv.key().c_str();
      msg.addProperty(key, kv.value().as<String>());
    }
    return msg;
  }
};
