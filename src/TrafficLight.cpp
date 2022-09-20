#include <iostream>
#include <random>
#include "TrafficLight.h"

/* Implementation of class "MessageQueue" */
 
template <typename T>
T MessageQueue<T>::receive()
{
    std::unique_lock<std::mutex> uLock(_mtx);
    _cond.wait(uLock,[this]{return _queue.size() > 0;});
    return std::move(_queue.pop_front());
}

template <typename T>
void MessageQueue<T>::send(T &&msg)
{
    std::lock_guard<std::mutex> lGuard(_mtx);
    _queue.clear();
    _queue.emplace_back(msg);
    _cond.notify_one();
}

/* Implementation of class "TrafficLight" */

/* 
TrafficLight::TrafficLight()
{
    _currentPhase = TrafficLightPhase::red;
}

*/
void TrafficLight::waitForGreen()
{
    // FP.5b : add the implementation of the method waitForGreen, in which an infinite while-loop 
    // runs and repeatedly calls the receive function on the message queue. 
    // Once it receives TrafficLightPhase::green, the method returns.
}

TrafficLightPhase TrafficLight::getCurrentPhase()
{
    return _currentPhase;
}


void TrafficLight::simulate()
{
    threads.emplace_back(std::thread(&TrafficLight::cycleThroughPhases, this));
}


// virtual function which is executed in a thread
void TrafficLight::cycleThroughPhases()
{
    // initialize loop start time, wait time distribution, and start infinite loop
    std::chrono::_V2::system_clock::time_point loopStartTime = std::chrono::high_resolution_clock::now();
    int waitDuration;

    while (true) {

        // Check if time difference exceeds specification
        waitDuration = _distribution(_generator);
        std::this_thread::sleep_for(std::chrono::milliseconds(1));

        if ( std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - loopStartTime) > std::chrono::milliseconds(waitDuration) ) {

            // Toggle status of light
            switch (getCurrentPhase())
            {
            case TrafficLightPhase::red:
                _currentPhase = TrafficLightPhase::green;
                break;

            case TrafficLightPhase::green:
                _currentPhase = TrafficLightPhase::red;
                break;
            }

            // Send update message to queue
            _trafficLightQueue.send(std::move(_currentPhase));

            // Reset the loop start time to compute the next waitDuration
            loopStartTime = std::chrono::high_resolution_clock::now();
        }

    }
}