# mem_hengst
Allows a user to read and write memory process on a Linux system. 
**Note:** Currently always uses `process_vm_writev` and `process_vm_readv` syscalls, which are nonstandard  extensions.

## Install
```
make 
sudo make install
```
To link against, use `-lmem_hengst`

## Usage 

```
std::vector<mem_hengst::Proc_map_entry> mem_map = mem_hengst::get_proc_map_entries(some_pid);
uint8_t* data =  (uint8_t*)mem_hengst::read_process_mem(pid, target_address, buffer_len);
mem_hengst::write_process_mem(pid, start, data, buffer_len);
```
See example.cpp for more details. 


