
#include "minishell.h"

int	ft_isalpha(int c)
{
	return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
}
void	ft_putnbr_fd(int n, int fd)
{
	if (n == -2147483648)
		write(fd, "-2147483648", 11);
	else if (n >= 0 && n <= 9)
		ft_putchar_fd(n + '0', fd);
	else if (n < 0)
	{
		write (fd, "-", 1);
		ft_putnbr_fd(n *= -1, fd);
	}
	else
	{
		ft_putnbr_fd(n / 10, fd);
		ft_putnbr_fd(n % 10, fd);
	}
}

int	ft_isdigit(int c)
{
	return (c >= '0' && c <= '9');
}

int	ft_isalnum(int c)
{
	return (ft_isalpha(c) || ft_isdigit(c));
}
int	ft_strcmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	while (s1[i] == s2[i] && s1[i] != '\0' && s2[i] != '\0')
		i++;
	return (s1[i] - s2[i]);
}

char	*ft_strdup(char *s1)
{
	char	*dup;
	int		i;

	i = 0;
	while (s1[i])
		i++;
	dup = (char *)malloc(i + 1);
	if (dup == NULL)
		return (NULL);
	i = 0;
	while (s1[i] != '\0')
	{
		dup[i] = s1[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}

int	ft_strlen(char *s)
{
	int	i;

	if (!s)
		return (0);
	i = 0;
	while (s[i])
		i++;
	return (i);
}

void	free_unused(int frr, char *s1, char *s2)
{
	if(frr != -1)
	{
		if(frr == 0 && s1)
			free(s1);
		else if(frr == 1 && s2)
			free(s2);
		else if(frr == 2 && s1 && s2)
		{
			free(s1);
			free(s2);
		}
	}
}

void	join_string2(char *str, char *s2, int j)
{
	int	i;

	i = -1;
	while (s2[++i])
	{
	str[j] = s2[i];
		j++;
	}
	str[j] = '\0';

}

char	*ft_strjoin(char *s1, char *s2,int frr)
{
	char	*str;
	int		i;
	int		j;

	i = 0;
	j = 0;
	if ((!s1 && !s2))
		return (strdup(""));
	if (!s1)
		return (strdup(s2));
	if (!s2)
		return (strdup(s1));
	if ((!s1 && s2[0] == 0 )|| (s1[0] == 0 && !s2))
		return (strdup(""));
	str = (char *)malloc(sizeof(char) * ft_strlen(s1) + ft_strlen(s2) + 2);
	if (str == 0)
		return (0);
	while (s1[i])
	{
		str[j] = s1[i];
		i++;
		j++;
	}
	join_string2(str, s2, j);
	free_unused(frr, s1, s2);
	return (str);
}

char	*ft_strcharjoin(char *s1, char c)
{
	int			i;
	i = 0;
	if (!s1)
		return (0);
	if (!c)
		return (s1);
	while (s1[i])
	{
		i++;
	}
	s1[i++] = c;
	s1[i] = '\0';
	return (s1);
}
	
void	ft_putchar_fd(char c, int fd)
{
	write(fd, &c, 1);
}

void	ft_putstr_fd(char *s, int fd)
{
	if (!s)
		return ;
	while (*s)
	{
		ft_putchar_fd(*s, fd);
		s++;
	}
}

char	*ft_strchr(const char *str, int c)
{
	char	*ptr;
	char	sym;

	ptr = (char *)str;
	sym = (char)c;
	while (*ptr && !(*ptr == sym))
		ptr++;
	if (*ptr == sym)
		return (ptr);
	else
		return (NULL);
}

int	ft_int_strchr(const char *s, int c)
{
	int	i;

	i = 0;
	if (!s)
		return (-1);
	if (s[i] == '\0')
		return (-1);
	while (s[i])
	{
		if (s[i] == (char)c)
			return (i);
		i++;
	}
	if ((char)c == '\0')
		return i;
	return (-1);
}
int	token_index(char *str)
{
	int i = 0;
	int j = 0;
	char *tokens;

	tokens = "$><|'\"";
	while(str[i])
	{
		j = 0;
		while(tokens[j])
		{
			if(str[i] == tokens[j] || str[i] == ' ')
				return(i);
			j++;
		}
		i++;
	}
	return(0);
}
int is_token(char c)
{
	int j;
	char *tokens;

	j = 0;
	tokens = "$><|'\"";
	while(tokens[j])
	{
		if(c == tokens[j] || c == ' ')
			return(1);
		j++;
	}
	return(0);
}
char	*ft_substr(char *s, unsigned int start, size_t len)
{
	char	*str;
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	if (!s)
		return (0);
	if ((unsigned int)ft_strlen(s) < start || len == 0)
		return (ft_strdup(""));
	str = (char *)malloc(sizeof(char) * len + 1);
	if (str == NULL)
		return (NULL);
	while (s[i])
	{
		if (j < len && i >= start)
		{
			str[j] = s[i];
			j++;
		}
		i++;
	}
	str[j] = '\0';
	return (str);
}
void	errors(int exitt)
{
	if(exitt == 2)
	{
		ft_putstr_fd("Minishell : quote m7lola!", 2);
		ft_putchar_fd('\n', 2);
		g_vars.g_err = 1;
		g_vars.exit_status = 2;
	}
	else if(exitt == 258)
	{
		ft_putstr_fd("Minishell : syntax error near unexpected token `newline'", 2);
		ft_putchar_fd('\n', 2);
		g_vars.g_err = 1;
		g_vars.exit_status = 2;
	}
	else if(exitt == 3)
	{
		ft_putstr_fd("Minishell : pipe m7lola!", 2);
		ft_putchar_fd('\n', 2);
		g_vars.g_err = 1;
		g_vars.exit_status = 2;
	}
}