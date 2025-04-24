/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_normalizer.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-bouk <yel-bouk@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 23:57:21 by mmalie            #+#    #+#             */
/*   Updated: 2025/04/22 13:33:11 by yel-bouk         ###   ########.fr       */
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

// char	*ft_normalize(char *line)
// {
// 	char	*trimmed_line;
// 	char	*normalized_line;
// 	char	*spaced;

// 	trimmed_line = ft_strtrim(line, " \f\n\r\t\v");
// 	if (trimmed_line == NULL)
// 		return (NULL);

// 	normalized_line = ft_strcollapse(trimmed_line);
// 	free(trimmed_line);
// 	if (normalized_line == NULL)
// 		return (NULL);

// 	spaced = ft_add_spaces_around(normalized_line, CTRL_CHAR_PIPE);
// 	free(normalized_line);
// 	if (!spaced)
// 		return (NULL);
// 	char *tmp = spaced;
// 	spaced = ft_add_spaces_around(tmp, '<');
// 	free(tmp);
// 	tmp = spaced;
// 	spaced = ft_add_spaces_around(tmp, '>');
// 	free(tmp);
// 	return spaced;
// }
char *ft_add_spaces_around_str(char *str, const char *seq)
{
	int i = 0, j = 0;
	int seq_len = ft_strlen(seq);
	int len = ft_strlen(str);
	int extra = 0;

	while (str[i])
	{
		if (strncmp(&str[i], seq, seq_len) == 0)
			extra += 2;
		i++;
	}

	char *new_str = malloc(len + extra + 1);
	if (!new_str) return NULL;

	i = 0;
	while (str[i])
	{
		if (strncmp(&str[i], seq, seq_len) == 0)
		{
			new_str[j++] = ' ';
			for (int k = 0; k < seq_len; k++)
				new_str[j++] = seq[k];
			new_str[j++] = ' ';
			i += seq_len;
		}
		else
		{
			new_str[j++] = str[i++];
		}
	}
	new_str[j] = '\0';
	return new_str;
}
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
	spaced = ft_add_spaces_around(tmp, '<');
	free(tmp);
	tmp = spaced;
	spaced = ft_add_spaces_around(spaced, '>');
	free(tmp);
	tmp = spaced;
	spaced = ft_add_spaces_around_str(spaced, "<<");
	free(tmp);
	tmp = spaced;
	spaced = ft_add_spaces_around_str(spaced, ">>");
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
		if (line[i] == '$')
			ft_replace_char(&line[i], CTRL_CHAR_VAR_TO_INTERPRET);
		else if (line[i] == '|')
			ft_replace_char(&line[i], CTRL_CHAR_PIPE);
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

int	ft_count_char(char *str, char c)
{
	size_t	str_len;
	size_t	i;
	int		count;

	str_len = ft_strlen(str);
	i = 0;
	count = 0;
	while (i < str_len)
	{
		if (str[i] == c)
			count++;
		i++;
	}
//	printf("[DEBUG] ~%c~ found %u times in ~%s~\n", c, count, str);
	return (count);
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
