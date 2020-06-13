#include "mem_hengst.hpp"

#include <iostream>
#include <regex>
#include <fstream>
#include <sys/uio.h>

void mem_hengst::print_pme(mem_hengst::Proc_map_entry pme)
{
	std::cout << pme.address_start << "-" << pme.address_start << std::endl;
	std::cout << "permissions: \t " << pme.permissions << std::endl;
	std::cout << "offset: \t " << pme.offset << std::endl;
	std::cout << "device: \t " << pme.device << std::endl;
	std::cout << "inode: \t\t " << pme.inode << std::endl;
	std::cout << "pathname: \t " << pme.pathname << std::endl;
}

void* mem_hengst::read_process_mem(pid_t pid, uint64_t address_start, size_t buffer_len)
{
    struct iovec local[1];
    local[0].iov_base = calloc(buffer_len, sizeof(char));
    local[0].iov_len = buffer_len;

    struct iovec remote[1];
    remote[0].iov_base = (void *)address_start;
    remote[0].iov_len = buffer_len;

    ssize_t n_read = process_vm_readv(pid, local, 1, remote, 1, 0);

    if (n_read < 0) {
		std::cerr << "Error in mem_hengst read: " << strerror(errno) << std::endl;
        return NULL;
    }

	//printf("Read '%s' from %u of process %d\n",local[0].iov_base, address_start, pid);

    return local[0].iov_base;
}

bool mem_hengst::write_process_mem(pid_t pid, uint64_t address_start, void* data, size_t buffer_len)
{
    struct iovec src[1];
    src[0].iov_base = data;
    src[0].iov_len = buffer_len;

    struct iovec dst[1];
    dst[0].iov_base = (void *)address_start;
    dst[0].iov_len = buffer_len;
	int n_write = process_vm_writev(pid, src, 1, dst, 1, 0);
	
	//printf("Will write '%s' to %u of process %d\n",src[0].iov_base, address_start, pid);

	if (n_write < 0){
		int j = EFAULT;
		std::cerr << "Error in mem_hengst write: " << strerror(errno) << std::endl;
		return false;
	}
	return true;
}

std::vector<mem_hengst::Proc_map_entry> mem_hengst::get_proc_map_entries(pid_t pid)
{
	std::vector<Proc_map_entry> res;
	std::ifstream file("/proc/" + std::to_string(pid) + "/maps");
	if (file.is_open()) {
		std::string line;
		while (std::getline(file, line)) {
			//printf("%s\n", line.c_str());
			std::regex regex{R"([ ]+)"};
			std::sregex_token_iterator it{line.begin(), line.end(), regex, -1};
			std::vector<std::string> splits{it, {}};

			std::regex regex2{R"([-]+)"};
			std::sregex_token_iterator it2{splits[0].begin(), splits[0].end(), regex2, -1};
			std::vector<std::string> address_range{it2, {}};

			Proc_map_entry pme{
				strtol( address_range[0].c_str(), NULL, 16 ),
				strtol( address_range[1].c_str(), NULL, 16 ),
				splits[1],
				splits[2],
				splits[3],
				splits[4],
				splits.size() == 6 ? splits[5] : "",
			};
			//mem_hengst::print_pme(pme);
			res.push_back(pme);
		}
		file.close();
	}
	return res;
}
