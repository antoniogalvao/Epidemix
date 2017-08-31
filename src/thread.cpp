#include <include/thread.h>

static void* runThread(void *arg)
{
   return ((Thread*)arg)->run();
}

Thread::Thread() : m_threadID(0), m_isRunning(0), m_isDetached(0) {}

Thread::~Thread()
{
   if (m_isRunning == 1 && m_isDetached == 0)
   {
      pthread_detach(m_threadID);
   }
   if(m_isRunning == 1)
   {
      pthread_cancel(m_threadID);
   }
}

int Thread::start()
{
   int returnCode = pthread_create(&m_threadID, NULL, runThread, this);
   if(returnCode == 0)
   {
      m_isRunning = 1;
   }
   return returnCode;
}

int Thread::join()
{
   int returnCode = -1;
   if (m_isRunning == 1)
   {
      returnCode = (pthread_join(m_threadID,NULL));
      if (returnCode == 0)
      {
         m_isDetached = 0;
      }
   }
   return returnCode;
}

int Thread::detach()
{
   int returnCode = -1;
   if(m_isRunning == 1 && m_isDetached == 0)
   {
      returnCode = pthread_detach(m_threadID);
      if (returnCode == 0)
      {
         m_isDetached = 1;
      }
   }
   return returnCode;
}

pthread_t Thread::self()
{
   return m_threadID;
}
