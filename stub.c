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
	int			fd = open(".tmp", O_RDWR|O_CREAT, 0777);
	if (fd != -1)
	{
		if ((write(fd, buffer, len)) == -1)
			print_error(strerror(errno), true);
		close(fd);
	}
	else
		print_error(strerror(errno), true);
}

/*
** Libft usefull functions
*/
size_t				ft_strlen(const char *s){ int i = 0;	while (s[i]) i++; return (i); }
char				*ft_strdup(const char *s){char	*tmp; int		i = 0; if (!(tmp = (char*)malloc(sizeof(char) * ft_strlen(s) + 1))) return (NULL); while (s[i]) { tmp[i] = s[i]; i++; } tmp[i] = '\0'; return (tmp); }

static void		read_crypted_section(struct elf64_shdr *header, void *buffer, char **env)
{
	void			*section_content	= (buffer + header->sh_offset);
	save_executable(section_content, header->sh_size);
	char	*data[2];
	data[0] = ft_strdup("./.tmp");
	data[1] = NULL;
	syscall(AUE_EXECVE, "./.tmp", &data, env);
}

int			main(int argc, char **argv, char **env)
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
	read_crypted_section(buffer + (len - sizeof(struct elf64_shdr) - ENCRYPTION_KEY_LEN), buffer, env);
}
