/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_m_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paula <paula@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 09:01:23 by paula             #+#    #+#             */
/*   Updated: 2024/02/14 09:01:59 by paula            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	init_ex(t_exec *ex, t_dados *data)
{
	int	j;

	j = 0;
	while (data)
	{
		j++;
		data = data->next;
	}
	ex->count = 0;
	ex->i = 0;
	ex->nbr_pipes = j - 1;
}
