#include <unistd.h> 
#include <vector>
#include <iostream>
#include <mem_hengst.hpp>

int main(int arc, char* argv[])
{
	int proc_map_index = 12;

	pid_t pid = getpid();
	std::vector<mem_hengst::Proc_map_entry> mem_map = mem_hengst::get_proc_map_entries(pid);
	
	mem_hengst::print_pme(mem_map[proc_map_index]);

	if (mem_map[proc_map_index].permissions.substr(1,1) != "w"){
		std::cout << "No permission to write to memory" << std::endl;
		return 1;
	}

	uint64_t start = mem_map[proc_map_index].address_start;
	int buffer_len = 4;
	
	uint8_t* data = (uint8_t*)mem_hengst::read_process_mem(pid, start, buffer_len);
	printf("Read '%s'\n", data);

	uint8_t new_data[buffer_len] = "test";
	printf("Will write '%s'\n", new_data);
	mem_hengst::write_process_mem(pid, start, new_data, buffer_len);	

	free(data);
	data = (uint8_t*)mem_hengst::read_process_mem(pid, start, buffer_len);
	printf("Read '%s'\n", data);
	free(data);

	return 0;
}