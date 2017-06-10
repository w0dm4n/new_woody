#include <stdio.h>
#include "includes/all.h"

void			*ft_mmap(int fd, size_t size)
{
	return (mmap(0, size, PROT_READ, MAP_SHARED, fd, 0));
}

void			print_error(char *error, bool should_exit)
{
	printf("Woody Woodpecker is telling you: %s\n", error);
	if (should_exit)
		exit(should_exit);
}

/*
** Save new built executable
*/
static void         save_executable(void *buffer, int len)
{
	int			fd = open("slt", O_RDWR | O_CREAT, S_IRUSR | S_IRGRP | S_IROTH);
	if (fd != -1)
	{
		if ((write(fd, buffer, len)) == -1)
			print_error(strerror(errno), true);
	}
	else
		print_error(strerror(errno), true);
}

static void		read_crypted_section(struct elf64_shdr *header, void *buffer)
{
	void			*section_content	= (buffer + header->sh_offset);
	int				i					= 0;
	/*while (i < header->sh_size)
	{
		printf("%c\n", ((char*)section_content)[i]);
		i++;
	}*/
	//save_executable(section_content, header->sh_size);
	printf("offset on header: %d, size of section: %d\n", header->sh_offset,
header->sh_size);
}

int			main(int argc, char **argv)
{
	int     fd = 0, len = 0;
	void	*buffer = NULL;
    if ((fd = open(argv[0], O_RDONLY)) == -1)
		print_error(strerror(errno), true);
	if ((len = lseek(fd, 0, SEEK_END)) <= 0)
		print_error(strerror(errno), true);
	if (!(buffer = ft_mmap(fd, len)))
		print_error(strerror(errno), true);
	printf("....WOODY.....\n");
	read_crypted_section(buffer + (len - sizeof(struct elf64_shdr)), buffer);
}
