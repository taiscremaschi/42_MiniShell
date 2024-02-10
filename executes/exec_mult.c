/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_mult.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: paula <paula@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/25 09:43:52 by paula             #+#    #+#             */
/*   Updated: 2024/02/10 10:45:57 by paula            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_handle_red_pipes(t_dados *data, t_env *my_env)
{
	int	number_red;
	int	i;

	number_red = data->nbr_redirections;
	//printf("nbr red eh %d\n", number_red);
	i = 0;
	while (number_red--)
	{
		if (data->redirect[i].redirect_type == 0
			|| data->redirect[i].redirect_type == 3)
		{
		//	printf("output\n");
			if (!redirect_output(&data->redirect[i]))
				exit_child(data, my_env);
		}
		if (data->redirect[i].redirect_type == 1
			|| data->redirect[i].redirect_type == 2)
		{
			if (!redirect_input(&data->redirect[i]))
				exit_child(data, my_env);
		}
	//	printf("somando i\n");
		i++;
	}
}

void	ft_handle_pipe(t_dados *aux, t_dados *data, int back_out)
{
	static int	pipe_fd[2];

	if (aux != data)
		redirect_fd(pipe_fd[READ_END], STDIN_FILENO);
	if (aux->next)
	{
		if (pipe(pipe_fd) < 0)
			ft_child_err("pipe", aux->cmd[0]);
		redirect_fd(pipe_fd[WRITE_END], STDOUT_FILENO);
	}
	else
		redirect_fd(back_out, STDOUT_FILENO);
}

int	ft_handle_exec(t_dados *aux, t_env *my_env, int nbr_pipes)
{
//	printf("entrou no exec\n");
	int	exit_status;

	exit_status = 0;
	if (!ft_cmd_builtin(aux))
	{
		ft_exec_child_process(aux, my_env);
	}
	else if (!nbr_pipes)
		exit_status = ft_execute_builtin(aux, &my_env);
	else
	{
		exit_status = ft_execute_builtin(aux, &my_env);
		close(STDOUT_FILENO);
		close(STDIN_FILENO);
		close(STDERR_FILENO);
		exit(exit_status);
	}
	return (exit_status);
}

// int	ft_execute_multiple_cmd(t_dados *data, t_env *my_env)
// {
// 	int		saved_fds[2];
// 	pid_t	*children_pid;
// 	int		i;
// 	t_dados	*aux;

// 	ft_save_fds(saved_fds);
// 	children_pid = ft_alloc(data);
// 	aux = data;
// 	i = 0;
// 	while (aux)
// 	{
// 		ft_handle_pipe(aux, data, saved_fds[OUT]);
// 		children_pid[i] = fork();
// 		check_child_pid(children_pid[i], aux);
// 		ft_def_signal(children_pid[i]);
// 		if (!children_pid[i++])
// 		{
// 			ft_handle_red_pipes(aux, my_env);
// 			ft_handle_exec(aux, my_env);
// 		}
// 		aux = aux->next;
// 	}
// 	back_saved_fd(saved_fds);
// 	return (wait_for_children(children_pid));
// }
