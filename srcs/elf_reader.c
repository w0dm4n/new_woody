/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   elf_reader.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frmarinh <frmarinh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/09 20:35:10 by frmarinh          #+#    #+#             */
/*   Updated: 2017/06/09 20:35:13 by frmarinh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

/*
**  Copy segments of the ELF file
*/
static void     load_segments(t_elf *elf)
{
    struct elf64_phdr	*segment_header = elf->buffer + elf->header->e_phoff;
    int i = 0;
	while (i < elf->header->e_phnum)
		new_segment(elf, &segment_header[i++]);
}

/*
** Copy sections of the ELF file
*/
static void	    load_sections(t_elf *elf)
{
	struct elf64_shdr	*section = (struct elf64_shdr*) (elf->buffer + elf->header->e_shoff);
	int i = 0;
	while (i < elf->header->e_shnum)
	{
		new_section(elf, &section[i], elf->buffer + section[i].sh_offset);
		i++;
	}
}

/*
** Set first section header of the ELF file
*/
static void     load_first_section(t_elf *elf)
{
    elf->first_section = elf->buffer + elf->header->e_shoff;
}

/*
**  Copy content of the ELF file
*/
void            read_elf_content(t_elf *elf)
{
    load_segments(elf);
    load_sections(elf);
    load_first_section(elf);
}
