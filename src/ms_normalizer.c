/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_normalizer.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-bouk <yel-bouk@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 23:57:21 by mmalie            #+#    #+#             */
/*   Updated: 2025/05/02 12:42:11 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

 #include "../inc/minishell.h"
char *ft_add_spaces_around(char *str, char special)
{
	int i = 0;
	int len = ft_strlen(str);
	int extra = 0;

	// Count how many extra spaces we need
	for (i = 0; str[i]; i++)
		if (str[i] == special)
			extra += 2;

	char *new_str = malloc(len + extra + 1);
	if (!new_str) return NULL;

	int j = 0;
	for (i = 0; str[i]; i++)
	{
		if (str[i] == special)
		{
			new_str[j++] = ' ';
			new_str[j++] = special;
			new_str[j++] = ' ';
		}
		else
			new_str[j++] = str[i];
	}
	new_str[j] = '\0';
	return new_str;
}

char *ft_add_spaces_around_str(const char *line, const char *str) {
	int len = strlen(line);
	int op_len = strlen(str);
	int count = 0;
	for (int i = 0; line[i]; i++) {
		if (strncmp(&line[i], str, op_len) == 0)
			count++;
	}

	if (count == 0)
		return strdup(line);

	// Each match will add 2 extra spaces
	char *result = malloc(len + count * 2 + 1);
	if (!result)
		return NULL;

	int i = 0, j = 0;
	while (line[i]) {
		if (strncmp(&line[i], str, op_len) == 0) {
			result[j++] = ' ';
			for (int k = 0; k < op_len; k++)
				result[j++] = str[k];
			result[j++] = ' ';
			i += op_len;
		} else {
			result[j++] = line[i++];
		}
	}
	result[j] = '\0';
	return result;
}

// char	*ft_normalize(char *line)
// {
// 	char	*trimmed_line;
// 	char	*normalized_line;

// 	trimmed_line = NULL;
// 	normalized_line = NULL;
// 	trimmed_line = ft_strtrim(line, " \f\n\r\t\v");
// 	if (trimmed_line == NULL)
// 	{
// 		return (NULL);
// 	}
// 	normalized_line = ft_strcollapse(trimmed_line);
// 	free(trimmed_line);
// 	if (normalized_line == NULL)
// 	{
// 		return (NULL);
// 	}
// 	return (normalized_line);
// }
char	*ft_normalize(char *line)
{
	char	*trimmed_line;
	char	*normalized_line;
	char	*spaced;
	char	*tmp;

	trimmed_line = ft_strtrim(line, " \f\n\r\t\v");
	if (trimmed_line == NULL)
		return (NULL);

	normalized_line = ft_strcollapse(trimmed_line);
	free(trimmed_line);
	if (normalized_line == NULL)
		return (NULL);

	spaced = ft_add_spaces_around(normalized_line, CTRL_CHAR_PIPE);
	free(normalized_line);
	if (!spaced)
		return (NULL);

	// Add spaces around <, >, <<, >>
	tmp = spaced;
	spaced = ft_add_spaces_around_str(tmp, (char[]){CTRL_CHAR_APPEND, '\0'});
	free(tmp);
	tmp = spaced;
	spaced = ft_add_spaces_around_str(tmp, (char[]){CTRL_CHAR_HEREDOC, '\0'});
	free(tmp);
	tmp = spaced;
	spaced = ft_add_spaces_around_str(tmp, (char[]){CTRL_CHAR_REDIR_IN, '\0'});
	free(tmp);
	tmp = spaced;
	spaced = ft_add_spaces_around_str(tmp, (char[]){CTRL_CHAR_REDIR_OUT, '\0'});
	free(tmp);

	return spaced;
}

// If there is more than one consecutive space, they are suppressed.
char	*ft_strcollapse(char *line)
{
	char	*collapsed_line;
	size_t	line_len;
	int		to_collapse;
	int		i;

	collapsed_line = NULL;
	line_len = ft_strlen(line);
	ft_init_two_ints(0, &i, &to_collapse);
	while (line[i] != '\0')
	{
		if (line[i] == '$' && line[i + 1] != ' ')
			ft_replace_char(&line[i], CTRL_CHAR_VAR_TO_INTERPRET);
		else if (line[i] == '|')
			ft_replace_char(&line[i], CTRL_CHAR_PIPE);
		else if (line[i] == '<' && line[i + 1] == '<')
		{
			ft_replace_char(&line[i], CTRL_CHAR_HEREDOC);
			ft_replace_char(&line[i + 1], ' ');
			i++; 
		}
		else if (line[i] == '>' && line[i + 1] == '>')
		{
			ft_replace_char(&line[i], CTRL_CHAR_APPEND);
			ft_replace_char(&line[i + 1], ' ');
			i++;
		}					
		else if(line[i] == '<')
			ft_replace_char(&line[i], CTRL_CHAR_REDIR_IN);
		else if(line[i] == '>')
			ft_replace_char(&line[i], CTRL_CHAR_REDIR_OUT);
		else if (line[i] == '\'' && ft_count_char(&line[i], '\'') > 1)
			handle_quote(line, '\'', &i, &to_collapse);
		else if (line[i] == '\"' && ft_count_char(&line[i], '\"') > 1)
			handle_quote(line, '\"', &i, &to_collapse);
		else if (ft_isspace(line[i]) && ft_isspace(line[i + 1]))
			to_collapse++;
		i++;
	}
	collapsed_line = malloc(sizeof(char) * ((line_len - to_collapse) + 1));
	if (!collapsed_line)
		return (NULL);
	collapsed_line = copy_collapse(collapsed_line, line, line_len);
	return (collapsed_line);
}

void	handle_quote(char *line, char quote_type, int *i, int *to_collapse)
{
	(*i)++;
	(*to_collapse)++;
	if (quote_type == '\'')
	{
		while (line[(*i)] != '\'')
		{
			ft_replace_if_space(&line[(*i)], CTRL_CHAR_SPACE_IN_QUOTE);
			(*i)++;
		}
	}
	else if (quote_type == '\"')
	{
		while (line[(*i)] != '\"')
		{
			if (line[(*i)] == '$' && line[(*i + 1)] != ' ' && line[(*i + 1)] != '\0')
			{
				ft_replace_char(&line[(*i)], CTRL_CHAR_VAR_TO_INTERPRET);
				(*i)++;
			}
			if (line[(*i)] != '\"')
			{
				ft_replace_if_space(&line[(*i)], CTRL_CHAR_SPACE_IN_QUOTE);
				(*i)++;
			}
		}
	}
	(*to_collapse)++;
}

// Both the src and dst are expected to be non-NULL and their size is correct
char	*copy_collapse(char *dst, char *src, size_t src_len)
{
	size_t	i;
	size_t	j;

	if (!src)
		return (NULL);
	ft_init_two_size_t(0, &i, &j);
	while (i < src_len)
	{
		if ((src[i] == '\'' && ft_count_char(&src[i], '\'') > 1)
			|| (src[i] == '\"' && ft_count_char(&src[i], '\"') > 1))
			pass_quotes(dst, src, &i, &j);
		else if (ft_isspace(src[i]) && ft_isspace(src[i + 1]))
			i++;
		else
			dst[j++] = src[i++];
	}
	dst[j] = '\0';
	return (dst);
}

void	pass_quotes(char *dst, char *src, size_t *i, size_t *j)
{
	if (src[(*i)] == '\'' && ft_count_char(&src[(*i)], '\'') > 1)
	{
		(*i)++;
		while (src[(*i)] != '\'')
			dst[(*j)++] = src[(*i)++];
		(*i)++;
	}
	else if (src[(*i)] == '\"' && ft_count_char(&src[(*i)], '\"') > 1)
	{
		(*i)++;
		while (src[(*i)] != '\"')
			dst[(*j)++] = src[(*i)++];
		(*i)++;
	}
}
