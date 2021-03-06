/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fahd <fahd@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/15 18:20:30 by fahd              #+#    #+#             */
/*   Updated: 2022/07/16 17:56:00 by fahd             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <stdlib.h>
# include <ctype.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <signal.h>
# include <dirent.h>
# include <errno.h>
# include <string.h>
# include <stdio.h>
# include <stddef.h>
# include <limits.h>
# include <readline/readline.h>
# include <readline/history.h>



typedef struct s_lexer
{
	char *str; //aka line
	char    c; // every char in line
	int     i; // index of line
}			t_lexer;

typedef struct s_token
{
	char *val;
	int flag;
	struct s_token *next;
	enum
	{
		WORD,
		DQUOTE,
		SQUOTE,
		BACKSLASH,
		PIPE,
		GREATANDGREAT,
		LESSANDLESS,
		GREAT,
		LESS,
		DOLLAR,
		END,
	} type;
}               t_token;

typedef struct s_redir
{
	int	fdout;
	int     fdin;
	char *file;
	int     type;
	struct s_redir *next;

}               t_redir;

typedef struct s_parse
{
	int	 fdin;
	int	 fdout;
	char *cmd;
	char **argv;
	t_redir *redir;
	struct s_parse *next;
}            t_parse;

typedef struct s_env
{
	char	*key;
	char	*val;
	char	sep;
	struct s_env *next;
}  		t_env; 

typedef struct s_minishell
{
	char	*line;
	int	exit_status;
	int		g_err;
	pid_t	pid;
	int	exited;
	t_env	*my_env;
}	t_minishell;

t_minishell g_vars;

t_lexer	*ft_init_lexer(char *str, char c);
t_token	*send_lexer_to_tokenize(t_lexer *lexer);
t_token *tokenize_WORD(t_lexer *lexer);
t_token *tokenize_DQUOTE(t_lexer *lexer);
t_token *tokenize_SQUOTE(t_lexer *lexer);
t_token *tokenize_GREAT(t_lexer *lexer);
t_token *tokenize_LESS(t_lexer *lexer);
t_token *tokenize_PIPE(t_lexer *lexer);
t_token *tokenize_DOllAR(t_lexer *lexer);
t_token *tokenize_GREATANDGREAT(t_lexer *lexer);
t_token *tokenize_BACKSLASH(t_lexer *lexer);
t_token *tokenize_LESSANDLESS(t_lexer *lexer);
t_token *init_token(char *val, int type);
t_lexer	*advance_lexer(t_lexer *lexer);
t_token *lst_add_back(t_token *lst, t_token *new);

char *ft_strsub(t_lexer *lexer, size_t len);
int	ft_strnstr(const char *str, const char *to_find, size_t len);
char	*ft_strcharjoin(char *s1, char c);
char	*ft_substr(char *s, unsigned int start, size_t len);
void	ft_putnbr_fd(int n, int fd);
int	    ft_isspace(int c);
int	    ft_isalpha(int c);
int str_digit(char *str);
int	str_is_alnum(char *str);
int	    ft_isdigit(int c);
int	    ft_isalnum(int c);
int	    ft_strcmp(char *s1, char *s2);
char	*ft_strdup(char *s1);
int	    ft_strlen(char *s);
char	*ft_strjoin(char *s1, char *s2,int fr);
size_t	ft_strlcpy(char *dest, const char *src, size_t dstsize);
void	ft_putchar_fd(char c, int fd);
void	ft_putstr_fd(char *s, int fd);
char	*ft_strchr(const char *str, int c);
int	ft_int_strchr(const char *s, int c);
int	token_index(char *str);
char *jme3arg(t_token **b, int exec);
int	lst_size(t_token *b);
void    *realloc_array(char **arg, char *str);
void  print_list_env(t_env *tmp);
t_parse *init_command(void);
void print_l(t_parse *lst);
void create_commands(t_token *token, t_parse **command);
char* expand_dollar(char *dq_content, int exec);
void print_list(t_token *lst);
int		exec_builtins(t_parse *commands, t_env **my_env);
void 	exec_pipeline(t_parse *commands, t_env **env);
int 	array_size(char **str);
char	**ft_split(char const *s, char c);
char	*ft_strjoin1(char *s1, char *s2 ,int c);
int		pwd(void);
void  	free_l(char **env);
int		env(void);
void	errors(int exitt);
void	init_env(char **env);
char	*my_getenv(t_env  **env, char *key);
void	update_export(t_env **env, char *key, char sep, char *val);
char	*get_path(char *cmd, char **env);
void	execute(t_parse *command, t_env **env);
t_env	*lst_new(char *key, char sep, char *val);
int		check_exp_arg(char *to_check);
int		builtins_cases(t_parse *cmd);
int		cd(t_parse *head, t_env *my_env);
void	lst_add_backenv(t_env **lst, t_env *new);
void	perr_exp(char *str);
int		export(t_parse *head);
void	check_numb(char *str);
int 	my_exit(t_parse *cmd);
int		echo(t_parse *cmd);
int		unset(t_parse *cmd);
int	    check_env_string(char *str);
void	wrong_cmd(char *cmd);
void    c_signal();
void    open_redir(t_parse *head, int exec);
int   is_piped(void);
#endif
