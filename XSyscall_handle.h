#pragma once

#include <unicorn/unicorn.h>
#include <string>
#include <functional>
#include <map>

class interrupt_handle
{
public:
	void process_interrupt(uc_engine *uc, uint32_t intno);
	void set_handle(uint32_t intno, std::function<int(uc_engine*)>handler);
protected:
	std::map<uint32_t, std::function<int(uc_engine*)>> _handles;
};

extern interrupt_handle g_interrupt_handle;

//================================================================
struct syscall_args
{
	uint64_t arg[7];
	void zero()
	{
		memset(arg, 0, sizeof(arg));
	}
};

class syscall_handle
{
public:
	int idx;
	std::string name;
	int arg_count;
	std::function<void*(uc_engine*, syscall_args&)> callback;

	syscall_handle()
	{		
	}
	syscall_handle(int idx, std::string name, int arg_count, std::function<void*(uc_engine*, syscall_args&)> cb)
	{
		this->idx = idx;
		this->name = name;
		this->arg_count = arg_count;
		this->callback = cb;
	}
	syscall_handle& operator=(syscall_handle& right)
	{
		this->idx = right.idx;
		this->name = right.name;
		this->arg_count = right.arg_count;
		this->callback = right.callback;

		return *this;
	}
};


class XSyscall_handle
{
public:
	XSyscall_handle();
	~XSyscall_handle();

	void set_handle(uint32_t intno, syscall_handle handler);
	int handle_syscall(uc_engine*);
	syscall_args _args;

protected:
	std::map<uint32_t, syscall_handle> _handles;
};

