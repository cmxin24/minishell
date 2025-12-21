/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_5.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xin <xin@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 22:45:47 by xin               #+#    #+#             */
/*   Updated: 2025/12/21 23:00:24 by xin              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*ft_strjoin_free(char *s1, char *s2)
{
	char	*res;

	if (!s1 && !s2)
		return (NULL);
	if (!s1)
		return (s2);
	if (!s2)
		return (s1);
	res = ft_strjoin(s1, s2);
	free(s1);
	free(s2);
	return (res);
}

int	get_var_len(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '?')
		return (1);
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;
	return (i);
}

static void	expand_tilde(t_expand_ctx *ctx)
{
	char	*home;

	if (ctx->str[0] == '~'
		&& (ctx->str[1] == '\0' || ctx->str[1] == '/'))
	{
		home = ft_get_env_value(*ctx->env, "HOME");
		if (home)
		{
			free(ctx->result);
			ctx->result = ft_strdup(home);
			ctx->i = 1;
			ctx->start = 1;
		}
	}
}

char	*expand_token_str(char *str, t_env **env)
{
	t_expand_ctx	ctx;
	char			*tail;

	ctx.str = str;
	ctx.env = env;
	ctx.result = ft_strdup("");
	ctx.i = 0;
	ctx.start = 0;
	ctx.in_single = 0;
	ctx.in_double = 0;
	expand_tilde(&ctx);
	while (ctx.str[ctx.i])
		process_expand_char(&ctx);
	tail = ft_substr(ctx.str, ctx.start, ctx.i - ctx.start);
	ctx.result = ft_strjoin_free(ctx.result, tail);
	return (ctx.result);
}
