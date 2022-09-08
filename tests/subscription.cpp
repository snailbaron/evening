#include <catch2/catch_test_macros.hpp>

#include <evening.hpp>

namespace ev = evening;

namespace {

struct Event {};

struct SubscriptionHolder {
    void subscribe(ev::Channel& channel)
    {
        subscription = channel.subscribe<Event>([this] (const Event&) {
            counter++;
        });
    }

    void reset()
    {
        subscription.reset();
    }

    std::shared_ptr<char> subscription;
    int counter = 0;
};

} // namespace

TEST_CASE("Subscribe and hold subscription object", "[subscription]")
{
    SubscriptionHolder holder;
    ev::Channel channel;

    holder.subscribe(channel);
    channel.send(Event{});
    REQUIRE(holder.counter == 1);

    holder.reset();
    channel.send(Event{});
    REQUIRE(holder.counter == 1);
}
