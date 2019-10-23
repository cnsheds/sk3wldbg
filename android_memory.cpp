#include "android_memory.h"


android_memory::android_memory()
{
	memmgr = nullptr;
}

void android_memory::set_memmgr(mem_mgr * mgr)
{
	memmgr = mgr;
}

void* android_memory::handle_mmap2(uc_engine* uc, syscall_args& args)
{
	//void *mmap2(void *addr, size_t length, int prot, int flags, int fd, off_t pgoffset);
	// MAP_FILE	    0
	// MAP_SHARED	0x01
	// MAP_PRIVATE	0x02
	// MAP_FIXED	    0x10
	// MAP_ANONYMOUS	0x20
	//
	map_block *pblock = nullptr;
	if (args.arg[0])
	{
		if ((pblock = memmgr->mmap(args.arg[0], args.arg[1], args.arg[2], SDB_MAP_FIXED)))
			return reinterpret_cast<void*>(args.arg[0]);
	}
	else
		if ((pblock = memmgr->mmap(args.arg[0], args.arg[1], args.arg[2])))
			return reinterpret_cast<void*>(pblock->guest);
		
	return nullptr;
}

void* android_memory::handle_mprotect(uc_engine* uc, syscall_args& args)
{
	//int mprotect(void *addr, size_t len, int prot);
	memmgr->mprotect(args.arg[0], args.arg[1], args.arg[2]);
	return nullptr;
}

void* android_memory::handle_munmap(uc_engine* uc, syscall_args& args)
{
	memmgr->munmap(args.arg[0], args.arg[1]);
	return nullptr;
}

void android_memory::init_hook(XSyscall_handle* psyshandle)
{
	auto fn_munmap = std::bind(&android_memory::handle_munmap, this, std::placeholders::_1, std::placeholders::_2);
	auto fn_mprotect = std::bind(&android_memory::handle_mprotect, this, std::placeholders::_1, std::placeholders::_2);
	auto fn_mmap2 = std::bind(&android_memory::handle_mmap2, this, std::placeholders::_1, std::placeholders::_2);

	psyshandle->set_handle(0x5B, syscall_handle(0x5B, "munmap", 2, fn_munmap));
	psyshandle->set_handle(0x7D, syscall_handle(0x7D, "mprotect", 3, fn_mprotect));
	psyshandle->set_handle(0xC0, syscall_handle(0xC0, "mmap2", 6, fn_mmap2));
	//psyshandle->set_handle(0xDC, syscall_handle(0xDC, "madvise", 3, _handle_munmap));
}
