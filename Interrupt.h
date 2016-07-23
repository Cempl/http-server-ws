#pragma once


/*******************************************************************************/
#include <atomic>
#include <future>


/*******************************************************************************/
#include "Server.h"
#include "Var_for_threads.h"


/*******************************************************************************/
using namespace std;


/*******************************************************************************/
void interruption_point();


/*******************************************************************************/
class interrupt_flag
{
	protected:///////////////////////////////////////////////////////////////////
		
		atomic<bool> flag;
		condition_variable* thread_cond;
		mutex set_clear_mutex;


	public://////////////////////////////////////////////////////////////////////


		interrupt_flag();
		void set();
		bool is_set() const;
		void set_condition_variable(condition_variable& cv);
		void clear_condition_variable();

		struct clear_cv_on_destruct;
};


extern thread_local interrupt_flag this_thread_interrupt_flag;


/*******************************************************************************/
class interruptible_thread
{
	protected://////////////////////////////////////////////////////////////////

		thread internal_thread;
		interrupt_flag* flag;


	public://////////////////////////////////////////////////////////////////////

		template<typename FunctionType>
		interruptible_thread(FunctionType func)
		{
			promise<interrupt_flag*> prom;
			internal_thread = thread([func,&prom]
									 {
										prom.set_value(&this_thread_interrupt_flag);
										try
										{
											func();
										}
										catch(string str)
										{
											cout << str;
										}
									 });

			flag = prom.get_future().get();
		}

		template<typename FunctionType, class _Argument>
		interruptible_thread(FunctionType func, _Argument& arg)
		{
			promise<interrupt_flag*> prom;
			internal_thread = thread([func,&arg,&prom]
									 {
										prom.set_value(&this_thread_interrupt_flag);
										try
										{
											func(arg);
										}
										catch(string str)
										{
											cout << str;
										}
									 });

			flag = prom.get_future().get();
		}

		template<class FunctionType,  class _This, class _Argument>
		interruptible_thread(FunctionType func, _This* class_type, _Argument& arg)
		{
			promise<interrupt_flag*> prom;
			internal_thread = thread([func, &class_type, &arg, &prom]
									 {
										prom.set_value(&this_thread_interrupt_flag);
										try
										{
											class_type->send_data(arg);
										}
										catch(string str)
										{
											cout << str;
										}
									 });

			flag = prom.get_future().get();
		}

		void interrupt();
		void join();
};

/*******************************************************************************/
struct interrupt_flag::clear_cv_on_destruct
{
	~clear_cv_on_destruct()
	{
		this_thread_interrupt_flag.clear_condition_variable();
	}
};

/*******************************************************************************/
template<typename Predicate>
void interruptible_wait(condition_variable& cv, unique_lock<mutex>& lk, Predicate pred)
{
	interruption_point();
	this_thread_interrupt_flag.set_condition_variable(cv);
	interrupt_flag::clear_cv_on_destruct guard;
	while(!this_thread_interrupt_flag.is_set() && !pred())
	{
		cv.wait_for(lk, chrono::milliseconds(1));
	}
	interruption_point();
}