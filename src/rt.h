/*
 	 The Real-Time eXperiment Interface (RTXI)
	 Copyright (C) 2011 Georgia Institute of Technology, University of Utah, Weill Cornell Medical College

	 This program is free software: you can redistribute it and/or modify
	 it under the terms of the GNU General Public License as published by
	 the Free Software Foundation, either version 3 of the License, or
	 (at your option) any later version.

	 This program is distributed in the hope that it will be useful,
	 but WITHOUT ANY WARRANTY; without even the implied warranty of
	 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	 GNU General Public License for more details.

	 You should have received a copy of the GNU General Public License
	 along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#ifndef RT_H
#define RT_H

#include <fifo.h>
#include <mutex.h>
#include <pthread.h>
#include <semaphore.h>
#include <settings.h>
#include <list>

//! Realtime Oriented Classes
/*!
 * Objects contained within this namespace are responsible
 *   for managing realtime execution.
 */
namespace RT
{

namespace OS
{

typedef void * Task;

int initiate(void);
void shutdown(void);

int createTask(Task *,void *(*)(void *),void *,int =0);
void deleteTask(Task);

int setPeriod(Task,long long);
void sleepTimestep(Task);

bool isRealtime(void);

/*!
 * Returns the current CPU time in nanoseconds. In general
 *   this is really only useful for determining the time
 *   between two events.
 *
 * \return The current CPU time.
 */
long long getTime(void);

} // namespace OS

/*!
 * A token passed to the realtime task through System::postEvent()
 *   for synchronization.
 *
 * \sa RT::System::postEvent()
 */
class Event
{

public:

    Event(void);
    virtual ~Event(void);

    /*!
     * Function called by the realtime task in System::postEvent()
     *
     * \sa RT::System::postEvent()
     */
    virtual int callback(void)=0;

    // TODO: Add documentation for event wait function
    void wait(void);

    // TODO: Create Getter for retval? if so also add documentation
    int retval;

    // TODO: Add documentation for execute function in Event
    void execute(void);

private:

    sem_t signal;

}; // class Event

/*!
 * Base class for devices that are to interface with System.
 *
 * \sa RT::System
 */
class Device 
{

public:

    Device(void);
    virtual ~Device(void);

    /*! \fn virtual void read(void)
     * Function called by the realtime task at the beginning of each period.
     *
     * \sa RT::System
     */
    /*! \fn virtual void write(void)
     * Function called by the realtime task at the end of each period.
     *
     * \sa RT::System
     */

    /**********************************************************
     * read & write must not be pure virtual because they can *
     *    be called during construction and destruction.      *
     **********************************************************/

    virtual void read(void) {};
    virtual void write(void) {};

    inline bool getActive(void) const
    {
        return active;
    };
    void setActive(bool);

private:

    bool active;

}; // class Device

/*!
 * Base class for objects that are to interface with System.
 *
 * \sa RT::System
 */
class Thread
{

public:

    typedef unsigned long Priority;

    static const Priority MinimumPriority = 0;
    static const Priority MaximumPriority = 100;
    static const Priority DefaultPriority = MaximumPriority/2;

    Thread(Priority p =DefaultPriority);
    virtual ~Thread(void);

    /*!
     * Returns the priority of the thread. The higher the
     *   priority the sooner the thread is called in the
     *   timestep.
     *
     * \return The priority of the thread.
     */
    Priority getPriority(void) const
    {
        return priority;
    };

    /*! \fn virtual void execute(void)
     * Function called periodically by the realtime task.
     *
     * \sa RT::System
     */

    /**********************************************************
     * execute must not be pure virtual because it can be     *
     *   called during construction and destruction.          *
     **********************************************************/

    virtual void execute(void) {};

    inline bool getActive(void) const
    {
        return active;
    };
    void setActive(bool);

private:

    bool active;
    Priority priority;

}; // class Thread

/*!
 * Manages the RTOS as well as all objects that require
 *   realtime execution.
 */
class System
{

public:

    /*!
     * System is a Singleton, which means that there can only be one instance.
     *   This function returns a pointer to that single instance.
     *
     * \return The instance of System.
     */
    static System *getInstance(void);

    /*!
     * Get the current period of the System in nanoseconds.
     *
     * \return The current period
     */
    long long getPeriod(void) const
    {
        return period;
    };
    /*!
     * Set a new period for the System in nanoseconds.
     *
     * \param period The new desired period.
     * \return 0 on success, A negative value upon failure.
     */
    int setPeriod(long long period);

    /*!
     * Loop through each Device and executes a callback.
     * The callback takes two parameters, a Device pointer and param,
     *   the second parameter to foreachDevice.
     *
     * \param callback The callback function.
     * \param param A parameter to the callback function.
     * \sa RT::Device
     */
    void foreachDevice(void (*callback)(Device *,void *),void *param);
    /*!
     * Loop through each Thread and executes a callback.
     * The callback takes two parameters, a Thread pointer and param,
     *   the second parameter to foreachThread.
     *
     * \param callback The callback function
     * \param param A parameter to the callback function
     * \sa RT::Thread
     */
    void foreachThread(void (*callback)(Thread *,void *),void *param);

    /*!
     * Post an Event for execution by the realtime task, this acts as a
     *   mechanism to synchronizing with the realtime task.
     *
     * \param event The event to be posted.
     * \param blocking If true the call to postEvent is blocking.
     * \return The value returned from event->callback()
     * \sa RT:Event
     */
    int postEvent(Event *event,bool blocking =true);

    // TODO: Add documentation for insertDevice, removeDevice, insertThread, and
    // removeThread
    void insertDevice(Device *);
    void removeDevice(Device *);

    void insertThread(Thread *);
    void removeThread(Thread *);

private:

    /******************************************************************
     * The constructors, destructor, and assignment operator are made *
     *   private to control instantiation of the class.               *
     ******************************************************************/

    System(void);
    ~System(void);
    System(const System &) : eventFifo(0) {};
    System &operator=(const System &)
    {
        return *getInstance();
    };

    class SetPeriodEvent : public RT::Event
    {

    public:

        SetPeriodEvent(long long);
        ~SetPeriodEvent(void);

        int callback(void);

    private:

        long long period;

    }; // class SetPeriodEvent


    static System *instance;

    Mutex threadMutex;
    Mutex deviceMutex;
    static void *bounce(void *);
    void execute(void);

    bool finished;
    pthread_t thread;
    RT::OS::Task task;
    long long period;

    std::list<RT::Device *> devices;
    std::list<RT::Thread *> threadList;

    Fifo eventFifo;

}; // class System


} // namespace RT

#endif // RT_H
