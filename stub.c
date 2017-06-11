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
size_t				ft_strlen(const char *s){ int i = 0; while (s[i]) i++; return (i); }
char				*ft_strdup(const char *s){char	*tmp; int i = 0; if (!(tmp = (char*)malloc(sizeof(char) * ft_strlen(s) + 1))) return (NULL); while (s[i]) { tmp[i] = s[i]; i++; } tmp[i] = '\0'; return (tmp); }

/*
** Unapply xor on a char
*/
static char		unxor_char(int i, char c, char *key)
{
	return (c ^ key[i % ENCRYPTION_KEY_LEN]);
}

/*
** Decrypt encrypted data
*/
void			*decrypt_binary(char *crypted, int crypted_len, char *key)
{
	char        *decrypted = NULL;

    if (!(decrypted = (char*)malloc(crypted_len)))
        return (NULL);
	int		i = 0;
	while (i < crypted_len)
	{
        decrypted[i] = unxor_char(i, crypted[i], key);
		i++;
	}
	return (decrypted);
}

/*
** Get encryption key hided in the buffer
*/
char			*get_encryption_key(void *buffer, int len)
{
	return (buffer + len - ENCRYPTION_KEY_LEN);
}

/*
** Read crypted elf binary in last section
*/
static void		read_crypted_section(struct elf64_shdr *header, void *buffer, char **env, int total_len)
{
	char			*key 			= get_encryption_key(buffer, total_len);
	char			*binary_content	= decrypt_binary((buffer + header->sh_offset), header->sh_size, key);

	save_executable(binary_content, header->sh_size);
	char	*data[2];
	data[0] = ft_strdup("./.tmp");
	data[1] = NULL;
	syscall(AUE_EXECVE, "./.tmp", &data, env);
}

/*
** Entry main of decrypt program
*/
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
	read_crypted_section(buffer + (len - sizeof(struct elf64_shdr) - ENCRYPTION_KEY_LEN), buffer, env, len);
}
