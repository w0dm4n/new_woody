/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   elf_writer.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frmarinh <frmarinh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/10 00:08:15 by frmarinh          #+#    #+#             */
/*   Updated: 2017/06/10 00:08:18 by frmarinh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

/*
** Save new built executable
*/
static void         save_executable(void *buffer, t_elf *elf)
{
	int			fd = open("woody", O_RDWR | O_CREAT, S_IRUSR | S_IRGRP | S_IROTH);
	if (fd != -1)
	{
		if ((write(fd, buffer, elf->len)) == -1)
			print_error(strerror(errno), true);
	}
	else
		print_error(strerror(errno), true);
}

/*
**  Write segments header of elf file in buffer
*/
static void         write_segments(void *buffer, t_elf *elf)
{
    t_segment			*segments		= elf->segments;
    int					i				= 0;
    void				*base_pointer	= buffer + elf->header->e_phoff;
    while (segments)
    {
        ft_memcpy(base_pointer + (sizeof(struct elf64_phdr) * i++), segments->data, sizeof(struct elf64_phdr));
        segments = segments->next;
    }
}

/*
**  Write elf header of elf file in buffer
*/
static void         write_elf_header(void *buffer, t_elf *elf)
{
    ft_memcpy(buffer, elf->header, sizeof(struct elf64_hdr));
}

static char			*get_section_name(t_elf *elf, int offset)
{
	char				*string_tab		= elf->buffer;
    string_tab += elf->first_section[elf->header->e_shstrndx].sh_offset;
	return (string_tab + offset);
}

/*
**  Write sections content of elf file in buffer
*/
static void         write_sections_content(void *buffer, t_elf *elf)
{
	t_section			*sections		= elf->sections;
	while (sections)
	{
        if (ft_strcmp(".bss", get_section_name(elf, sections->data->sh_name)))
            ft_memcpy(buffer + sections->data->sh_offset, sections->content, sections->data->sh_size);
        sections = sections->next;
	}
}

/*
**  Write sections header of elf file in buffer
*/
static void         write_sections_header(void *buffer, t_elf *elf)
{
    t_section			*sections		= elf->sections;
    void				*base_pointer	= buffer + elf->header->e_shoff;
    int                 i               = 0;
    while (sections)
    {
        ft_memcpy(base_pointer + ((sizeof(struct elf64_shdr) * i++)), sections->data, sizeof(struct elf64_shdr));
        sections = sections->next;
    }
}

/*
**  Add custom section in elf file and return start offset of the section
*/
static int         add_custom_section(void *buffer, t_elf *dst, t_elf *src)
{
    t_section			*sections	      = dst->sections;
    void                *section_pointer  = NULL;
    while (sections->next)
        sections = sections->next;
    section_pointer = buffer + (sections->data->sh_offset + sections->data->sh_size);
    ft_memcpy(section_pointer, src->buffer, src->len);
    //ft_memset(section_pointer, 'a', src->len);
    return (sections->data->sh_offset + sections->data->sh_size);
}

/*
** Write new structure header
*/
static void         write_custom_section_header(void *buffer, t_elf *dst, int section_offset, t_elf *src)
{
    struct elf64_shdr           *section_header = NULL;

    if (!(section_header = (struct elf64_shdr*)malloc(sizeof(struct elf64_shdr))))
        return;
    section_header->sh_offset = section_offset;
    section_header->sh_size = src->len;
    printf("Offset set: %d, size set: %d\n", section_header->sh_offset, section_header->sh_size);
    ft_memcpy(buffer + (dst->len - sizeof(struct elf64_shdr)), section_header, sizeof(struct elf64_shdr));
}

/*
** Write elf content in a new buffer and create a new file
*/
void                write_elf(t_elf *dst, t_elf* src)
{
    void        *buffer = NULL;
    int          offset_new_section = 0;

    dst->len += src->len + sizeof(struct elf64_shdr);
    if (!(buffer = (void*)malloc(dst->len)))
        return ;
    dst->header->e_shoff += src->len;
    write_elf_header(buffer, dst);
    write_segments(buffer, dst);
    offset_new_section = add_custom_section(buffer, dst, src);
    write_sections_content(buffer, dst);
    write_sections_header(buffer, dst);
    write_custom_section_header(buffer, dst, offset_new_section, src);
    save_executable(buffer, dst);
}
