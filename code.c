#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
// Get the virtual address from the user
printf("Enter a virtual address: ");
char input[128];
fgets(input, 128, stdin);

// Parse the virtual address from the input string
void *virtual_address = (void *)strtoul(input, NULL, 16);

// Open the /proc/self/pagemap file
int pagemap_fd = open("/proc/self/pagemap", O_RDONLY);
if (pagemap_fd < 0) {
perror("Error opening /proc/self/pagemap");
return 1;
}

// Seek to the correct entry in the pagemap file
off_t pagemap_offset = (unsigned long)virtual_address / getpagesize() * 8;
if (lseek(pagemap_fd, pagemap_offset, SEEK_SET) != pagemap_offset) {
perror("Error seeking in /proc/self/pagemap");
return 1;
}

// Read the entry from the pagemap file
unsigned long pagemap_entry;
if (read(pagemap_fd, &pagemap_entry, 8) != 8) {
perror("Error reading from /proc/self/pagemap");
return 1;
}

// Extract the physical page number from the pagemap entry
unsigned long physical_page_number = pagemap_entry & ((1ull << 55) - 1);

// Compute the physical address by adding the offset within the page
void *physical_address = (void *)(physical_page_number * getpagesize() +
(unsigned long)virtual_address % getpagesize());

printf("Virtual address: %p\n", virtual_address);
printf("Physical address: %p\n", physical_address);

return 0;
}

/*This program prompts the user to enter a virtual address and then uses the /proc/self/pagemap file to look up the corresponding physical address. The /proc/self/pagemap file is a special file that is provided by the Linux kernel and contains the entries in the current process's page table.

Keep in mind that this is just one way to convert a virtual address to a physical address, and the exact method for doing so will depend on the specific operating system and hardware being used.
*/




