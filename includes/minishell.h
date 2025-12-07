/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xin <xin@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 21:13:06 by xin               #+#    #+#             */
/*   Updated: 2025/12/07 15:43:47 by xin              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <errno.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/wait.h>
# include "../lib/includes/libft.h"

typedef enum e_type_of_token
{
	WORD, // normal word
	PIPE, // | the stdout of the left cmd is connected to stdin of the right cmd
	REDIRECT_IN, // < use file as stdin
	REDIRECT_OUT, // > use file as stdout, 
	APPEND, // >> append to file
	HEREDOC, // << here document, use muti-line input as stdin
	TOKEN_EOF // end of input
}	t_type_of_token;

typedef struct s_token
{
	char				*content;
	t_type_of_token		type;
	struct s_token		*next;
}	t_token;

typedef struct s_cmd
{
	char			**content;
	struct s_cmd	*next;
}	t_cmd;

typedef struct s_env
{
	char	*key;
	char	*value;
	struct s_env	*next;
}	t_env;

// environment functions
t_env	*ft_init_env(char **envp);
void	ft_free_env_list(t_env *env_list);
char	*ft_get_env_value(t_env *env_list, char *key);
void	ft_set_env_value(t_env **env_list, char *key, char *value);
void	ft_unset_env(t_env **env, char *key);
char	**ft_env_list_to_array(t_env *env_list);

// built-in functions
int		ft_pwd(void);
int		ft_env(t_env *envp);
int		ft_cd(char **args, t_env **envp);
int		ft_exit(char **args, t_env *env);
int		is_builtin(char *cmd);
int		exec_builtin(char **args, t_env **envp);

// parser functions
t_cmd	*ft_parser(t_token *tokens);
void	ft_free_cmd_list(t_cmd *cmd);

// executor functions
void	ft_executor(t_cmd *cmd_list, t_env **envp);

// expander functions
char	*expand_token_str(char *str, t_env **env);
void	ft_expander(t_cmd *cmd_list, t_env **envp);

// tools functions
void	ft_free_array(char **array);

// token functions
t_token	*create_token(char *content, t_type_of_token type);
void	add_token(t_token **token_list, t_token *new_token);
void	ft_free_tokens(t_token **token_list);

// lexer functions
int		ft_is_separator(char c);
t_token	*ft_lexer(char *line);

#endif
