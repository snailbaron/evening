#pragma once

#include <vector>
#include <map>
#include <any>
#include <functional>
#include <typeindex>
#include <memory>
#include <numeric>

namespace evening {

class Channel final {
public:
    Channel() {}
    Channel(const Channel&) = delete;
    Channel(Channel&&) = delete;
    Channel& operator=(const Channel&) = delete;
    Channel& operator=(Channel&&) = delete;

    /**
     * Subscribe to channel, providing a tracker.
     */
    template <class Event, class Tracker>
    void subscribe(
        std::weak_ptr<Tracker> tracker,
        std::function<void(const Event&)> handler)
    {
        _handlers[std::type_index(typeid(Event))].push_back(
            [tracker, handler] (const std::any& event) -> bool {
                if (tracker.expired()) {
                    return false;
                }

                handler(std::any_cast<Event>(event));
                return true;
            });
    }

    template <class Event, class Tracker>
    void subscribe(
        const std::shared_ptr<Tracker>& tracker,
        std::function<void(const Event&)> handler)
    {
        subscribe(std::weak_ptr<Tracker>(tracker), handler);
    }


    /**
     * Subscribe to channel, returning subscription object.
     */
    template <class Event>
    std::shared_ptr<char> subscribe(std::function<void(const Event&)> handler)
    {
        auto life = std::make_shared<char>();
        std::weak_ptr<char> tracker = life;
        _handlers[std::type_index(typeid(Event))].push_back(
            [tracker, handler] (const std::any& event) -> bool {
                if (tracker.expired()) {
                    return false;
                }

                handler(std::any_cast<Event>(event));
                return true;
            });

        return life;
    }

    template <class Event>
    void push(const Event& event)
    {
        _events[std::type_index(typeid(Event))].push_back(event);
    }

    template <class Event, class... Args>
    void makePush(Args&&... args)
    {
        _events[std::type_index(typeid(Event))].emplace_back(Event(std::forward<Args>(args)...));
    }

    template <class Event>
    void send(const Event& event)
    {
        auto handlersPairIt = _handlers.find(std::type_index(typeid(Event)));
        if (handlersPairIt == _handlers.end()) {
            return;
        }

        sendEvent(event, handlersPairIt);
    }

    template <class Event, class... Args>
    void makeSend(Args&&... args)
    {
        send(Event{std::forward<Args>(args)...});
    }

    void deliver()
    {
        for (auto eventsPairIt = _events.begin();
                eventsPairIt != _events.end();) {
            auto handlersPairIt = _handlers.find(eventsPairIt->first);
            if (handlersPairIt != _handlers.end()) {
                const auto& events = eventsPairIt->second;
                for (const auto& event : events) {
                    sendEvent(event, handlersPairIt);
                }
            }
            eventsPairIt = _events.erase(eventsPairIt);
        }
    }

private:
    using Handler = std::function<bool(const std::any&)>;
    using HandlerMap = std::map<std::type_index, std::vector<Handler>>;

    void sendEvent(
        const std::any& event,
        HandlerMap::iterator handlersIter)
    {
        auto& handlers = handlersIter->second;

        for (auto handlerIt = handlers.begin(); handlerIt != handlers.end();) {
            if ((*handlerIt)(event)) {
                handlerIt++;
            } else {
                handlerIt = handlers.erase(handlerIt);
            }
        }

        if (handlers.empty()) {
            _handlers.erase(handlersIter);
        }
    }

    std::map<std::type_index, std::vector<std::any>> _events;
    HandlerMap _handlers;
};

class Subscriber {
public:
    virtual ~Subscriber() {}

protected:
    template <class Event>
    void subscribe(Channel& channel, std::function<void(const Event&)> handler)
    {
        channel.subscribe<Event>(std::weak_ptr<char>(_lifeTracker), handler);
    }

private:
    std::shared_ptr<char> _lifeTracker = std::make_shared<char>();
};

} // namespace evening