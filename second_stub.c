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
int					ft_strcmp(const char *s1, const char *s2) { int res; while (*s1 == *s2) { if (*s1 == '\0') return (0); s1++; s2++; } res = *(unsigned char *)s1 - *(unsigned char *)s2; return ((*(unsigned char *)s1 < *(unsigned char *)s2) ? res : res); }

static char		*get_section_name(void *buffer, int offset)
{
	struct elf64_hdr			*hdr = (struct elf64_hdr*) buffer;
	struct elf64_shdr			*first_section = buffer + hdr->e_shoff;
	struct elf64_shdr			*string_tab = &first_section[hdr->e_shstrndx];
	char						*strtab = buffer + string_tab->sh_offset;
	return (strtab + offset);
}

static void		*get_text(void *buffer)
{
	struct elf64_hdr			*hdr = (struct elf64_hdr*) buffer;
	struct elf64_shdr			*section = (struct elf64_shdr*) (buffer + hdr->e_shoff);
	int							i = 0;
	while (i < hdr->e_shnum)
	{
		if (!ft_strcmp(".text", get_section_name(buffer, section[i].sh_name)))
			return (buffer + section[i].sh_offset);
		i++;
	}
	return (NULL);
}

static void		read_crypted_section(struct elf64_shdr *header, void *buffer, char **env)
{
	char			*section_content	= (buffer + header->sh_offset);

	void				(*send)();

	/*send = binary_content + hdr->e_entry;
	char	**data = malloc(2);
	data[0] = ft_strdup("./.tmp");
	data[1] = NULL;
	send(1, data, env);*/
	//printf("%d\n", hdr->e_entry);
	//__starter(binary_content + hdr->e_entry);
	send	= get_text(buffer);
	char	*data[2];
	data[0] = ft_strdup("./woody");
	data[1] = NULL;
	send();
	//EXECUTEBUFFER(section_content);
	/*int i =0;
	while (i < header->sh_size)
	{
		printf("%d ", section_content[i++]);
	}
	printf("\n");*/
	/*save_executable(section_content, header->sh_size);

	syscall(AUE_EXECVE, "./.tmp", &data, env);*/
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
