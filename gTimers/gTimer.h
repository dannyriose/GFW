#ifndef GTIMER_H
#define GTIMER_H

#include <gString/gString.h>
#include <gObjectEngineModel/gSharedEngineModel/gSharedEngineModel.h>
#include "gTimersCommon.h"
namespace gfw{
    //! Type of timers
    enum GTIMER_TYPE{
        GTIMER_TYPE_SYSTEM,
        GTIMER_TYPE_VIRTUAL,
        GTIMER_TYPE_NOTSET
    };

    //! Base class for timers
    class SHARED_GFW gTimer: public gSharedEngineModel
    {
    protected:
        GTIMER_TYPE m_timertype;
        //! Call backfunction if specified
        ITimerCallBack m_callback;
        //! Callback object
        gTimerCallBack *m_callbackObject;
        //! Intervar
        gu32 m_interval;
        //! User data
        void *m_userdata;
        //! is running
        bool m_running;
        //! periodic
        bool m_periodic;

    public:
        gTimer():gSharedEngineModel(GEM_TIMER),
            m_timertype(GTIMER_TYPE_NOTSET),
            m_callback(0),
            m_callbackObject(0),
            m_interval(0),
            m_userdata(0),
            m_running(0),
            m_periodic(true)
        {

        }
        gTimer(const gTimer &other):gSharedEngineModel(other),
            m_timertype(other.m_timertype),
            m_callback(other.m_callback),
            m_callbackObject(other.m_callbackObject),
            m_interval(other.m_interval),
            m_userdata(other.m_userdata),
            m_running(other.m_running),
            m_periodic(other.m_periodic){

        }
        gTimer(const gString &_sid,gs32 _iid,GTIMER_TYPE _timertype):
            gSharedEngineModel(GEM_TIMER,_sid,_iid),
            m_timertype(_timertype),
            m_callback(0),
            m_callbackObject(0),
            m_interval(0),
            m_userdata(0),
            m_running(0),
            m_periodic(true){

        }
        gTimer &operator = (const gTimer &other);
       ~gTimer(){

        }
        virtual void timeOut(){
            if(m_callback)
                m_callback(this);
            if(m_callbackObject){
                m_callbackObject->setTimer(this);
                m_callbackObject->timeOut();
            }
        }


        inline void setInterval(gu32 msInterval){
            m_interval=msInterval;
        }
        inline const gu32 &getInterval() const{
            return m_interval;
        }
        inline void setCallBackFunction(ITimerCallBack func){
            m_callback=func;
        }
        inline ITimerCallBack getCallBackFunction(){
            return m_callback;
        }
        inline void setCallBackObject(gTimerCallBack *obj){
            m_callbackObject=obj;
        }
        inline gTimerCallBack *getCallBackObject(){
            return m_callbackObject;
        }
        inline void setUserData(void *userdata){
            m_userdata=userdata;
        }
        inline void *getUserData(){
            return m_userdata;
        }
        inline void setPeriodic(bool bSet){
            m_periodic=bSet;
        }
        inline bool isPeriodic() const{
            return m_periodic;
        }

        virtual void start(){
            m_running=true;
        }
        virtual void stop(){
            m_running=false;
        }

        virtual void poll(){

        }
        virtual void setEnable(bool bSet){
            m_running = bSet;
        }

        virtual gu32 getRealTime() const;
        virtual gu64 getRealTimeus() const;
        virtual gu32 getTime(){
            return 0;
        }
        //! Creates timer given a type.
        /** \param _type: Type of timer to creater. System or virtual.
         * \return pointer to new created timer. call gTimer::deleteTimer to delete pointer.*/
        static gTimer *createTimer(GTIMER_TYPE _type);
        //! Deletes a previous created timer by gTimer::createTimer.
        /** \param _timer: Pointer to timer object.*/
        static void deleteTimer(gTimer *_timer);

        bool isRunning() const{return m_running;}

        gs32 timerType() const
        {
            return m_timertype;
        }

    };
}
#endif // GTIMER_H
