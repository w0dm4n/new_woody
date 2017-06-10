/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frmarinh <frmarinh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/09 19:24:28 by frmarinh          #+#    #+#             */
/*   Updated: 2017/06/09 19:24:29 by frmarinh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

int     main(int argc, char **argv)
{
    if (argc == 2)
    {
        t_elf   *stub = read_elf("stub");
        t_elf   *to_encrypt = read_elf(argv[1]);
        write_elf(stub, to_encrypt);
    }
    else
        print_error("Woody Woodpecker informs you that you need to give him an executable to encrypt !", true);
    return (0);
}
