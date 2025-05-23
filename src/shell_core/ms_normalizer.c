/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_normalizer.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-bouk <yel-bouk@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 23:57:21 by mmalie            #+#    #+#             */
/*   Updated: 2025/05/23 16:16:47 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

// Returns a normalized (trimmed, flagged and spaced) line
char	*ft_normalize(char *line)
{
	char	*trimmed;
	char	*flagged;
	char	*spaced;
	char	*tmp;

	trimmed = ft_strtrim(line, " \f\n\r\t\v");
	if (trimmed == NULL)
		return (NULL);
	flagged = ft_strflag(trimmed);
	free(trimmed);
	if (flagged == NULL)
		return (NULL);
	spaced = ft_add_spaces_around(flagged, CC_PIPE);
	free(flagged);
	if (!spaced)
		return (NULL);
	

	tmp = spaced;
	spaced = ft_add_spaces_around_str(tmp, (char[]){CC_APPEND, '\0'});
	free(tmp);
	tmp = spaced;
	spaced = ft_add_spaces_around_str(tmp, (char[]){CC_HEREDOC, '\0'});
	free(tmp);
	tmp = spaced;
	spaced = ft_add_spaces_around_str(tmp, (char[]){CC_REDIR_IN, '\0'});
	free(tmp);
	tmp = spaced;
	spaced = ft_add_spaces_around_str(tmp, (char[]){CC_REDIR_OUT, '\0'});
	free(tmp);
	if (DEBUG == 1)
		printf("[DEBUG FT_NORMALIZER] %s\n", spaced);
	return (spaced);
}

// If there is more than one consecutive space, they are suppressed.
char	*ft_strflag(char *line)
{
	char	*flagged_line;
	size_t	line_len;
	int		i;

	flagged_line = NULL;
	i = 0;
	while (line[i])
	{
		if (line[i] == '$')
			flag_dollar(line, &i);
		else if (ft_is_in_set(line[i], "|<>"))
			flag_pipe_and_redir(line, &i);
		else if (ft_is_in_set(line[i], "\"\'"))
			flag_quote(line, &i);
		i++;
	}
	ft_replace_chars_in_str(line, CC_DOLLAR_UNCLOSED, '$');
	line_len = ft_strlen(line);
	flagged_line = malloc(sizeof(char) * ((line_len + 1)));
	if (!flagged_line)
		return (NULL);
	flagged_line = copy_collapse(flagged_line, line, line_len);
	return (flagged_line);
}

// Both the src and dst are expected to be non-NULL and their size is correct
char	*copy_collapse(char *dst, char *src, size_t src_len)
{
	size_t	i;
	size_t	j;

	if (!src)
		return (NULL);
	ft_init_two_size_t(0, &i, &j);
	if (DEBUG == 1)
		printf("[DEBUG COPY_COLLAPSE] %s\n", src);
	while (i < src_len)
	{
		if ((src[i] == '\'' && ft_count_char(&src[i], '\'') > 1)
			|| (src[i] == CC_VAR_BOUND)
			|| (src[i] == '\"' && ft_strrchr(&src[i], CC_VAR_BOUND) != NULL)
			|| (src[i] == '\"' && ft_count_char(&src[i], '\"') > 1))
			pass_quotes(dst, src, &i, &j);
		else if ((ft_isspace(src[i]) && ft_isspace(src[i + 1]))
			|| (src[i] == CC_LONE_DOLLAR))
			i++;
		else
			dst[j++] = src[i++];
	}
	dst[j] = '\0';
	//printf("[DEB - after cpclapse] %s\n", dst);
	return (dst);
}


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
