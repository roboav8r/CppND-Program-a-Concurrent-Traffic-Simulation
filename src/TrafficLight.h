#ifndef TRAFFICLIGHT_H
#define TRAFFICLIGHT_H

#include <mutex>
#include <deque>
#include <condition_variable>
#include <random>
#include "TrafficObject.h"

// forward declarations to avoid include cycle
class Vehicle;


template <class T>
class MessageQueue
{
public:
    void send(T &&msg);
    T receive();

private:
    std::deque<T> _queue;
    std::condition_variable _cond;
    std::mutex _mtx;
};

enum TrafficLightPhase{red, green};

class TrafficLight : public TrafficObject
{
public:
    // constructor / desctructor

    // getters / setters

    // typical behaviour methods
    void waitForGreen();
    void simulate();
    TrafficLightPhase getCurrentPhase();

private:
    // typical behaviour methods
    void cycleThroughPhases();

    // For random waitDuration generation
    std::default_random_engine _generator;
    std::uniform_int_distribution<int> _distribution{4000,6000};

    // Concurrency objects
    MessageQueue<TrafficLightPhase> _trafficLightQueue;
    std::condition_variable _condition;
    std::mutex _mutex;
    TrafficLightPhase _currentPhase{red};
};

#endif