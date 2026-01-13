/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nschneid <nschneid@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 21:13:06 by xin               #+#    #+#             */
/*   Updated: 2026/01/13 15:28:29 by nschneid         ###   ########.fr       */
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

/**
 * @brief Types and Structures
 * @note
 * redirect in: <, means input redirection from a file
 * redirect out: >, means output redirection to a file
 * append: >>, means appending output to a file
 * heredoc: << means reading input until a delimiter is found
 */
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

typedef struct s_expand_ctx
{
	char	*str;
	char	*result;
	int		i;
	int		start;
	int		in_single;
	int		in_double;
	t_env	**env;
}	t_expand_ctx;

typedef struct s_strip_ctx
{
	char	*str;
	int		i;
	int		j;
	char	quote;
	char	*dst;
}	t_strip_ctx;

// main stream functions
char	*ft_get_input(void);
int		count_lines(char *line, int i, int in_word);
char	*get_next_line_segment(char *line, int *index);
char	**ft_split_lines_safe(char *line);
char	*read_quote_line(void);
int		has_unclosed_quote(char *str);

// heredoc functions
int		ft_heredoc(t_redir *redir, t_env *env);
char	*expand_heredoc_line(char *line, t_env *env);
void	prnt_heredoc_warn(char *delimiter);

// signal functions
void	ft_disable_echo_ctl(void);
void	ft_init_signals(void);
void	ft_handle_sigint(int sig);
void	ft_handle_heredoc_sigint(int signum);
void	ft_set_heredoc_signals(void);
void	ft_restore_signals(void);

// environment functions
t_env	*ft_init_env(char **envp);
void	ft_free_env_list(t_env *env_list);
void	ft_free_cmd_list(t_cmd *cmd);
void	ft_free_ast(t_ast *ast);
char	*ft_get_env_value(t_env *env_list, char *key);
void	ft_set_env_value(t_env **env_list, char *key, char *value);
void	ft_append_env_value(t_env **env, char *key, char *value);
void	ft_unset_env(t_env **env, char *key);
char	**ft_env_list_to_array(t_env *env_list);
size_t	get_env_count(t_env *head);
char	*env_to_str(t_env *node);

// built-in functions
int		ft_pwd(void);
int		ft_env(t_env *env, char **args);
int		cd_get_target_path(char **args, t_env **env,
			char **path, int *print_path);
int		ft_cd(char **args, t_env **env);
int		ft_exit(char **args, t_env *env, int print);
int		echo_n_flag(char *str);
int		ft_echo(char **args);
int		ft_export(char **args, t_env **env);
int		ft_unset(char **args, t_env **env);
int		is_builtin(char *cmd);
int		exec_builtin(char **args, t_env **env, int is_child);
int		ft_is_valid_identifier(char *str);
void	ft_indentifier_error(char *cmd, char *arg);
int		ft_check_exit(char *str);

// parser functions
t_ast	*ft_parser(t_token **tokens);
int		ft_redirection(t_cmd *cmd, t_token **token);
t_cmd	*parse_subshell(t_token **tokens);
t_cmd	*ft_new_cmd(void);
void	ft_add_cmd(t_cmd **list, t_cmd **current, t_cmd *new_cmd);
int		ft_count_args(t_token *token);
t_ast	*parse_primary(t_token **tokens);
t_cmd	*ft_parse_pipeline(t_token **tokens);

// executor functions
void	ft_executor(t_ast *ast, t_env **env, int is_subshell);
int		ft_builtin_redirect(t_cmd *cmd, int *saved_stdout, int *saved_stdin);
void	ft_restore_io(int saved_stdout, int saved_stdin);
int		ft_process_heredoc(t_ast *ast, t_env *env);
char	*find_command_path(char *cmd, char **envp, int i, char *temp);
void	ft_wait_for_children(pid_t last_pid);
void	child_process(t_cmd *cmd, t_env **envp, int *pipe_fd, int fd_in);
int		open_input_redir(t_redir *redir);
int		open_output_redir(t_redir *redir);

// expander functions
int		ft_len_without_quotes(char *str);
char	*ft_strip_quotes(char *str, int len);
char	*expand_token_str(char *str, t_env **env);
void	ft_expander(t_ast *ast, t_env **env);
void	ft_expand_pipeline(t_cmd *cmd_list, t_env **env);
char	**list_to_array(t_list *list);
void	free_list_nodes(t_list *list);
char	**ft_split_unquoted(char *str);
void	expand_command_args(t_cmd *cmd, t_env **env);
char	*ft_strjoin_free(char *s1, char *s2);
int		get_var_len(char *str);
void	process_expand_char(t_expand_ctx *ctx);
int		strip_should_escape(t_strip_ctx *ctx);
void	strip_handle_escape(t_strip_ctx *ctx);

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

// wildcard functions
char	**expand_wildcard(char *pattern);
int		match(char *pattern, char *string);
int		has_wildcard(char *str);

#endif
