#include "minishell.h"

char	*ft_strjoin2(char const *s1, char const *s2)
{
	size_t	len;
	char	*str;
	char	*ret;

	len = ft_strlen((char *)s1) + ft_strlen((char *)s2);
	str = (char *)malloc(sizeof(char) * (len + 2));
	if (str == NULL)
		return (NULL);
	ret = str;
	while (*s1 != '\0')
		*str++ = *s1++;
	while (*s2 != '\0')
		*str++ = *s2++;
	*str++ = '\n';
	*str = '\0';
	return (ret);
}
