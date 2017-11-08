#include "catch.hpp"

#include <evening/events.hpp>

#include <memory>
#include <iostream>

namespace ev = evening;

namespace {

struct EventA {};
struct EventB {};

struct SimpleSubscriber : ev::Subscriber {
    void subscribeToA(ev::Channel& channel)
    {
        subscribe<EventA>(channel, [this] (const EventA& event) {
            a++;
        });
    }

    void subscribeToB(ev::Channel& channel)
    {
        subscribe<EventB>(channel, [this] (const EventB& event) {
            b++;
        });
    }

    int a = 0;
    int b = 0;
};

} // namespace

TEST_CASE("Subscribe to itself", "[subscriber]")
{
    ev::Channel channel;
    SimpleSubscriber subscriber;

    SECTION("No subscriptions")
    {
        channel.send(EventA{});
        channel.send(EventB{});
        REQUIRE(subscriber.a == 0);
        REQUIRE(subscriber.b == 0);
    }

    SECTION("Some subscriptions")
    {
        subscriber.subscribeToA(channel);
        channel.send(EventA{});
        channel.send(EventB{});
        REQUIRE(subscriber.a == 1);
        REQUIRE(subscriber.b == 0);
    }

    SECTION("Multiple subscriptions")
    {
        subscriber.subscribeToA(channel);
        subscriber.subscribeToB(channel);
        channel.send(EventA{});
        channel.send(EventB{});
        REQUIRE(subscriber.a == 1);
        REQUIRE(subscriber.b == 1);
    }

    SECTION("Push and deliver")
    {
        subscriber.subscribeToA(channel);

        channel.push(EventA{});
        REQUIRE(subscriber.a == 0);

        channel.deliver();
        REQUIRE(subscriber.a == 1);
    }
}
