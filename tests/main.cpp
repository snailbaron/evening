#include <evening/events.hpp>

#include <iostream>
#include <cassert>

using namespace evening;

struct EventOne {
    int value = 0;
};

struct EventTwo {
    int value = 0;
};

class PrintSubscriber : public Subscriber {
public:
    PrintSubscriber(Channel& channel)
    {
        subscribe<EventOne>(channel, [] (const EventOne& event) {
            std::cout << "Print: " << event.value << std::endl;
        });
    }
};

int main()
{
    Channel channel;
    assert(channel.eventCount() == 0);
    assert(channel.handlerCount() == 0);

    {
        PrintSubscriber printer(channel);
        assert(channel.handlerCount() == 1);

        channel.send(EventOne{1});
        channel.send(EventOne{2});
        assert(channel.eventCount() == 0);

        channel.push(EventOne{3});
        channel.push(EventOne{4});
        assert(channel.eventCount() == 2);

        channel.deliver();
        assert(channel.eventCount() == 0);
    }

    assert(channel.handlerCount() == 1);
    channel.send(EventOne{5});
    assert(channel.handlerCount() == 0);
    assert(channel.eventCount() == 0);
}