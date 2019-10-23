
#include "XSyscall_handle.h"
#include "mem_mgr.h"

// class unicorn_heap
// {
// public:
// 	unicorn_heap();
// 	int malloc(uint32_t size, int prot = UC_PROT_READ | UC_PROT_WRITE);
// 	def calloc(self, size, count);
// 	def realloc(self, ptr, new_size);
// 	def free(self, addr);
// 		def protect(self, addr, len_in, prot);
// };

class android_memory
{
public:
	android_memory();
	void set_memmgr(mem_mgr* mgr);
	android_memory(mem_mgr *);
	void* handle_mmap2(uc_engine* uc, syscall_args& args);
	void* handle_mprotect(uc_engine* uc, syscall_args& args);
	void* handle_munmap(uc_engine* uc, syscall_args& args);

	void init_hook(XSyscall_handle* psyshandle);
protected:
	mem_mgr *memmgr;
};
