#ifndef __thread__
#define __thread__

#include <pthread.h>

class Thread {
   public:
      Thread();
      virtual ~Thread();

      int start();
      int join();
      int detach();
      pthread_t self();

      virtual void* run() = 0;

   private:
      pthread_t m_threadID;
      int m_isRunning;
      int m_isDetached;
};

#endif
