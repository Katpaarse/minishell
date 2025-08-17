/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_parser.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jukerste <jukerste@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 13:58:30 by jukerste          #+#    #+#             */
/*   Updated: 2025/08/17 14:29:07 by jukerste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_debug_cmds(t_cmd *cmds)
{
	int	i;
	int	j;

	i = 0;
	while (cmds)
	{
		printf("Command: %d:\n", i++);
		if (cmds->args)
		{
			j = 0;
			while (cmds->args[j])
			{
                printf("  arg[%d] = %s\n", j, cmds->args[j]);
				j++;
			}
        }
        if (cmds->infile)
            printf("  infile  = %s\n", cmds->infile);
        if (cmds->outfile)
            printf("  outfile = %s (append=%d)\n", cmds->outfile, cmds->append);
        cmds = cmds->next;
    }
}
