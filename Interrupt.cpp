/*******************************************************************************/
#include "Interrupt.h"


/*******************************************************************************/
thread_local interrupt_flag this_thread_interrupt_flag;


/*******************************************************************************/
interrupt_flag::interrupt_flag()
:
thread_cond{0}
{
}


/*******************************************************************************/
void interrupt_flag::set()
{
	flag.store(true, memory_order_relaxed);
	lock_guard<mutex> lk(set_clear_mutex);
	if(thread_cond)
	{
		thread_cond -> notify_all();
	}
}


/*******************************************************************************/
bool interrupt_flag::is_set() const
{
	return flag.load(memory_order_relaxed);
}


/*******************************************************************************/
void interrupt_flag::set_condition_variable(condition_variable& cv)
{
	lock_guard<mutex> lk(set_clear_mutex);
	thread_cond = &cv;
}


/*******************************************************************************/
void interrupt_flag::clear_condition_variable()
{
	lock_guard<mutex> lk(set_clear_mutex);
	thread_cond = 0;
}


/*******************************************************************************/
void interruptible_thread::interrupt()
{
	if(flag)
	{
		flag -> set();
	}
}


/*******************************************************************************/
void interruptible_thread::join()
{
	internal_thread.join();
}


/*******************************************************************************/
void interruption_point()
{
	if(this_thread_interrupt_flag.is_set())
	{
		throw interrException("Interrupt");
	}
}

