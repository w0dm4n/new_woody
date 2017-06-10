/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   elf.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frmarinh <frmarinh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/09 19:47:54 by frmarinh          #+#    #+#             */
/*   Updated: 2017/06/09 19:48:00 by frmarinh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

/*
** Allocate a new s_elf struct
*/
static t_elf           *alloc_elf()
{
    return ((t_elf*)malloc(sizeof(struct s_elf)));
}

/*
** Get magic value of the current elf file
*/
static void		       get_magic(t_elf *elf)
{
	char	*value = NULL;
	char	*save_pointer = NULL;
	int		i = 0;

	if (!(value = malloc(MAGIC_LEN * 2)))
		return ;
	save_pointer = value;
	ft_bzero(elf->magic, MAGIC_LEN * 2);
	while (i < MAGIC_LEN)
	{
		asprintf(&value, "%x", elf->header->e_ident[i++]);
		ft_strncat((char*) elf->magic, value, 2);
		value += 2;
	}
	free(save_pointer);
}

/*
** Get arch type of the current elf file
*/
void		get_arch(t_elf *elf)
{
	elf->is_64 = (elf->header->e_ident[4] == ARCH_64) ? true : false;
}

/*
** Get endian type of the current elf file
*/
void		get_endian(t_elf *elf)
{
	int				endian = elf->header->e_ident[5];
	if (endian == L_ENDIAN)
		elf->little_endian = true;
	else if (endian == B_ENDIAN)
		elf->big_endian = true;
}

/*
** Read elf header of the current elf file
*/
static void            read_elf_header(t_elf *elf)
{
    struct elf64_hdr* header = (struct elf64_hdr*) elf->buffer;
    if (!(elf->header = malloc(sizeof(struct elf64_hdr))))
        return;
    ft_memcpy(elf->header, header, sizeof(struct elf64_hdr));
    get_magic(elf);
    get_arch(elf);
    get_endian(elf);
}

/*
** Open the elf file and read the content
*/
static void            open_elf_file(t_elf *elf)
{
    int     fd = 0;
    if ((fd = open(elf->file_name, O_RDONLY)) == -1)
		print_error(strerror(errno), true);
	if ((elf->len = lseek(fd, 0, SEEK_END)) <= 0)
		print_error(strerror(errno), true);
	if (!(elf->buffer = ft_mmap(fd, elf->len)))
		print_error(strerror(errno), true);
}

/*
** Check if the magic equals to the standard elf magic
*/
static bool		       is_elf_file(t_elf *elf)
{
	return (ft_strcmp(ELF_MAGIC_HEXA, (char*)elf->magic) == 0);
}

/*
** Read an elf file entirely
*/
t_elf           *read_elf(char  *file_name)
{
    t_elf           *elf = alloc_elf();

    elf->segments = NULL;
    elf->sections = NULL;
    elf->file_name = ft_strdup(file_name);
    open_elf_file(elf);
    read_elf_header(elf);
    if (is_elf_file(elf) && elf->is_64 && elf->little_endian)
        read_elf_content(elf);
    else
        print_error("I cannot handle file that are not of type ELF 64-bit with little endian !", true);
    return (elf);
}
