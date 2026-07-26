#pragma once

#include "framework/event_broker.hpp"
#include <any>
#include <cassert>
#include <typeindex>
#include <unordered_map>
#include <vector>

namespace framework {

template <typename T> void EventBroker::push_back(T message) {
  messages[std::type_index(typeid(T))].push_back(message);
}

template <typename T> std::vector<T> EventBroker::get_all() const {
  if (!messages.contains(std::type_index(typeid(T)))) {
    return {};
  }

  auto &message_vec = messages.at(std::type_index(typeid(T)));

  std::vector<T> results;
  results.reserve(message_vec.size());

  for (const auto &any_value : message_vec) {
    results.push_back(std::any_cast<T>(any_value));
  }

  return std::move(results);
}

inline void EventBroker::clear_all() {
  messages.clear();
  singleton_messages.clear();
}

template <typename T> void EventBroker::push_back_draw(T message) {
  draw_messages[std::type_index(typeid(T))].push_back(message);
}

template <typename T> std::vector<T> EventBroker::get_all_draw() const {
  if (!draw_messages.contains(std::type_index(typeid(T)))) {
    return {};
  }

  auto &message_vec = draw_messages.at(std::type_index(typeid(T)));

  std::vector<T> results;
  results.reserve(message_vec.size());

  for (const auto &any_value : message_vec) {
    results.push_back(std::any_cast<T>(any_value));
  }

  return std::move(results);
}

inline void EventBroker::clear_all_draw() {
  draw_messages.clear();
}

template <typename T> void EventBroker::set_singleton(T message) {
  singleton_messages[std::type_index(typeid(T))] = message;
}

template <typename T> std::optional<T> EventBroker::get_singleton() const {
  if (!singleton_messages.contains(std::type_index(typeid(T)))) {
    return std::nullopt;
  }

  auto any_value = singleton_messages.at(std::type_index(typeid(T)));

  return std::any_cast<T>(any_value);
}

} // namespace framework