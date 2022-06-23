/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fahd <fahd@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/16 02:26:33 by fahd              #+#    #+#             */
/*   Updated: 2022/06/18 09:27:39 by fahd             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char    ft_strchar(char *s, char c)
{
    char	*str;

	str = (char *)s;
	while (*str != c)
	{
		if (*str == '\0')
		{
			return (0);
		}
		str++;
	}
	return (c);
}

int	check_key(t_env **my_env, char *to_check)
{
	t_env	*tmp;

	tmp = (*my_env);
	if (to_check[ft_strlen(to_check ) - 1] == '+')
		to_check = ft_substr(to_check, 0, ft_strlen(to_check) - 1);
	while (tmp)
	{
		if (ft_strcmp(tmp->key, to_check) == 0)
			return (1);
		tmp = tmp->next;
	}
	return (0);
}

void	print_export(t_env *my_env)
{
	while (my_env)
	{
		if (!my_env->sep)
			printf("declare -x %s\n", my_env->key);
		else if (!my_env->val)
			printf("declare -x %s%c\"\"\n", my_env->key, my_env->sep);
		else
			printf("declare -x %s%c\"%s\"\n", my_env->key, my_env->sep, my_env->val);
		my_env = my_env->next;
	}
	g_vars.exit_status = 0;
}

void	run_export(t_env *my_env, char **argc)
{
	int	i;
	char	**tmp;

	i = 0;
	tmp = NULL;
	while (argc[i])
	{
		tmp = ft_split(argc[i], '=');
		if (check_exp_arg(tmp[0]))
		{
			if (check_key(&my_env, tmp[0]))
				update_export(&my_env, tmp[0], ft_strchar(argc[i] , '='), tmp[1]);
			else
				lst_add_backenv(&g_vars.my_env, lst_new(tmp[0],
					 ft_strchar(argc[i] , '='), tmp[1]));
		}
		i++;
	}
}

int	export(t_parse *head)
{
	t_env	*my_env;

	my_env = g_vars.my_env;
	if (!head->argv[0])
	{
		print_export(my_env);
	}
	else
		run_export(my_env, head->argv);
	return (g_vars.exit_status);
}