/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xin <xin@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 19:44:00 by xin               #+#    #+#             */
/*   Updated: 2025/12/21 23:49:58 by xin              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	update_quote_state(t_expand_ctx *ctx)
{
	if (ctx->str[ctx->i] == '\'' && !ctx->in_double)
		ctx->in_single = !ctx->in_single;
	else if (ctx->str[ctx->i] == '\"' && !ctx->in_single)
		ctx->in_double = !ctx->in_double;
}

static int	should_skip_escape(t_expand_ctx *ctx)
{
	if (ctx->str[ctx->i] != '\\' || ctx->in_single)
		return (0);
	if (!ctx->in_double)
		return (1);
	if (ctx->in_double
		&& (ctx->str[ctx->i + 1] == '$'
			|| ctx->str[ctx->i + 1] == '\"'
			|| ctx->str[ctx->i + 1] == '\\'))
		return (1);
	return (0);
}

static char	*get_var_value(t_expand_ctx *ctx, int *len)
{
	char	*key;
	char	*val;

	*len = get_var_len(&ctx->str[ctx->i]);
	if (*len == 0 && (ctx->str[ctx->i] == '\'' || ctx->str[ctx->i] == '\"'))
		return (ft_strdup(""));
	if (*len == 0)
	{
		*len = 1;
		return (ft_substr(ctx->str, ctx->i - 1, 2));
	}
	if (*len == 1 && ctx->str[ctx->i] == '?')
		return (ft_itoa(g_signal));
	key = ft_substr(ctx->str, ctx->i, *len);
	val = ft_get_env_value(*ctx->env, key);
	free(key);
	if (val)
		return (ft_strdup(val));
	return (ft_strdup(""));
}

static void	handle_dollar(t_expand_ctx *ctx)
{
	char	*prefix;
	char	*var;
	int		len;

	prefix = ft_substr(ctx->str, ctx->start, ctx->i - ctx->start);
	ctx->result = ft_strjoin_free(ctx->result, prefix);
	ctx->i++;
	var = get_var_value(ctx, &len);
	ctx->result = ft_strjoin_free(ctx->result, var);
	ctx->i += len;
	ctx->start = ctx->i;
}

void	process_expand_char(t_expand_ctx *ctx)
{
	if (should_skip_escape(ctx))
	{
		ctx->i += 2;
		return ;
	}
	update_quote_state(ctx);
	if (ctx->str[ctx->i] == '$'
		&& !ctx->in_single
		&& ctx->str[ctx->i + 1]
		&& !(ctx->in_double && ctx->str[ctx->i + 1] == '\"'))
	{
		handle_dollar(ctx);
		return ;
	}
	ctx->i++;
}
