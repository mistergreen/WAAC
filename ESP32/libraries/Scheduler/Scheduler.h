
#ifndef Scheduler_h
#define Scheduler_h

#include "Storable.h"

class Scheduler : public Storable
{
  public:
    // Class constructor, it sets the default values.
    Scheduler();
    
    // Class destructor, nothing to deallocate.
    virtual ~Scheduler() {};
    
    // It sets the events string.
    void setEvent(char *in_string);
    
    // It gets the events string.
    void getEvent(char *string);
    
    // It returns true if there is an event in progress.
    bool isEventInProgress();
    
    // It returns true if there are scheduled events.
    bool areScheduledEvents();
    
    // It serializes the class into a Json document.
    void serialize(
        // Input Json object pointer to be filled with the class information.
        JsonObject& doc);
    
    // It fills the class using the information contained into the document.
    void deserialize(
        // Input Json object pointer containing the class information.
        JsonObject& doc);
    
  private:
    
    // The amount of timed events.
    int8_t timedIndexCounter;
    
    // It stores the hours.
    int hour[5];
    int minute[5];
    int second[5];
    
    // It stores the durations.
    int hourDuration[5];
    int minuteDuration[5];
    int secondDuration[5];
    
    // It stores the days of week.
    char dow[5][8];
    
    long convertToSeconds(int in_hour, int in_minute, int in_second);
    
    void stripTime(char *inString, int *inArray);
};

#endif