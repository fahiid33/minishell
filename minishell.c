/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fahd <fahd@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/15 18:45:32 by fahd              #+#    #+#             */
/*   Updated: 2022/07/16 17:39:42 by fahd             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strndup(char *str, unsigned int n)
{
	char				*new;
	unsigned int		i;

	i = 0;
	new = malloc(sizeof(char) * (n + 1));
	while (i < n)
		new[i++] = *str++;
	new[n] = 0;
	return (new);
}

void print_list(t_token *lst)
{
   while (lst)
   {
      printf("value == %s --------  type: %d --------  flag: %d \n", lst->val, lst->type, lst->flag);
      lst = lst->next;
   }
}

void print_l(t_parse *lst)
{
   int i;
   while (lst)
   {
      i  =0;
      if (lst->cmd)
      {
         printf("cmd == %s args: ", lst->cmd);
         while(lst->argv && lst->argv[i])
         {
            printf("%s  ",lst->argv[i]);
            i++;
         }
         
         while(lst->redir)
         {
            printf(" redir => ");
            printf("%s ; ", lst->redir->file);
            lst->redir = lst->redir->next;
         }
         if (lst->next->next)
            printf("  | \n");
         else
            printf("\n");
      }
      lst = lst->next;
   }
}

char	*str_join(char *s1, char *s2)
{
	char	*copy;
	int		i;
	int		j;

	copy = malloc(sizeof(char) * (ft_int_strchr(s1, 0) + ft_int_strchr(s2, 0) + 2));
	i = 0;
	j = 0;
	while (s1[j])
		copy[i++] = s1[j++];
	copy[i++] = '/';
	j = 0;
	while (s2[j])
		copy[i++] = s2[j++];
	copy[i] = 0;
	return (copy);
}


char	*get_path(char *cmd, char **env)
{
	char	*path;
	char	*dir;
	char	*bin;
	int		i;

	i = 0;
	while (env[i] && strncmp(env[i], "PATH=", 5))
		i++;
	if (!env[i])
		return (cmd);
	path = env[i] + 5;
	while (path && ft_int_strchr(path, ':') > -1)
	{
		dir = ft_strndup(path, ft_int_strchr(path, ':'));
		bin = str_join(dir, cmd);
		free(dir);
		if (access(bin, F_OK) == 0)
			return (bin);
		free(bin);
		path += ft_int_strchr(path, ':') + 1;
	}
	return (cmd);
}

int array_size(char **str)
{
	int i = 0;
	while (str[i])
		i++;
	// printf("%d\n", i);
	return (i);
}

char	**my_envir(char **env)
{
   char   **str;
   int   i = 0;
   int	j = 0;
   int size = array_size(env);
   if(getenv("OLDPWD") != NULL)
      str = (char **)malloc(sizeof (char *) * (size));
   else
      str = (char **)malloc(sizeof (char *) * (size + 1));
	while (env[i])
	{
      if(strncmp(env[i],"OLDPWD",6))
		{
         str[j] = strdup(env[i]);
		   j++;
      }
		i++;
	}
	str[j] = NULL;
	return (str); 
}

void  free_l(char **env)
{
   int i;
   for (i = 0; env[i]; i++)
		free(env[i]);
	free(env[i]);
}
void  print_list_env(t_env *tmp)
{
 if (tmp)
 {
   while (tmp)
    {
        if (tmp->sep)
        {
            ft_putstr_fd(tmp->key, 1);
            ft_putchar_fd(tmp->sep, 1);
            if (tmp->val == NULL)
                ft_putchar_fd('\n', 1);
            else
            {
                ft_putstr_fd(tmp->val, 1);
                  ft_putchar_fd('\n', 1);
            }
        }
        tmp = tmp->next;
    }
 }
}

int   ft_is_space(char *line)
{
   int i;

   i = 0;
   while (line[i])
   {
      if (line[i] != ' ')
         return 0;
      i++;
   }
   return 1;
}
void  free_all(t_lexer *lex, t_token *tok, char *line, t_parse *cmd)
{
   // (void)tok;
   // (void)line;
   if (lex)
   {
      while (lex)
      {
         free(lex);
         advance_lexer(lex);
      }
   }
   if (tok)
   {
      while (tok)
      {
         free(tok);
         tok = tok->next;
      }
   }
   free(line);
   if (cmd)
   {
      while (cmd)
      {
         free(cmd);
         cmd = cmd->next;
      }
   }
}

int main(int ac, char *av[], char **env)
{
   // char	*line;
   t_parse *commands;
   t_lexer	*test;
   t_token *test1;
   (void)ac;
   (void)av;
   init_env(env);
      // system("leaks minishell");
      // exit(0);
   while (1)
   {
      c_signal();
      g_vars.g_err = 0;
      g_vars.line = readline("MESSI-1.0$ ");
      if (!g_vars.line)
      {
         printf("exit\n");
         exit(g_vars.exit_status);
      }
      if (g_vars.line[0] == '\0' ||  ft_is_space(g_vars.line))
      {
         free(g_vars.line);
         continue;
      }
      commands = init_command();
      test = malloc(sizeof(t_lexer));
      test = ft_init_lexer(g_vars.line, g_vars.line[0]);
      test1 = send_lexer_to_tokenize(test);
      add_history(g_vars.line);
      create_commands(test1, &commands);
      if (!g_vars.g_err)
      {
         exec_pipeline(commands, &g_vars.my_env);
      }
   }

}
