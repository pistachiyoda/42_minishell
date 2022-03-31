#include "../test.h"

char	**expand_envp;

char	*call_expand(std::string buf)
{
	return expand_env_variables_in_buf(create_environ(expand_envp), (char *)buf.c_str());
}

TEST_GROUP(expand_env_G)
{
	void setup() {
		expand_envp = ft_split("MANPATH=/opt/homebrew/share/man::,TERM_PROGRAM=iTerm.app,TERM=xterm-256color,SHELL=/bin/zsh,HOMEBREW_REPOSITORY=/opt/homebrew,TMPDIR=/var/folders/8g/1r9l8hd17hg96b7d9_rbgdyc0000gn/T/,TERM_PROGRAM_VERSION=3.4.15,TERM_SESSION_ID=w0t0p1:63A9B361-68C8-4D21-979C-345682F40051,ZSH=/Users/mai/.oh-my-zsh,USER=mai,COMMAND_MODE=unix2003,SSH_AUTH_SOCK=/private/tmp/com.apple.launchd.PGP3ldMtWY/Listeners,__CF_USER_TEXT_ENCODING=0x0:1:14,PAGER=less,LSCOLORS=Gxfxcxdxbxegedabagacad,PATH=/opt/homebrew/bin:/opt/homebrew/sbin:/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin,__CFBundleIdentifier=com.googlecode.iterm2,PWD=/Users/mai/Dev/42/minishell/tests,LANG=ja_JP.UTF-8,ITERM_PROFILE=Default,XPC_FLAGS=0x0,XPC_SERVICE_NAME=0,SHLVL=5,HOME=/Users/mai,COLORFGBG=7;0,WHITESPACE=,LC_TERMINAL_VERSION=3.4.15,HOMEBREW_PREFIX=/opt/homebrew,ITERM_SESSION_ID=w0t0p1:63A9B361-68C8-4D21-979C-345682F40051,HOGE=hoge.txt,LESS=-R,LOGNAME=mai,INFOPATH=/opt/homebrew/share/info:,HOMEBREW_CELLAR=/opt/homebrew/Cellar,LC_TERMINAL=iTerm2,SQLITE_EXEMPT_PATH_FROM_VNODE_GUARDS=/Users/mai/Library/WebKit/Databases,COLORTERM=truecolor,_=/usr/bin/env", ',');
	}
	void teardown() {
	}
};

TEST(expand_env_G, one_env_only)
{
	char *ret;

	ret = call_expand("$USER");
	STRCMP_EQUAL("mai", ret);
	ret = call_expand("$USER ");
	STRCMP_EQUAL("mai ", ret);
	ret = call_expand(" $USER");
	STRCMP_EQUAL(" mai", ret);
}

TEST(expand_env_G, one_env_with_space)
{
	char *ret;

	ret = call_expand("test  $USER");
	STRCMP_EQUAL("test  mai", ret);
	ret = call_expand("$USER test");
	STRCMP_EQUAL("mai test", ret);
	ret = call_expand("test  $USER  test");
	STRCMP_EQUAL("test  mai  test", ret);
}

TEST(expand_env_G, one_env_without_space)
{
	char *ret;

	ret = call_expand("test$USER");
	STRCMP_EQUAL("testmai", ret);
	ret = call_expand("$USERmai");
	STRCMP_EQUAL("", ret);
}

TEST(expand_env_G, multi_env_without_space)
{
	char *ret;

	ret = call_expand("$SHELL$USER");
	STRCMP_EQUAL("/bin/zshmai", ret);
	ret = call_expand("$SHELL$USER$NOTEXISTENV");
	STRCMP_EQUAL("/bin/zshmai", ret);
	ret = call_expand("$SHELL$NOTEXISTENV$USER");
	STRCMP_EQUAL("/bin/zshmai", ret);
}

TEST(expand_env_G, multi_env_with_space)
{
	char *ret;

	ret = call_expand("$SHELL   $USER");
	STRCMP_EQUAL("/bin/zsh   mai", ret);
	ret = call_expand("$SHELL test$USER test$NOTEXISTENV   ");
	STRCMP_EQUAL("/bin/zsh testmai test   ", ret);
}

// single_quote
TEST(expand_env_G, single_quote_after)
{
	char *ret;

	ret = call_expand("$USER'");
	STRCMP_EQUAL("mai'", ret);
}

TEST(expand_env_G, single_quote_before)
{
	char *ret;

	ret = call_expand("'$USER");
	STRCMP_EQUAL("'mai", ret);
}

TEST(expand_env_G, single_quoted_after_and_before)
{
	char *ret;

	ret = call_expand("'$USER'");
	STRCMP_EQUAL("'mai'", ret);
}

// double_quote
TEST(expand_env_G, double_quote_after)
{
	char *ret;

	ret = call_expand("$USER\"");
	STRCMP_EQUAL("mai\"", ret);
}

TEST(expand_env_G, double_quote_before)
{
	char *ret;

	ret = call_expand("\"$USER");
	STRCMP_EQUAL("\"mai", ret);
}

TEST(expand_env_G, double_quoted_after_and_before)
{
	char *ret;

	ret = call_expand("\"$USER\"");
	STRCMP_EQUAL("\"mai\"", ret);
}

// +
TEST(expand_env_G, plus_after)
{
	char *ret;

	ret = call_expand("$USER+");
	STRCMP_EQUAL("mai+", ret);
}

TEST(expand_env_G, plus_before)
{
	char *ret;

	ret = call_expand("+$USER");
	STRCMP_EQUAL("+mai", ret);
}

TEST(expand_env_G, plus_after_and_before)
{
	char *ret;

	ret = call_expand("+$USER+");
	STRCMP_EQUAL("+mai+", ret);
}

// -
TEST(expand_env_G, minus_after)
{
	char *ret;

	ret = call_expand("$USER-");
	STRCMP_EQUAL("mai-", ret);
}

TEST(expand_env_G, minus_before)
{
	char *ret;

	ret = call_expand("-$USER");
	STRCMP_EQUAL("-mai", ret);
}

TEST(expand_env_G, minus_after_and_before)
{
	char *ret;

	ret = call_expand("-$USER-");
	STRCMP_EQUAL("-mai-", ret);
}

// _
TEST(expand_env_G, underscore_after)
{
	char *ret;

	ret = call_expand("$USER_");
	STRCMP_EQUAL("", ret);
}

TEST(expand_env_G, underscore_before)
{
	char *ret;

	ret = call_expand("_$USER");
	STRCMP_EQUAL("_mai", ret);
}

TEST(expand_env_G, underscore_after_and_before)
{
	char *ret;

	ret = call_expand("_$USER_");
	STRCMP_EQUAL("_", ret);
}

// :
TEST(expand_env_G, colon_after)
{
	char *ret;

	ret = call_expand("$USER:");
	STRCMP_EQUAL("mai:", ret);
}

TEST(expand_env_G, colon_before)
{
	char *ret;

	ret = call_expand(":$USER");
	STRCMP_EQUAL(":mai", ret);
}

TEST(expand_env_G, colon_after_and_before)
{
	char *ret;

	ret = call_expand(":$USER:");
	STRCMP_EQUAL(":mai:", ret);
}

// $
TEST(expand_env_G, dollar)
{
	char *ret;

	ret = call_expand("$");
	STRCMP_EQUAL("$", ret);
}

TEST(expand_env_G, dollar_with_equal)
{
	char *ret;

	ret = call_expand("$=");
	STRCMP_EQUAL("$=", ret);
}

TEST(expand_env_G, dollar_with_question1)
{
	char *ret;

	ret = call_expand("$?a");
	STRCMP_EQUAL("0a", ret);
}

TEST(expand_env_G, dollar_with_question2)
{
	char *ret;

	ret = call_expand("$?\"a\"");
	STRCMP_EQUAL("0\"a\"", ret);
}

TEST(expand_env_G, dollar_with_question3)
{
	char *ret;

	ret = call_expand("$=$a$\"\"");
	STRCMP_EQUAL("$=$\"\"", ret);
}

TEST(expand_env_G, dollar_with_question4)
{
	char *ret;

	ret = call_expand("$=aa$TEST=aa");
	STRCMP_EQUAL("$=aa=aa", ret);
}

TEST(expand_env_G, dollar_with_question5)
{
	char *ret;

	ret = call_expand("$\"=\"");
	STRCMP_EQUAL("$\"=\"", ret);
}

TEST(expand_env_G, dollar_with_question6)
{
	char *ret;

	ret = call_expand("$\"\"");
	STRCMP_EQUAL("$\"\"", ret);
}

TEST(expand_env_G, dollar_with_question7)
{
	char *ret;

	ret = call_expand("$\'\'");
	STRCMP_EQUAL("$\'\'", ret);
}

TEST(expand_env_G, dollar_with_question8)
{
	char *ret;

	ret = call_expand("$\'aa\'");
	STRCMP_EQUAL("$\'aa\'", ret);
}

TEST(expand_env_G, dollar_with_question9)
{
	char *ret;

	ret = call_expand("$\'$TEST\'");
	STRCMP_EQUAL("$\'\'", ret);
}

TEST(expand_env_G, dollar_with_question10)
{
	char *ret;

	ret = call_expand("$\"aa\"");
	STRCMP_EQUAL("$\"aa\"", ret);
}

TEST(expand_env_G, dollar_with_question11)
{
	char *ret;

	ret = call_expand("$\"$TEST\"");
	STRCMP_EQUAL("$\"\"", ret);
}

TEST(expand_env_G, dollar_with_question12)
{
	char *ret;

	ret = call_expand("$?TEST");
	STRCMP_EQUAL("0TEST", ret);
}

TEST(expand_env_G, dollar_with_question13)
{
	char *ret;

	ret = call_expand("$?$TEST");
	STRCMP_EQUAL("0", ret);
}
