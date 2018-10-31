/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expand.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjacques <mjacques@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/30 23:17:33 by mjacques          #+#    #+#             */
/*   Updated: 2018/10/30 23:21:51 by mjacques         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

static char	*ft_expand(char *str, char *name, int pos)
{
	int		envar;
	int		len;
	char	*tmp;

	envar = ft_envar(name);
	len = ft_strlen(name) + 1;
	if (envar == -1)
		tmp = ft_strjoin("", &str[pos]);
	else
		tmp = ft_strjoin(&g_envp[envar][len], &str[pos]);
	return (tmp);
}

static char	*ft_check_tilde(char *cmd, char *tilde)
{
	int		pos;

	pos = tilde - cmd + 1;
	if (!cmd[pos])
		cmd = free_str(cmd, ft_expand(cmd, "HOME", pos));
	else if (cmd[pos] && (cmd[pos] == '/'))
		cmd = free_str(cmd, ft_expand(cmd, "HOME", pos));
	return (cmd);
}

static char	*ft_variable_len(char *cmd, int *pos)
{
	int		len;
	char	*name;

	len = 0;
	while (cmd[*pos + len] && cmd[*pos + len] > 47)
		len++;
	name = ft_strmcpy(&cmd[*pos], len);
	*pos = len;
	return (name);
}

static char	*ft_check_dollar(char *cmd, char *dollar)
{
	int		pos;
	char	*name;
	char	*expand;

	pos = dollar - cmd;
	dollar = ft_strsub(cmd, 0, pos);
	pos += 1;
	name = ft_variable_len(cmd, &pos);
	expand = ft_expand(cmd, name, pos + 1);
	ft_strdel(&name);
	cmd = free_str(cmd, ft_strjoin(dollar, expand));
	ft_strdel(&dollar);
	ft_strdel(&expand);
	return (cmd);
}

char		**ft_check_expand(char **cmd)
{
	int		i;
	char	*position;

	i = 0;
	while (cmd[++i])
	{
		if ((position = ft_strchr(cmd[i], '$')))
			cmd[i] = ft_check_dollar(cmd[i], position);
		if ((position = ft_strchr(cmd[i], '~')))
			cmd[i] = ft_check_tilde(cmd[i], position);
	}
	return (cmd);
}
