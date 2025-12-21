/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xin <xin@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 21:13:06 by xin               #+#    #+#             */
/*   Updated: 2025/12/21 12:10:51 by xin              ###   ########.fr       */
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
# include <sys/stat.h>
# include <signal.h>
# include <termios.h>
# include <fcntl.h>
# include "../lib/includes/libft.h"
# include "../lib/includes/get_next_line.h" 

extern int	g_signal;

typedef enum e_type_of_token
{
	WORD,
	PIPE,
	REDIRECT_IN,
	REDIRECT_OUT,
	APPEND,
	HEREDOC,
	AND,
	OR,
	L_PAREN,
	R_PAREN,
}	t_type_of_token;

typedef struct s_token
{
	char				*content;
	t_type_of_token		type;
	struct s_token		*next;
}	t_token;

typedef enum e_redir_type
{
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	REDIR_HEREDOC
}	t_redir_type;

typedef struct s_redir
{
	char				*file;
	t_redir_type		type;
	int					heredoc_quoted;
	struct s_redir		*next;
}	t_redir;

typedef enum e_ast_type
{
	AST_PIPELINE,
	AST_AND,
	AST_OR
}	t_ast_type;

struct		s_ast;

typedef struct s_cmd
{
	char				**content;
	t_redir				*redirs;
	struct s_cmd		*next;
	struct s_ast		*subshell;
}	t_cmd;

typedef struct s_ast
{
	t_ast_type			type;
	t_cmd				*pipeline;
	struct s_ast		*left;
	struct s_ast		*right;
}	t_ast;

typedef struct s_env
{
	char				*key;
	char				*value;
	struct s_env		*next;
}	t_env;

// main stream functions
int		has_unclosed_quote(char *str);
char	*ft_get_input(void);

// heredoc functions
int		ft_heredoc(t_redir *redir, t_env *env);
char	*expand_heredoc_line(char *line, t_env *env);

// signal functions
void	ft_disable_echo_ctl(void);
void	ft_init_signals(void);
void	ft_handle_sigint(int sig);

// environment functions
t_env	*ft_init_env(char **envp);
void	ft_free_env_list(t_env *env_list);
void	ft_free_cmd_list(t_cmd *cmd);
void	ft_free_ast(t_ast *ast);
char	*ft_get_env_value(t_env *env_list, char *key);
void	ft_set_env_value(t_env **env_list, char *key, char *value);
void	ft_append_env_value(t_env **env, char *key, char *value);
void	ft_unset_env(t_env **env, char *key);
char	**ft_env_list_to_array(t_env *env_list, int i);

// built-in functions
int		ft_pwd(void);
int		ft_env(t_env *env, char **args);
int		ft_cd(char **args, t_env **env);

// wildcard functions
char	**expand_wildcard(char *pattern);
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
t_ast	*ft_parser(t_token *tokens);
void	ft_redirection(t_cmd *cmd, t_token **token);

// executor functions
void	ft_executor(t_ast *ast, t_env **env);
int		ft_builtin_redirect(t_cmd *cmd, int *saved_stdout, int *saved_stdin);
void	ft_restore_io(int saved_stdout, int saved_stdin);
int		ft_process_heredoc(t_ast *ast, t_env *env);

// expander functions
int		ft_len_without_quotes(char *str);
char	*ft_strip_quotes(char *str, int len);
char	*expand_token_str(char *str, t_env **env);
void	ft_expander(t_ast *ast, t_env **env);
void	ft_expand_pipeline(t_cmd *cmd_list, t_env **env);

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
int		ft_check_token_syntax(t_token *tokens);
int		handle_pipe(char *line, int i, t_token **t);
int		handle_and(char *line, int i, t_token **t);
int		handle_redirect_in(char *line, int i, t_token **t);
int		handle_redirect_out(char *line, int i, t_token **t);
int		handle_paren(char c, int i, t_token **t);
int		handle_operator(char *line, int i, t_token **t);
int		check_lparen(t_token *t, int *parens);
int		check_rparen(t_token *t, int *parens);
int		check_operator(t_token *t);
int		check_redirect(t_token *t);
int		check_word(t_token *t);

#endif
