#include "catch.hpp"

#include <evening.hpp>

namespace ev = evening;

namespace {

struct Event {};

struct Element {};

struct Container {
    void createElement(ev::Channel& channel)
    {
        element = std::make_shared<Element>();
        channel.subscribe<Event>(element, [this] (const Event&) {
            counter++;
        });
    }

    void resetElement()
    {
        element.reset();
    }

    std::shared_ptr<Element> element;
    int counter = 0;
};

} // namespace

TEST_CASE("Subscribe to element", "[element]")
{
    Container container;
    ev::Channel channel;

    container.createElement(channel);
    channel.send(Event{});
    REQUIRE(container.counter == 1);

    container.resetElement();
    channel.send(Event{});
    REQUIRE(container.counter == 1);
}