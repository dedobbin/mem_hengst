#ifndef __MEM_HENGST_HPP__
#define __MEM_HENGST_HPP__

#include <unistd.h> 
#include <stdint.h>
#include <string>
#include <vector>

namespace mem_hengst
{
	struct Proc_map_entry
	{
		uint64_t address_start;
		uint64_t address_end;
		std::string permissions;
		std::string offset;
		std::string device;
		std::string inode;
		std::string pathname;
	};
	void print_pme(mem_hengst::Proc_map_entry pme);
	std::vector<Proc_map_entry> get_proc_map_entries(pid_t pid);
	void* read_process_mem(pid_t pid, uint64_t address_start, size_t bufferLength);
	bool write_process_mem(pid_t pid, uint64_t address_start, void* data, size_t buffer_len);
}


#endif