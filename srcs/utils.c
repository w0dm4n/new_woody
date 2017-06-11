/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frmarinh <frmarinh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/09 19:45:52 by frmarinh          #+#    #+#             */
/*   Updated: 2017/06/09 19:45:53 by frmarinh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

/*
** Convert int to hexastring
*/
char			*int_to_hexastring(char c)
{
	char		*res;

	res = ft_itoabase_uint(c, "0123456789ABCDEF");
	if (ft_strlen(res) < 2)
		res = ft_strjoin("0", res);
	return (res);
}

/*
** Convert hexastring to int
*/
char			hexastring_to_int(char *s)
{
	char xor;

	xor = ft_atoi_base(s, 16);
	ft_strdel(&s);
	return (xor);
}

/*
** Generate a random number
*/
int			get_rand(int max)
{
	struct timeval time;
	syscall(AUE_GETTIMEOFDAY, &time);
	return (time.tv_usec % max);
}

/*
** Allocate size from a file descriptor
*/
void		*ft_mmap(int fd, size_t size)
{
	return (mmap(0, size, PROT_READ, MAP_SHARED, fd, 0));
}

/*
** Print an error and exit if should_exit
*/
void		print_error(char *error, bool should_exit)
{
	printf("Woody Woodpecker is telling you: %s\n", error);
	if (should_exit)
		exit(should_exit);
}

/*
** Add new segment to list in elf file
*/
static void			add_segment(t_elf *elf, t_segment *segment)
{
	t_segment	*segments = elf->segments;
	if (segments)
	{
		while (segments->next)
			segments = segments->next;
		segments->next = segment;
		segment->prev = segments;
	}
	else
		elf->segments = segment;
}

/*
** Add new section to list in elf file
*/
static void			add_section(t_elf *elf, t_section *section)
{
	t_section	*sections = elf->sections;
	if (sections)
	{
		while (sections->next)
			sections = sections->next;
		sections->next = section;
		section->prev = sections;
	}
	else
		elf->sections = section;
}

/*
** Copy segment structure and allocate new segment in elf file
*/
void	       new_segment(t_elf *elf, struct elf64_phdr	*data)
{
	t_segment			*segment = malloc(sizeof(struct s_segment));
	struct elf64_phdr	*new = malloc(sizeof(struct elf64_phdr));
	ft_memcpy(new, data, sizeof(struct elf64_phdr));
	segment->data = new;
	segment->prev = NULL;
	segment->next = NULL;
	add_segment(elf, segment);
}

/*
** Copy section structure and content and allocate new section in elf file
*/
void			new_section(t_elf *elf, struct elf64_shdr *data, void *buffer)
{
	t_section			*section = malloc(sizeof(struct s_section));
	struct elf64_shdr	*new = malloc(sizeof(struct elf64_shdr));
	void				*new_buffer = malloc(data->sh_size);

	ft_memcpy(new_buffer, buffer, data->sh_size);
	ft_memcpy(new, data, sizeof(struct elf64_shdr));
	section->data = new;
	section->content = new_buffer;
	section->prev = NULL;
	section->next = NULL;
	add_section(elf, section);
}
