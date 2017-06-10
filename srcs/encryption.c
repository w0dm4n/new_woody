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

/*static char		*xor_char(int i, char c)
{
	char *ret;

	ret = int_to_hexastring(c ^ ENCRYPTION_KEY[i % ft_strlen(ENCRYPTION_KEY)]);
	return (ret);
}

static char		*unxor_char(int i, char c)
{
	char *ret;

	ret = ft_strnew(1);
	ret[0] = c ^ ENCRYPTION_KEY[i % ft_strlen(ENCRYPTION_KEY)];
	return (ret);
}

char			*encrypt_message(char *msg)
{
	int		i;
	char	*crypted;

	i = 0;
	crypted = ft_strnew(0);
	while (msg[i])
	{
		crypted = ft_strjoin(crypted, xor_char(i, msg[i]));
		i++;
	}
	return (ft_strreverse(crypted));
}

char			*decrypt_message(char *crypted)
{
	int		i;
	int		n;
	char	*msg;

	crypted = ft_strreverse(crypted);
	i = 0;
	n = 0;
	msg = ft_strnew(0);
	while (crypted[n + 1])
	{
		msg = ft_strjoin(msg, unxor_char(i,\
			hexastring_to_int(ft_strsub(crypted, n, 2))));
		i++;
		n += 2;
	}
	return (msg);
}*/

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
}
