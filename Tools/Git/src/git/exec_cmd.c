#include "cache.h"
#include "exec_cmd.h"
#include "quote.h"
#include "argv-array.h"
#define MAX_ARGS	32

#ifdef RUNTIME_PREFIX
#include <unistd.h>
#endif

#if defined(__APPLE__) && defined(RUNTIME_PREFIX)
#include <mach-o/dyld.h>
#endif

static const char *argv_exec_path;
static const char *argv0_path;

char *system_path(const char *path)
{
#ifdef RUNTIME_PREFIX
	static const char *prefix;
#else
	static const char *prefix = PREFIX;
#endif
	struct strbuf d = STRBUF_INIT;

	if (is_absolute_path(path))
		return xstrdup(path);

#ifdef RUNTIME_PREFIX
#ifdef __APPLE__
	/* I found out we can get into this state during tests because of our
	 * system_path() call to find the Xcode-bundled xcconfig because the
	 * tests (eg: test-scrap-cache-tree) don't call git_extract_argv0_path.
	 */
	if (!argv0_path) {
		char new_argv0[PATH_MAX];
		uint32_t new_argv0_s = PATH_MAX;
		if(_NSGetExecutablePath(new_argv0, &new_argv0_s) == 0) {
			const char *slash = new_argv0 + strlen(new_argv0);
			while (new_argv0 <= slash && !is_dir_sep(*slash))
				slash--;

			if (slash >= new_argv0)
				argv0_path = xstrndup(new_argv0, slash - new_argv0);
		}
	}
#endif

	assert(argv0_path);
	assert(is_absolute_path(argv0_path));

	if (!prefix &&
	    !(prefix = strip_path_suffix(argv0_path, GIT_EXEC_PATH)) &&
	    !(prefix = strip_path_suffix(argv0_path, BINDIR)) &&
	    !(prefix = strip_path_suffix(argv0_path, "git"))) {
		prefix = PREFIX;
		trace_printf("RUNTIME_PREFIX requested, "
				"but prefix computation failed.  "
				"Using static fallback '%s'.\n", prefix);
	}
#endif

	strbuf_addf(&d, "%s/%s", prefix, path);
	return strbuf_detach(&d, NULL);
}

const char *git_extract_argv0_path(const char *argv0)
{
	const char *slash;

	if (!argv0 || !*argv0)
		return NULL;

	slash = find_last_dir_sep(argv0);

	if (slash) {
#ifdef RUNTIME_PREFIX
		if (!is_dir_sep(*argv0)) {
			char *path_buf, *s;
			size_t path_buf_size;

			path_buf = getcwd(NULL, 0);
			if (!path_buf)
				die("Unable to determine current working directory");

			path_buf_size = strlen(path_buf) + strlen(argv0) + 2;

			/* xrealloc dies on failure */
			path_buf = xrealloc(path_buf, path_buf_size);
			strlcat(path_buf, "/", path_buf_size);
			strlcat(path_buf, argv0, path_buf_size);

			/* Find the last slash in our new buffer */
			s = path_buf + path_buf_size - 1;
			while (path_buf <= s && !is_dir_sep(*s))
				s--;

			assert(s >= path_buf);

			*s = '\0';
			argv0_path = path_buf;

			return s + 1;
		}
#endif
		argv0_path = xstrndup(argv0, slash - argv0);
		return slash + 1;
#ifdef __APPLE__
	} else {
		char new_argv0[PATH_MAX];
		uint32_t new_argv0_s = PATH_MAX;
		if(_NSGetExecutablePath(new_argv0, &new_argv0_s) == 0) {
			slash = new_argv0 + strlen(new_argv0);
			while (new_argv0 <= slash && !is_dir_sep(*slash))
		                slash--;

			if (slash >= new_argv0)
				argv0_path = xstrndup(new_argv0, slash - new_argv0);
		}
#endif
	}

	return argv0;
}

void git_set_argv_exec_path(const char *exec_path)
{
	argv_exec_path = exec_path;
	/*
	 * Propagate this setting to external programs.
	 */
	setenv(EXEC_PATH_ENVIRONMENT, exec_path, 1);
}


/* Returns the highest-priority, location to look for git programs. */
const char *git_exec_path(void)
{
	const char *env;

	if (argv_exec_path)
		return argv_exec_path;

	env = getenv(EXEC_PATH_ENVIRONMENT);
	if (env && *env) {
		return env;
	}

	return system_path(GIT_EXEC_PATH);
}

static void add_path(struct strbuf *out, const char *path)
{
	if (path && *path) {
		strbuf_add_absolute_path(out, path);
		strbuf_addch(out, PATH_SEP);
	}
}

void setup_path(void)
{
	const char *old_path = getenv("PATH");
	struct strbuf new_path = STRBUF_INIT;

	add_path(&new_path, git_exec_path());

	if (old_path)
		strbuf_addstr(&new_path, old_path);
	else
		strbuf_addstr(&new_path, _PATH_DEFPATH);

	setenv("PATH", new_path.buf, 1);

	strbuf_release(&new_path);
}

const char **prepare_git_cmd(struct argv_array *out, const char **argv)
{
	argv_array_push(out, "git");
	argv_array_pushv(out, argv);
	return out->argv;
}

int execv_git_cmd(const char **argv) {
	struct argv_array nargv = ARGV_ARRAY_INIT;

	prepare_git_cmd(&nargv, argv);
	trace_argv_printf(nargv.argv, "trace: exec:");

	/* execvp() can only ever return if it fails */
	sane_execvp("git", (char **)nargv.argv);

	trace_printf("trace: exec failed: %s\n", strerror(errno));

	argv_array_clear(&nargv);
	return -1;
}


int execl_git_cmd(const char *cmd,...)
{
	int argc;
	const char *argv[MAX_ARGS + 1];
	const char *arg;
	va_list param;

	va_start(param, cmd);
	argv[0] = cmd;
	argc = 1;
	while (argc < MAX_ARGS) {
		arg = argv[argc++] = va_arg(param, char *);
		if (!arg)
			break;
	}
	va_end(param);
	if (MAX_ARGS <= argc)
		return error("too many args to run %s", cmd);

	argv[argc] = NULL;
	return execv_git_cmd(argv);
}
