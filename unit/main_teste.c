#include "../libft/libft.h"
#include "../includes/minishell.h"
#include "unity.h"

t_env	*init_env;

static int run_cmd(char **cmd){
	t_dados test1 = {
		.comando = cmd,
		.redirect = NULL,
		.nbr_redirections = 0,
		.next = NULL
	};
	return ft_one_cmd(&test1, &init_env);
}

static void assert_n_files_and_clean(char **actuals, char **expecteds)
{
	int return_code;
	char *join1 = ft_strjoin(*actuals, " ");
	char *join2 = ft_strjoin(join1, *expecteds);
	char *join3 = ft_strjoin("diff ", join2);
	
	while (*actuals)
	{
		return_code = run_cmd((char *[]){"bash", "-c", join3, NULL});
		TEST_ASSERT_EQUAL(0, return_code);
		return_code = run_cmd((char *[]){"bash", "-c", join3, NULL});
		TEST_ASSERT_EQUAL(0, return_code);
		actuals++;
		expecteds++;
	}
	free(join1);
	free(join2);
	free(join3);
}

static void assert_files_and_clean(void){
	assert_n_files_and_clean((char *[]){"actual.txt", NULL}, (char *[]){"expected.txt", NULL});
}

static void	tests_ft_out_redirect_pwd(void)
{
	t_dados actual = {
		.comando = (char *[]){"pwd", NULL},
		.redirect = (t_redirect[]){(t_redirect) {.filename="actual.txt", .redirect_type = 0}},
		.nbr_redirections = 1,
		.next = NULL
	};

	char** expected = (char *[]){"bash", "-c", "pwd > expected.txt", NULL};

	TEST_ASSERT_EQUAL(0, ft_one_cmd(&actual, &init_env));

	TEST_ASSERT_EQUAL(0, run_cmd(expected));

	assert_files_and_clean();
}

static void test_creat(void)
{
		t_dados actual = {
		.comando = (char *[]){"pwd", NULL},
		.redirect = (t_redirect[]){(t_redirect) {.filename="actual.txt", .redirect_type = 0}},
		.nbr_redirections = 1,
		.next = NULL
	};

	char** expected = (char *[]){"bash", "-c", "pwd > expected.txt", NULL};

	TEST_ASSERT_EQUAL(0, ft_one_cmd(&actual, &init_env));

	TEST_ASSERT_EQUAL(0, run_cmd(expected));
}

static void	tests_ft_append_redirect_ls(void)
{
	test_creat();

		t_dados actual = {
		.comando = (char *[]){"ls", NULL},
		.redirect = (t_redirect[]){(t_redirect) {.filename="actual.txt", .redirect_type = 3}},
		.nbr_redirections = 1,
		.next = NULL
	};

	char** expected = (char *[]){"bash", "-c", "ls >> expected.txt", NULL};

	TEST_ASSERT_EQUAL(0, ft_one_cmd(&actual, &init_env));

	TEST_ASSERT_EQUAL(0, run_cmd(expected));

	assert_files_and_clean();
}

static void	tests_ft_input_redirect(void)
{
	test_creat();

		t_dados actual = {
		.comando = (char *[]){"wc", NULL},
		.redirect = (t_redirect[]){(t_redirect) {.filename="actual.txt", .redirect_type = 1}},
		.nbr_redirections = 1,
		.next = NULL
	};
	
	char** expected = (char *[]){"bash", "-c", "wc < expected.txt", NULL};

	TEST_ASSERT_EQUAL(0, ft_one_cmd(&actual, &init_env));

	TEST_ASSERT_EQUAL(0, run_cmd(expected));

	assert_files_and_clean();
}

static void tests_return_code_error(void){

	TEST_ASSERT_EQUAL(1, run_cmd((char *[]){"cd", "a", NULL}));
	TEST_ASSERT_EQUAL(1, run_cmd((char *[]){"bash", "-c", "wc a", NULL}));

}

static void	tests_ft_more_redirect(void)
{
		t_dados actual = {
		.comando = (char *[]){"pwd", NULL},
		.redirect = (t_redirect[]){(t_redirect) {.filename="actual2.txt", .redirect_type = 0}, (t_redirect) {.filename="actual.txt", .redirect_type = 3}},
		.nbr_redirections = 2,
		.next = NULL
	};
	
	char** expected = (char *[]){"bash", "-c", "pwd > expected2.txt >> expected.txt", NULL};

	TEST_ASSERT_EQUAL(0, ft_one_cmd(&actual, &init_env));

	TEST_ASSERT_EQUAL(0, run_cmd(expected));

	assert_n_files_and_clean((char *[]){"actual.txt", "actual2.txt", NULL}, (char *[]){"expected.txt", "expected2.txt", NULL});
}

static void	assert_lista(t_dados *expected, t_dados *parsing)
{
		int		i;
	t_dados	*aux = expected;
	t_dados	*aux_p = parsing;

	i = 0;
	while (aux)
	{
		while (aux->comando[i])
		{
			TEST_ASSERT_NOT_NULL(aux_p);
			TEST_ASSERT_NOT_NULL(aux_p->comando[i]);
			printf("expec: %s\n", aux->comando[i]);
			printf("parsing: %s\n", aux_p->comando[i]);
			TEST_ASSERT_EQUAL_STRING(aux->comando[i], aux_p->comando[i]);
			i++;
		}
		i = 0;
		aux = aux->next;
		aux_p = aux_p->next;
	}
	free_list(&parsing);
}

static void	tests_ft_parsing(void)
{
		t_dados expected = {
		.comando = (char *[]){"wc", NULL},
		.redirect = NULL,
		.nbr_redirections = 0,
		.next = NULL
	};
	
		t_dados expected2 = {
		.comando = (char *[]){"pwd", NULL},
		.redirect = NULL,
		.nbr_redirections = 0,
		.next = &expected
	};

	char	*input = {"pwd | wc"};
	t_dados	*actual = parsing(input);

	assert_lista(&expected2, actual);
}

static void	tests_ft_validate_open_quotes(void)
{
	int		expected = 1;
	char	*input = {"echo \"a"};
	int		actual = validate_input(input);

	TEST_ASSERT_EQUAL_INT64_MESSAGE(expected, actual, "input com aspas abertas deve retornar erro");
}

static void	tests_ft_validate_open_quotes_pipe(void)
{
	int		expected = 1;
	char	*input = {"ls | wc \"a"};
	int		actual = validate_input(input);

	TEST_ASSERT_EQUAL_INT64_MESSAGE(expected, actual, "input com aspas abertas deve retornar erro");
}

static void	tests_ft_validate_open_quotes_pipe2(void)
{
	int		expected = 1;
	char	*input = {"ls | wc \'a"};
	int		actual = validate_input(input);

	TEST_ASSERT_EQUAL_INT64_MESSAGE(expected, actual, "input com aspas abertas deve retornar erro");
}

static void	tests_ft_validate_sintaxe(void)
{
	int		expected = 1;
	char	*input = {" ls >< a. txt"};
	int		actual = validate_input(input);

	TEST_ASSERT_EQUAL_INT64_MESSAGE(expected, actual, "input com erro de sintaxe red sem complemento ><");
}

static void	tests_ft_validate_sintaxe_start_pipe(void)
{
	int		expected = 1;
	char	*input = {"| ls"};
	int		actual = validate_input(input);

	TEST_ASSERT_EQUAL_INT64_MESSAGE(expected, actual, "input com erro de sintaxe, nao pode comecar com |");
}

static void	tests_ft_validate_sintaxe_empty_pipe(void)
{
	int		expected = 1;
	char	*input = {"ls |"};
	int		actual = validate_input(input);

	TEST_ASSERT_EQUAL_INT64_MESSAGE(expected, actual, "input com erro de sintaxe, nao conter | vazio");
}

static void	tests_ft_validate_sintaxe_not_error(void)
{
	int		expected = 0;
	char	*input = {"ls |>> wc"};
	int		actual = validate_input(input);

	TEST_ASSERT_EQUAL_INT64_MESSAGE(expected, actual, "input sem erro de sintaxe");
}

static void	tests_ft_validate_sintaxe_with_error(void)
{
	int		expected = 1;
	char	*input = {"ls |>>> wc"};
	int		actual = validate_input(input);

	TEST_ASSERT_EQUAL_INT64_MESSAGE(expected, actual, "input com erro de sintaxe >>>");
}

static void	tests_ft_validate_without_space(void)
{
	int		expected = 0;
	char	*input = {"echo\"1\"\"2\""};
	int		actual = validate_input(input);

	TEST_ASSERT_EQUAL_INT64_MESSAGE(expected, actual, "esperado que passe");
}

static void	tests_ft_parsing_with_space(void)
{
		t_dados expected2 = {
		.comando = (char *[]){"echo", "12", NULL},
		.redirect = NULL,
		.nbr_redirections = 0,
		.next = NULL
	};

	char	*input = "echo \"1\"\"2\"";
	t_dados	*actual = parsing(input);

	assert_lista(&expected2, actual);
}

static void	tests_ft_parsing_without_space(void)
{
		t_dados expected2 = {
		.comando = (char *[]){"echo12", NULL},
		.redirect = NULL,
		.nbr_redirections = 0,
		.next = NULL
	};

	char	*input = "echo\"1\"\"2\"";
	t_dados	*actual = parsing(input);

	assert_lista(&expected2, actual);
}
	//start_execution(&test1, &init_env);

void	setUp(void)
{
	// set stuff up here
}

void	tearDown(void)
{
	// clean stuff up here
}

// not needed when using generate_test_runner.rb
int	main(int ac, char **av, char **env)
{
	(void)ac;
	(void)av;
	init_env = init_minienv(env);
	UNITY_BEGIN();
	RUN_TEST(tests_ft_out_redirect_pwd);
	RUN_TEST(tests_ft_append_redirect_ls);
	RUN_TEST(tests_ft_input_redirect);
	RUN_TEST(tests_return_code_error);
	RUN_TEST(tests_ft_more_redirect);
	RUN_TEST(tests_ft_parsing);
	RUN_TEST(tests_ft_validate_open_quotes);
	RUN_TEST(tests_ft_validate_open_quotes_pipe);
	RUN_TEST(tests_ft_validate_open_quotes_pipe2);
	RUN_TEST(tests_ft_validate_sintaxe);
	RUN_TEST(tests_ft_validate_sintaxe_start_pipe);
	RUN_TEST(tests_ft_validate_sintaxe_empty_pipe);
	RUN_TEST(tests_ft_validate_sintaxe_not_error);
	RUN_TEST(tests_ft_validate_sintaxe_with_error);
	RUN_TEST(tests_ft_validate_without_space);
	RUN_TEST(tests_ft_parsing_with_space);
	RUN_TEST(tests_ft_parsing_without_space);
	return (UNITY_END());
}
