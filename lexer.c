/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fstitou <fstitou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/17 05:17:20 by fahd              #+#    #+#             */
/*   Updated: 2022/07/15 23:52:49 by fstitou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *ft_strsub(t_lexer *lexer, size_t len)
{
	char *sub;
	unsigned int i;

	i = 0;
	if (!lexer->str)
		return (NULL);
	if (!(sub = (char *)malloc(sizeof(char) * (len + 1))))
		return (NULL);
	while (i < len)
	{
		sub[i] = lexer->c;
		i++;
		advance_lexer(lexer);
	}
	sub[i] = '\0';
	return (sub);
}

t_lexer	*ft_init_lexer(char *str, char c)
{
    t_lexer			*lexer;

    lexer = (t_lexer *)malloc(sizeof(t_lexer));
	if (!lexer)
        return (NULL);
	lexer->str = str;
	lexer->c = c;
	lexer->i = 0;
    return (lexer);
}
t_lexer *advance_lexer(t_lexer *lexer)
{
	lexer->i++;
	lexer->c = lexer->str[lexer->i];
	return (lexer);
}

t_token	*send_lexer_to_tokenize(t_lexer *lexer)
{
	t_token			*token;
	t_token			*tmp;
	char			*val;
	int				type;
	int size = 0;
	
	tmp = NULL;
	token = NULL;
	while (lexer->c)
	{
		if (lexer->c == ' ' || lexer->c == '\t')
			advance_lexer(lexer);
		else if (lexer->c == '\'')
		{
			advance_lexer(lexer);
			type = SQUOTE;
			if	(ft_int_strchr(&(lexer->str[lexer->i]), '\'') >= 0)
				size = ft_int_strchr(&(lexer->str[lexer->i]), '\'');
			else
				errors(2);
			val = ft_strsub(lexer, size);
			advance_lexer(lexer);
			token = init_token(val, type);
			if(lexer->c != ' ' && lexer->c != '\0')
				token->flag = 1;
			tmp = lst_add_back(tmp, token);
		}
		else if (lexer->c == 34)
		{
			type = DQUOTE;
			advance_lexer(lexer);
			if	(ft_int_strchr(&(lexer->str[lexer->i]), 34) >= 0)
				size = ft_int_strchr(&(lexer->str[lexer->i]), 34);
			else
				errors(2);
			val = ft_strsub(lexer, size);
			advance_lexer(lexer);
			token = init_token(val, type);
			if(lexer->c != ' ' && lexer->c != '\0')
				token->flag = 1;
			tmp = lst_add_back(tmp, token);
		}
		else if (lexer->c == '|')
		{
			type = PIPE;
			val = ft_strsub(lexer, 1);
			token = init_token(val, type);
			tmp = lst_add_back(tmp, token);
		}
		else if (lexer->c == '<')
		{
			if (lexer->str[lexer->i + 1] == '<')
			{
				type = LESSANDLESS;
				val = ft_strsub(lexer, 2);
				token = init_token(val, type);
				tmp = lst_add_back(tmp, token);
				}
			else
			{
				type = LESS;
				val = ft_strsub(lexer, 1);
				token = init_token(val, type);
				tmp = lst_add_back(tmp, token);
				}
		}
		else if (lexer->c == '>')
		{
			if (lexer->str[lexer->i+1] == '>')
			{
				type = GREATANDGREAT;
				val = ft_strsub(lexer, 2);
				token = init_token(val, type);
				tmp = lst_add_back(tmp, token);
				}
			else
			{
				type = GREAT;
				val = ft_strsub(lexer, 1);
				token = init_token(val, type);
				tmp = lst_add_back(tmp, token);
				}
		}
		else if (lexer->c == '$')
		{
			type = DOLLAR;
			val = ft_strsub(lexer, 1);
			if(lexer->c == '$')
				val = ft_strjoin(val, ft_strsub(lexer, 1),2);
			else if(lexer->c == ' ')
				val = ft_strjoin(val, ft_strsub(lexer, 1),2);
			else if(lexer->c == '?')
				val = ft_strjoin(val, ft_strsub(lexer, 1),2);
			token = init_token(val, type);
			if(val[1] != ' ' && lexer->c != ' ' && lexer->c != '\0')
				token->flag = 1;
			tmp = lst_add_back(tmp, token);
		}
		else
		{
			type = WORD;
			if(token_index(&(lexer->str[lexer->i])))
				size = token_index(&(lexer->str[lexer->i]));
			else if	(ft_int_strchr(&(lexer->str[lexer->i]), ' ') > 0)
				size = ft_int_strchr(&(lexer->str[lexer->i]), ' ');
			else
				size = ft_int_strchr(&(lexer->str[lexer->i]), '\0');
			val = ft_strsub(lexer, size);
			token = init_token(val, type);
			if(lexer->c != ' ' && lexer->c != '\0')
				token->flag = 1;
			tmp = lst_add_back(tmp, token);
		}
	}
	token = init_token("", END);
	tmp = lst_add_back(tmp, token);
	return (tmp);		
}

char	*expand_dollar(char *dq_content, int exec)
{
   t_token			*token;
	t_token			*tmp;
	char			*val;
	int				type;
   t_lexer *lexer;
	int size = 0;
   char *result;
	
	tmp = NULL;
	token = NULL;

   result = strdup("");
   lexer = ft_init_lexer(dq_content, dq_content[0]);
   while (lexer->c)
   {
      if (lexer->c == '$')
		{
			type = DOLLAR;
			val = ft_strsub(lexer, 1);
			if(lexer->c == '$')
				val = ft_strjoin(val, ft_strsub(lexer, 1),2);
			else if(lexer->c == ' ')
				val = ft_strjoin(val, " ",0);
			else if(lexer->c == '?')
				val = ft_strjoin(val, ft_strsub(lexer, 1),2);
			token = init_token(val, type);
			if(lexer->c != '\0')
				token->flag = 1;
			tmp = lst_add_back(tmp, token);
		}
		else
		{
			type = WORD;
			if(token_index(&(lexer->str[lexer->i])))
				size = token_index(&(lexer->str[lexer->i]));
			else if	(ft_int_strchr(&(lexer->str[lexer->i]), '$') > 0)
				size = ft_int_strchr(&(lexer->str[lexer->i]), '$');
			else
				size = ft_int_strchr(&(lexer->str[lexer->i]), '\0');
			val = ft_strsub(lexer, size);
			token = init_token(val, type);
			token->flag = 1;
			tmp = lst_add_back(tmp, token);
		}
   }
    token = init_token("", END);
    token->flag = 0;
	tmp = lst_add_back(tmp, token);
	if(exec)
		result = jme3arg(&tmp, 1);
	else
		result = jme3arg(&tmp, 0);
   	return result;
}