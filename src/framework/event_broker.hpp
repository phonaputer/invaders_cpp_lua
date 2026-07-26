#pragma once

#include <any>
#include <optional>
#include <typeindex>
#include <unordered_map>
#include <vector>

namespace framework {

class EventBroker {
  private:
    std::unordered_map<std::type_index, std::vector<std::any>> draw_messages;
    std::unordered_map<std::type_index, std::vector<std::any>> messages;
    std::unordered_map<std::type_index, std::any> singleton_messages;

  public:
    template <typename T> void push_back(T message);
    template <typename T> std::vector<T> get_all() const;
    void clear_all();

    template <typename T> void push_back_draw(T message);
    template <typename T> std::vector<T> get_all_draw() const;
    void clear_all_draw();

    template <typename T> void set_singleton(T message);
    template <typename T> std::optional<T> get_singleton() const;
};

} // namespace framework

#include "framework/event_broker.tpp"