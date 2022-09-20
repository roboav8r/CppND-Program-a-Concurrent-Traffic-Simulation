#include <iostream>
#include <random>
#include "TrafficLight.h"

/* Implementation of class "MessageQueue" */

/* 
template <typename T>
T MessageQueue<T>::receive()
{
    // FP.5a : The method receive should use std::unique_lock<std::mutex> and _condition.wait() 
    // to wait for and receive new messages and pull them from the queue using move semantics. 
    // The received object should then be returned by the receive function. 
}
*/

template <typename T>
void MessageQueue<T>::send(T &&msg)
{
    // FP.4a : The method send should use the mechanisms std::lock_guard<std::mutex> 
    // as well as _condition.notify_one() to add a new message to the queue and afterwards send a notification.
}

/* Implementation of class "TrafficLight" */

/* 
TrafficLight::TrafficLight()
{
    _currentPhase = TrafficLightPhase::red;
}

void TrafficLight::waitForGreen()
{
    // FP.5b : add the implementation of the method waitForGreen, in which an infinite while-loop 
    // runs and repeatedly calls the receive function on the message queue. 
    // Once it receives TrafficLightPhase::green, the method returns.
}
*/

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
            _messageQueue.send(std::move(_currentPhase));

            // Reset the loop start time to compute the next waitDuration
            loopStartTime = std::chrono::high_resolution_clock::now();
        }

    }
}