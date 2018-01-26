#include "catch.hpp"

#include <evening.hpp>

#include <memory>
#include <iostream>

namespace ev = evening;

namespace {

struct EventA {};
struct EventB {};

struct SimpleSubscriber : ev::Subscriber {
    void subscribeToA(ev::Channel& channel)
    {
        subscribe<EventA>(channel, [this] (const EventA&) {
            a++;
        });
    }

    void subscribeToB(ev::Channel& channel)
    {
        subscribe<EventB>(channel, [this] (const EventB&) {
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

    REQUIRE(subscriber.a == 0);
    REQUIRE(subscriber.b == 0);

    SECTION("No subscriptions")
    {
        SECTION("send") {
            channel.send(EventA{});
            channel.send(EventB{});
            REQUIRE(subscriber.a == 0);
            REQUIRE(subscriber.b == 0);
        }
        SECTION("makeSend") {
            channel.makeSend<EventA>();
            channel.makeSend<EventB>();
            REQUIRE(subscriber.a == 0);
            REQUIRE(subscriber.b == 0);
        }
        SECTION("push") {
            channel.push(EventA{});
            channel.push(EventB{});
            REQUIRE(subscriber.a == 0);
            REQUIRE(subscriber.b == 0);
            channel.deliver();
            REQUIRE(subscriber.a == 0);
            REQUIRE(subscriber.b == 0);
        }
        SECTION("makePush") {
            channel.makePush<EventA>();
            channel.makePush<EventB>();
            REQUIRE(subscriber.a == 0);
            REQUIRE(subscriber.b == 0);
            channel.deliver();
            REQUIRE(subscriber.a == 0);
            REQUIRE(subscriber.b == 0);
        }
    }

    SECTION("Single subscription")
    {
        subscriber.subscribeToA(channel);
        REQUIRE(subscriber.a == 0);
        REQUIRE(subscriber.b == 0);

        SECTION("send") {
            channel.send(EventA{});
            channel.send(EventB{});
            REQUIRE(subscriber.a == 1);
            REQUIRE(subscriber.b == 0);
        }
        SECTION("makeSend") {
            channel.makeSend<EventA>();
            channel.makeSend<EventB>();
            REQUIRE(subscriber.a == 1);
            REQUIRE(subscriber.b == 0);
        }
        SECTION("push") {
            channel.push(EventA{});
            channel.push(EventB{});
            REQUIRE(subscriber.a == 0);
            REQUIRE(subscriber.b == 0);
            channel.deliver();
            REQUIRE(subscriber.a == 1);
            REQUIRE(subscriber.b == 0);
        }
        SECTION("makePush") {
            channel.makePush<EventA>();
            channel.makePush<EventB>();
            REQUIRE(subscriber.a == 0);
            REQUIRE(subscriber.b == 0);
            channel.deliver();
            REQUIRE(subscriber.a == 1);
            REQUIRE(subscriber.b == 0);
        }
    }

    SECTION("Multiple subscriptions")
    {
        subscriber.subscribeToA(channel);
        subscriber.subscribeToB(channel);
        REQUIRE(subscriber.a == 0);
        REQUIRE(subscriber.b == 0);

        SECTION("send") {
            channel.send(EventA{});
            channel.send(EventB{});
            REQUIRE(subscriber.a == 1);
            REQUIRE(subscriber.b == 1);
        }
        SECTION("makeSend") {
            channel.makeSend<EventA>();
            channel.makeSend<EventB>();
            REQUIRE(subscriber.a == 1);
            REQUIRE(subscriber.b == 1);
        }
        SECTION("push") {
            channel.push(EventA{});
            channel.push(EventB{});
            REQUIRE(subscriber.a == 0);
            REQUIRE(subscriber.b == 0);
            channel.deliver();
            REQUIRE(subscriber.a == 1);
            REQUIRE(subscriber.b == 1);
        }
        SECTION("makePush") {
            channel.makePush<EventA>();
            channel.makePush<EventB>();
            REQUIRE(subscriber.a == 0);
            REQUIRE(subscriber.b == 0);
            channel.deliver();
            REQUIRE(subscriber.a == 1);
            REQUIRE(subscriber.b == 1);
        }
    }
}
