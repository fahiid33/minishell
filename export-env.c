/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export-env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fstitou <fstitou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/01 22:29:30 by aainhaja          #+#    #+#             */
/*   Updated: 2022/07/16 00:11:56 by fstitou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	update_export(t_env **env, char *key, char sep, char *val)
{
	t_env	*tmp;
	int	k;

	tmp = (*env);
	k = 0;
	if (key[ft_strlen(key) - 1] == '+')
	{
		k = 1;
		key[ft_strlen(key) - 1] = 0;
	}
	while (tmp)
	{
		if (!strcmp(key, tmp->key))
		{
			if (k)
			{
				tmp->sep = sep;
				tmp->val = ft_strjoin(tmp->val, val, 0);
				return ;
			}
			else
			{
				tmp->sep = sep;
				tmp->val = val;
				return ;
			}
		}
		tmp = tmp->next;
	}
}
int	str_is_alnum(char *str)
{
	int	i;
	i = 0;
	while (str[i])
	{
		if (!ft_isalnum(str[i]))
			return (0);
		i++;
	}
	return (1);
}

int	check_exp_arg(char *to_check)
{
	if (to_check[strlen(to_check) - 1] == '+' && str_is_alnum(ft_substr(to_check, 0, strlen(to_check) - 1)))
		to_check = ft_substr(to_check, 0, strlen(to_check) - 1);
	if (str_is_alnum(to_check) == 0 || (to_check[0] >= '0' && to_check[0] <= '9'))
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(to_check, 2);
		ft_putstr_fd(": not a valid identifier\n", 2);
		g_vars.exit_status = 1;
		return (0);
	}
	return (1);
}