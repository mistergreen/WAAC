
#ifndef Scheduler_h
#define Scheduler_h

#include "Storable.h"
#include "Device.h"
#include <DeviceDelegate.h>

class EventHandler : public Storable
{
  public:
    // Class constructor, it sets the default values.
    EventHandler();

    // Class constructors it sets the passed dependent device.
    EventHandler(int in_dependent_device_id);
    
    // Class destructor, nothing to deallocate.
    virtual ~EventHandler() {};
    
    // It sets the events string.
    virtual void setEvent(char *in_string);
    
    // It gets the events string.
    virtual void getEvent(char *string);

    // It returns the dependendent device ID.
    virtual int getDependentDevice();

    // It sets the dependent device ID.
    virtual void setDependentDevice(int id);

    void setSuspendTime(bool in_suspend);

    // The main loop that executes the scheduling.
    void loop();

    // It serializes the class into a Json document.
    void serialize(
        // Input Json object pointer to be filled with the class information.
        JsonObject& doc);
    
    // It fills the class using the information contained into the document.
    void deserialize(
        // Input Json object pointer containing the class information.
        JsonObject& doc);

  protected:
    // The maximum number of events.
    static const int sMAX_NUM_EVENTS = 5;

    // It returns true if there is an event in progress.
    bool isEventInProgress();
    
    // It returns true if there are scheduled events.
    bool areScheduledEvents();

    // It is the action performed during an event.
    // To be implemented by the externding class.
    virtual void performActionInEvent() {};

    // It is the action performed out of an event.
    // To be implemented by the externding class.
    virtual void performActionOutEvent() {};

    // It is the action performed not during an event by a dependent device.
    virtual void performActionInEventNoSchedule();

    // It is the action performed out not during an event by a dependent device.
    virtual void performActionOutEventNoSchedule();

    // It returns the percentage of time elapsed in an event.
    float getEventPercentage();

    // It reurns if the timer is suspended.
    bool getSuspendTime();

    // The ID of the event in progress, -1 if no event is in progress.
    int inProgressEventId;
    
    // The ID of the last event in progress, -1 if no event is in progress.
    int lastEventId;
    
  private:

    int dependentDeviceId;
    Device *dependentDeviceObject;

    // The amount of timed events.
    int8_t timedIndexCounter;

    // It suspends the events checking.
    bool suspendTime;
    
    // It stores the hours.
    int hour[sMAX_NUM_EVENTS];
    int minute[sMAX_NUM_EVENTS];
    int second[sMAX_NUM_EVENTS];
    
    // It stores the durations.
    int hourDuration[sMAX_NUM_EVENTS];
    int minuteDuration[sMAX_NUM_EVENTS];
    int secondDuration[sMAX_NUM_EVENTS];
    
    // It stores the days of week.
    char dow[sMAX_NUM_EVENTS][8];

    long convertToSeconds(int in_hour, int in_minute, int in_second);
    
    void stripTime(char *inString, int *inArray);
};

#endif