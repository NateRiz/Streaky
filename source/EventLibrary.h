/**
 * @author      : nathan (nathan@$HOSTNAME)
 * @file        : EventLibrary
 * @created     : Monday Aug 26, 2019 14:29:40 EDT
 */
#pragma once
#include "base/Ptr.h"

#include "ConfigHardware.h"

/// Used for EventDriven tests instead of polling with the Sense instruction.
/// CMD line args -EVENT_DRIVEN [0-2]
/// 0 - Polling
/// 1 - Events
/// 2 - Polling and Events
template<typename CH>
class EventLibrary{
public:

  using event_lib_t = typename CH::event_lib_t;
  using event_t = typename event_lib_t::event_t;
  using hardware_t = typename CH::hardware_t;

  EventLibrary()=default;

  event_lib_t& CreateEventLib(){
    event_lib.AddEvent("NextBit",
      [](hardware_t& hw, const event_t& event){
        hw.SpawnCore(event.affinity, 1.0);
      }
    );

    event_lib.RegisterDispatchFun("NextBit",
      [](hardware_t& hw, const event_t& event){
        hw.QueueEvent(event);
      }   
    
    );

    return event_lib;
  }
  

private:
  event_lib_t event_lib;
};
