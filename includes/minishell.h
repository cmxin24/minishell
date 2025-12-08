/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xin <xin@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 21:13:06 by xin               #+#    #+#             */
/*   Updated: 2025/12/08 23:27:33 by xin              ###   ########.fr       */
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
# include <signal.h>
# include <termios.h>
# include <fcntl.h>
# include "../lib/includes/libft.h"

extern int	g_signal;

typedef enum e_type_of_token
{
	WORD,
	PIPE,
	REDIRECT_IN,
	REDIRECT_OUT,
	APPEND,
	HEREDOC,
	TOKEN_EOF
}	t_type_of_token;

typedef struct	s_token
{
	char			*content;
	t_type_of_token	type;
	struct s_token	*next;
}	t_token;

typedef struct	s_cmd
{
	char			**content;
	char			*redirect_in;
	char			*redirect_out;
	int				is_append;
	int				is_heredoc;
	int				heredoc_quoted;
	struct s_cmd	*next;
}	t_cmd;

typedef struct	s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

// heredoc functions
int		ft_heredoc(t_cmd *cmd, char *delimiter, t_env *env, int quotes);
char	*expand_heredoc_line(char *line, t_env *env);

// signal functions
void	ft_disable_echo_ctl(void);
void	ft_init_signals(void);
void	ft_handle_sigint(int sig);

// environment functions
t_env	*ft_init_env(char **envp);
void	ft_free_env_list(t_env *env_list);
char	*ft_get_env_value(t_env *env_list, char *key);
void	ft_set_env_value(t_env **env_list, char *key, char *value);
void	ft_unset_env(t_env **env, char *key);
char	**ft_env_list_to_array(t_env *env_list);

// built-in functions
int		ft_pwd(void);
int		ft_env(t_env *env);
int		ft_cd(char **args, t_env **env);
int		ft_exit(char **args, t_env *env);
int		echo_n_flag(char *str);
int		ft_echo(char **args);
int		ft_export(char **args, t_env **env);
int		ft_unset(char **args, t_env **env);
int		is_builtin(char *cmd);
int		exec_builtin(char **args, t_env **env);
int		ft_is_valid_identifier(char *str);
void	ft_indentifier_error(char *cmd, char *arg);
int		ft_check_exit(char *str);

// parser functions
t_cmd	*ft_parser(t_token *tokens);
void	ft_free_cmd_list(t_cmd *cmd);
void	ft_redirection(t_cmd *cmd, t_token **token);

// executor functions
void	ft_executor(t_cmd *cmd_list, t_env **env);
int		ft_builtin_redirect(t_cmd *cmd, int *saved_stdout);
void	ft_restore_stdout(int saved_stdout);
int		ft_process_heredoc(t_cmd *cmd, t_env *env);

// expander functions
int		ft_len_without_quotes(char *str);
char	*ft_strip_quotes(char *str, int len);
char	*expand_token_str(char *str, t_env **env);
void	ft_expander(t_cmd *cmd_list, t_env **env);

// tools functions
void	ft_free_array(char **array);

// token functions
t_token	*create_token(char *content, t_type_of_token type);
void	add_token(t_token **token_list, t_token *new_token);
void	ft_free_tokens(t_token **token_list);

// lexer functions
int		syntax_error(char *token);
int		ft_is_separator(char c);
t_token	*ft_lexer(char *line);

#endif
