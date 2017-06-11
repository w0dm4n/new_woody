/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   encryption.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frmarinh <frmarinh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/10 04:06:22 by frmarinh          #+#    #+#             */
/*   Updated: 2017/06/10 04:06:27 by frmarinh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

/*
** Apply xor on a char
*/
static char		xor_char(int i, char c, char *key)
{
	return (c ^ key[i % ENCRYPTION_KEY_LEN]);
}


/*
** Encrypt data
*/
void			*encrypt_binary(char *binary, int binary_len, char *key)
{
	int		i = 0;
    char    *encrypted = NULL;

    if (!(encrypted = (char*)malloc(binary_len)))
        return (NULL);
	while (i < binary_len)
	{
        encrypted[i] = xor_char(i, binary[i], key);
        i++;
	}
    return (encrypted);
}

/*
** Generate a random encryption key
*/
static char            *generate_encryption_key()
{
    char        *key = NULL;

    if (!(key = (char*)malloc(ENCRYPTION_KEY_LEN)))
        return (NULL);
    int i = 0;
    while (i < ENCRYPTION_KEY_LEN)
        key[i++] = get_rand(126);
    return (key);
}

/*
** Write random encryption key to buffer
*/
static void             write_key_to_elf(char *key, void *buffer, t_elf *elf)
{
    void        *pointer = buffer + elf->len - ENCRYPTION_KEY_LEN;
    ft_memcpy(pointer, key, ENCRYPTION_KEY_LEN);
}

/*
** Encrypt ELF src into dst
*/
void            encrypt_src(void *buffer, t_elf *dst, t_elf *src)
{
    char        *key = generate_encryption_key();
    write_key_to_elf(key, buffer, dst);
    src->buffer = encrypt_binary(src->buffer, src->len, key);
}
