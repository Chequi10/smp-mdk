#ifndef SMP_SCHEDULER_H
#define SMP_SCHEDULER_H

#include "IScheduler.h"
#include "Service.h"

namespace Smp {
namespace Services {

class Scheduler : public IScheduler, public Service
{
public:
    Scheduler();

    ~Scheduler();

    /// Add immediate event.
    /// Add an immediate event to the scheduler.
    /// @param   entryPoint Entry point to call from event.
    void AddImmediateEvent(const IEntryPoint* entryPoint);

    /// Add event relative to simulation time.
    /// Add event to scheduler that is called based on simulation time.
    /// @param   entryPoint Entry point to call from event.
    /// @param   simulationTime Duration from now when to trigger event.
    /// @param   cycleTime Duration between two triggers of the event.
    /// @param   count Number of times the event shall be repeated, 
    ///                or 0 for a single event, or -1 for no limit.
    /// @return  Identifier that can be used to change or remove event.
    /// @remarks An event with <code>count=0</code> is not cyclic.
    ///          It will be removed automatically after it has been
    ///          triggered.
    ///          @par
    ///          An event with <code>count>0</code> is cyclic, and will
    ///          be repeated <code>count</code> times. Therefore, it
    ///          will be called <code>count+1</code> times, and then it
    ///          will be removed automatically.
    ///          @par
    ///          An event with <code>count=-1</code> is cyclic as well,
    ///          but it will be triggered forever, unless it is removed
    ///          from the scheduler using the RemoveEvent() method.
    ///          @par
    ///          For a cyclic event, the cycleTime needs to be positive.
    ///          For non-cyclic events, it is ignored.
    ///          @par
    ///          The simulationTime must not be negative. Otherwise, the
    ///          event will never be executed, but immediately removed.
    EventId AddSimulationTimeEvent(
        const IEntryPoint* entryPoint,
        const Duration simulationTime,
        const Duration cycleTime = 0,
        const Int64 count = 0);

    /// Add event relative to mission time.
    /// Add event to scheduler that is called based on mission time.
    /// @param   entryPoint Entry point to call from event.
    /// @param   missionTime Absolute mission time when to trigger event.
    /// @param   cycleTime Duration between two triggers of the event.
    /// @param   count Number of times the event shall be repeated, 
    ///                or 0 for a single event, or -1 for no limit.
    /// @return  Identifier that can be used to change or remove event.
    /// @remarks An event with <code>count=0</code> is not cyclic.
    ///          It will be removed automatically after it has been
    ///          triggered.
    ///          @par
    ///          An event with <code>count>0</code> is cyclic, and will
    ///          be repeated <code>count</code> times. Therefore, it
    ///          will be called <code>count+1</code> times, and then it
    ///          will be removed automatically.
    ///          @par
    ///          An event with <code>count=-1</code> is cyclic as well,
    ///          but it will be triggered forever, unless it is removed
    ///          from the scheduler using the RemoveEvent() method.
    ///          @par
    ///          For a cyclic event, the cycleTime needs to be positive.
    ///          For non-cyclic events, it is ignored.
    ///          @par
    ///          The missionTime must not be before the current mission 
    ///          time of the ITimeKeeper service. Otherwise, the
    ///          event will never be executed, but immediately removed.
    EventId AddMissionTimeEvent(
        const IEntryPoint* entryPoint,
        const Duration missionTime,
        const Duration cycleTime = 0,
        const Int64 count = 0);

    /// Add event relative to epoch time.
    /// Add event to scheduler that is called based on epoch time.
    /// @param   entryPoint Entry point to call from event.
    /// @param   epochTime Epoch time when to trigger event.
    /// @param   cycleTime Duration between two triggers of the event.
    /// @param   count Number of times the event shall be repeated, 
    ///                or 0 for a single event, or -1 for no limit.
    /// @return  Identifier that can be used to change or remove event.
    /// @remarks An event with <code>count=0</code> is not cyclic.
    ///          It will be removed automatically after it has been
    ///          triggered.
    ///          @par
    ///          An event with <code>count>0</code> is cyclic, and will
    ///          be repeated <code>count</code> times. Therefore, it
    ///          will be called <code>count+1</code> times, and then it
    ///          will be removed automatically.
    ///          @par
    ///          An event with <code>count=-1</code> is cyclic as well,
    ///          but it will be triggered forever, unless it is removed
    ///          from the scheduler using the RemoveEvent() method.
    ///          @par
    ///          For a cyclic event, the cycleTime needs to be positive.
    ///          For non-cyclic events, it is ignored.
    ///          @par
    ///          The epochTime must not be before the current epoch
    ///          time of the ITimeKeeper service. Otherwise, the
    ///          event will never be executed, but immediately removed.
    EventId AddEpochTimeEvent(
        const IEntryPoint* entryPoint,
        const DateTime epochTime,
        const Duration cycleTime = 0,
        const Int64 count = 0);

    /// Add event relative to Zulu time.
    /// Add event to scheduler that is called based on Zulu time.
    /// @param   entryPoint Entry point to call from event.
    /// @param   zuluTime Absolute (Zulu) time when to trigger event.
    /// @param   cycleTime Duration between two triggers of the event.
    /// @param   count Number of times the event shall be repeated, 
    ///                or 0 for a single event, or -1 for no limit.
    /// @return  Identifier that can be used to change or remove event.
    /// @remarks An event with <code>count=0</code> is not cyclic.
    ///          It will be removed automatically after it has been
    ///          triggered.
    ///          @par
    ///          An event with <code>count>0</code> is cyclic, and will
    ///          be repeated <code>count</code> times. Therefore, it
    ///          will be called <code>count+1</code> times, and then it
    ///          will be removed automatically.
    ///          @par
    ///          An event with <code>count=-1</code> is cyclic as well,
    ///          but it will be triggered forever, unless it is removed
    ///          from the scheduler using the RemoveEvent() method.
    ///          @par
    ///          For a cyclic event, the cycleTime needs to be positive.
    ///          For non-cyclic events, it is ignored.
    ///          @par
    ///          The zuluTime must not be before the current Zulu
    ///          time of the ITimeKeeper service. Otherwise, the
    ///          event will never be executed, but immediately removed.
    EventId AddZuluTimeEvent(
        const IEntryPoint* entryPoint,
        const DateTime zuluTime,
        const Duration cycleTime = 0,
        const Int64 count = 0);

    /// Set event execution time using simulation time.
    /// Update when an existing event on the scheduler shall be called.
    /// @param   event Identifier of event to modify.
    /// @param   simulationTime Duration from now when to trigger event.
    /// @remarks The event identifier must be valid, i.e. an identifier
    ///          of a scheduler event that has not yet been removed.
    ///          @par
    ///          The simulationTime must not be negative. Otherwise, the
    ///          event will never be executed, but immediately removed.
    void SetEventSimulationTime(const EventId event, const Duration simulationTime);

    /// Set event execution time using mission time.
    /// Update when an existing event on the scheduler shall be called.
    /// @param   event Identifier of event to modify.
    /// @param   missionTime Absolute mission time when to trigger event.
    /// @remarks The event identifier must be valid, i.e. an identifier
    ///          of a scheduler event that has not yet been removed.
    ///          @par
    ///          The missionTime must not be before the current mission 
    ///          time of the ITimeKeeper service. Otherwise, the
    ///          event will never be executed, but immediately removed.
    void SetEventMissionTime(const EventId event, const Duration missionTime);

    /// Set event execution time using epoch time.
    /// Update when an existing event on the scheduler shall be called.
    /// @param   event Identifier of event to modify.
    /// @param   epochTime Epoch time when to trigger event.
    /// @remarks The event identifier must be valid, i.e. an identifier
    ///          of a scheduler event that has not yet been removed.
    ///          @par
    ///          The epochTime must not be before the current epoch
    ///          time of the ITimeKeeper service. Otherwise, the
    ///          event will never be executed, but immediately removed.
    void SetEventEpochTime(const EventId event, const DateTime epochTime);

    /// Set event execution time using Zulu time.
    /// Update when an existing event on the scheduler shall be called.
    /// @param   event Identifier of event to modify.
    /// @param   zuluTime Absolute (Zulu) time when to trigger event.
    /// @remarks The event identifier must be valid, i.e. an identifier
    ///          of a scheduler event that has not yet been removed.
    ///          @par
    ///          The zuluTime must not be before the current Zulu
    ///          time of the ITimeKeeper service. Otherwise, the
    ///          event will never be executed, but immediately removed.
    void SetEventZuluTime(const EventId event, const DateTime zuluTime);

    /// Set event cycle time.
    /// Update cycle time of an existing event on the scheduler.
    /// @param   event Identifier of event to modify.
    /// @param   cycleTime Duration between two triggers of the event.
    /// @remarks The event identifier must be valid, i.e. an identifier
    ///          of a scheduler event that has not yet been removed.
    ///          @par
    ///          For a cyclic event, the cycleTime needs to be positive.
    ///          For non-cyclic events, it is ignored.
    void SetEventCycleTime(const EventId event, const Duration cycleTime);

    /// Set event count.
    /// Update the count of an existing event on the scheduler.
    /// @param   event Identifier of event to modify.
    /// @param   count Number of times the event shall be repeated, 
    ///                or 0 for a single event, or -1 for no limit.
    /// @remarks The event identifier must be valid, i.e. an identifier
    ///          of a scheduler event that has not yet been removed.
    ///          @par
    ///          An event with <code>count=0</code> is not cyclic.
    ///          It will be removed automatically after it has been
    ///          triggered.
    ///          @par
    ///          An event with <code>count>0</code> is cyclic, and will
    ///          be repeated <code>count</code> times. Therefore, it
    ///          will be called <code>count+1</code> times, and then it
    ///          will be removed automatically.
    ///          @par
    ///          An event with <code>count=-1</code> is cyclic as well,
    ///          but it will be triggered forever, unless it is removed
    ///          from the scheduler using the RemoveEvent() method.
    ///          @par
    ///          For a cyclic event, the cycleTime needs to be positive.
    ///          For non-cyclic events, it is ignored.
    void SetEventCount(const EventId event, const Int64 count);

    /// Remove an event from the scheduler.
    /// @param   event Event identifier of the event to remove.
    /// @remarks The event identifier must be valid, i.e. an identifier
    ///          of a scheduler event that has not yet been removed.
    ///          An event with <code>count>=0</code> is removed 
    ///          automatically after it has been triggered
    ///          <code>count+1</code> times, and is no longer accessable.
    void RemoveEvent(const EventId event);

private:

};

} // end namespace Services
} // end namespace Smp

#endif // SMP_SCHEDULER_H