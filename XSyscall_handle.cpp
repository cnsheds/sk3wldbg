#include "XSyscall_handle.h"
#include <utility>
#include "android_memory.h"

interrupt_handle g_interrupt_handle;

void interrupt_handle::process_interrupt(uc_engine* uc, uint32_t intno)
{
	if (_handles.find(intno) != _handles.end())
	{
		_handles[intno](uc);
	}
}

void interrupt_handle::set_handle(uint32_t intno, std::function<int(uc_engine*)> handler)
{
	_handles[intno] = handler;
}

//================================================================

XSyscall_handle::XSyscall_handle()
{
	g_interrupt_handle.set_handle(2, std::bind(&XSyscall_handle::handle_syscall, this, std::placeholders::_1));
}


XSyscall_handle::~XSyscall_handle()
{
}

void XSyscall_handle::set_handle(uint32_t sysno, syscall_handle handler)
{
	_handles[sysno] = handler;
}

int XSyscall_handle::handle_syscall(uc_engine* uc)
{
	uint32_t idx = 0;
	uc_reg_read(uc, UC_ARM_REG_R7, &idx);
	if (_handles.find(idx) != _handles.end())
	{
		_args.zero();
		uc_reg_read(uc, UC_ARM_REG_R0, &_args.arg[0]);
		uc_reg_read(uc, UC_ARM_REG_R1, &_args.arg[1]);
		uc_reg_read(uc, UC_ARM_REG_R2, &_args.arg[2]);
		uc_reg_read(uc, UC_ARM_REG_R3, &_args.arg[3]);
		uc_reg_read(uc, UC_ARM_REG_R4, &_args.arg[4]);
		uc_reg_read(uc, UC_ARM_REG_R5, &_args.arg[5]);
		uc_reg_read(uc, UC_ARM_REG_R6, &_args.arg[6]);

		void* result = _handles[idx].callback(uc, _args);
		if (result != nullptr)
		{
			uc_reg_write(uc, UC_ARM_REG_R0, &result);
		}
	}

	return 0;
}
