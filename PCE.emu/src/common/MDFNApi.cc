#define LOGTAG "main"
#include <mednafen/mednafen.h>
#include <mednafen/general.h>
#include <mednafen/state-driver.h>
#include <mednafen/movie.h>
#include <mednafen/cputest/cputest.h>
#include <mednafen/MemoryStream.h>
#include <imagine/logger/logger.h>
#include <thread>
#include <mutex>
#include <condition_variable>

struct MDFN_Thread : public std::thread
{
	using thread::thread;
};
struct MDFN_Mutex : public std::mutex {};
struct MDFN_Cond : public std::condition_variable {};

#ifndef NDEBUG
void MDFN_printf(const char *format, ...) noexcept
{
	if(!logger_isEnabled())
		return;
	va_list args;
	va_start( args, format );
	logger_vprintf(LOG_M, format, args);
	va_end( args );
}

void MDFN_Notify(MDFN_NoticeType t, const char* format, ...) noexcept
{
	if(!logger_isEnabled())
		return;
	va_list args;
	va_start( args, format );
	logger_vprintf(LOG_E, format, args);
	va_end( args );
}
#endif

void MDFND_OutputNotice(MDFN_NoticeType t, const char* s) noexcept
{
	if(!Config::DEBUG_BUILD || !logger_isEnabled())
		return;
	logMsg("%s", s);
}

int MDFN_SavePNGSnapshot(const char *fname, const MDFN_Surface *src, const MDFN_Rect *rect, const MDFN_Rect *LineWidths)
{
	return 1;
}

void MDFN_ResetMessages(void) { }

MDFN_Thread *MDFND_CreateThread(void* (*fn)(void *), void *data)
{
	return new MDFN_Thread{fn, data};
}

void MDFND_WaitThread(MDFN_Thread *thread, int *status)
{
	thread->join();
	delete thread;
}

MDFN_Mutex *MDFND_CreateMutex(void)
{
	MDFN_Mutex *mutex = new MDFN_Mutex();
	assert(mutex);
	return mutex;
}

void MDFND_DestroyMutex(MDFN_Mutex *mutex)
{
	delete mutex;
}

int MDFND_LockMutex(MDFN_Mutex *mutex)
{
	//logMsg("lock %p", mutex);
	mutex->lock();
	return 0;
}

int MDFND_UnlockMutex(MDFN_Mutex *mutex)
{
	//logMsg("unlock %p", mutex);
	mutex->unlock();
	return 0;
}

MDFN_Cond* MDFND_CreateCond(void)
{
	MDFN_Cond *cond = new MDFN_Cond();
	assert(cond);
	return cond;
}

void MDFND_DestroyCond(MDFN_Cond* cond)
{
	delete cond;
}

int MDFND_SignalCond(MDFN_Cond* cond)
{
	cond->notify_one();
	return 0;
}

int MDFND_WaitCond(MDFN_Cond* cond, MDFN_Mutex* mutex)
{
	//logMsg("waiting %p on mutex %p", cond, mutex);
	std::unique_lock<std::mutex> lock{*mutex, std::adopt_lock};
	cond->wait(lock);
	lock.release();
	return 0;
}

void GetFileBase(const char *f) { }

void MDFN_indent(int indent) { }
int MDFN_RawInputStateAction(StateMem *sm, int load, int data_only) { return 1; }
void MDFND_SetMovieStatus(StateStatusStruct *status) noexcept {}
void MDFND_SetStateStatus(StateStatusStruct *status) noexcept {}
void NetplaySendState(void) { }
void MDFND_NetplayText(const char *text, bool NetEcho) {}

int MDFNnetplay=0;

void MDFN_StateAction(StateMem *sm, const unsigned load, const bool data_only)
{
	SFORMAT StateRegs[]{SFEND};
	MDFNSS_StateAction(sm, load, data_only, StateRegs, "MDFNRINP", true);

	if(data_only)
		MDFNMOV_StateAction(sm, load);

	MDFNGameInfo->StateAction(sm, load, data_only);
}

void MDFN_MidLineUpdate(EmulateSpecStruct *espec, int y) {}

#if defined(__i386__) || defined(__x86_64__)
int ff_get_cpu_flags_x86(void)
{
	int flags = CPUTEST_FLAG_CMOV | CPUTEST_FLAG_MMX | CPUTEST_FLAG_SSE | CPUTEST_FLAG_SSE2;
	return flags;
}
#endif

bool Stream::isMemoryStream() { return false; }

bool MemoryStream::isMemoryStream() { return true; }

namespace Time
{

int64 EpochTime(void)
{
 time_t ret = time(nullptr);
 return (int64)ret;
}

}
