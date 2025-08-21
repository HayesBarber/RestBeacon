#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>
#include <map>
#include <vector>

struct Message {
  std::map<String, String> properties;

  void addProperty(const String &key, const String &value) {
    properties[key] = value;
  }

  String getProperty(const String &key) const {
    auto it = properties.find(key);
    return it != properties.end() ? it->second : "";
  }

  std::vector<String> getArrayProperty(const String &key) const {
    std::vector<String> result;
    String raw = getProperty(key);
    int start = 0;
    int idx;
    while ((idx = raw.indexOf('|', start)) != -1) {
      result.push_back(raw.substring(start, idx));
      start = idx + 1;
    }
    if (start < raw.length()) {
      result.push_back(raw.substring(start));
    }
    return result;
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
