/*
	PROJECT: Medline database.
	MODULE:	 cmdline
	FILES:	 cmdline.c, (this one) cmdline.h

	This module contains the ProcessCommandLine procedure which parses
	the contents of the command line and stores the results in global
	variables (global module).

	Work started: 8 May 1991, Rand Huntzinger
	Original version completed: 24 July 1991, Rand S. Huntzinger
*
*
* RCS Modification History:
* $Log: cmdline.c,v $
* Revision 6.0  1997/08/25 18:36:03  madden
* Revision changed to 6.0
*
* Revision 1.3  1995/05/17 17:54:29  epstein
* add RCS log revision history
*
*/

#include <stdio.h>
#include <string.h>
#include "ma_global.h"


#define rindex(s,c) strrchr(s,c)

/* The flagHelp field is made true if the -help flag is seen.  It means that
   the user wants us to print extensive help information and then exit without
   actually processing any input files. */

static	short	flagHelp = FALSE;	/* TRUE => print help and exit */


/* This table can be used to initialize string variables from the
   environment.  The env field contains the name of the environment
   variable, the default field contains a default to be used if the
   environment variable is non-null, and var contains the address
   where the string pointer of the default is to be placed.  These
   defaults can be overriddent by command line options. */

static struct env_default {
	char	*env;
	char	*def_value;
	char	**var;
} init_env_table[] = {

	/* These variables define the defaults to be used for logging
	   into the Medline database under Sybase.  They include the
	   database name, server and Sybase user name and password */

	ENV_DATABASE,	DEF_DATABASE,	&flagDatabase,
	ENV_USER,	DEF_USER,	&flagUser,
	ENV_SERVER,	DEF_SERVER,	&flagServer,
	ENV_PASSWORD,	DEF_PASSWORD,	&flagPassword,
	ENV_CONFIGFILE,	DEF_CONFIGFILE,	&flagConfigFile,

};
static int init_env_count = (sizeof(init_env_table)/sizeof(struct env_default));


/* The FlagType enumerated type defines the various kinds of flags we
   need to be able to interpret.  They are used in the flag_options
   table to get us to the proper flag handling routine for a given flag */

typedef enum
	{ FtypeFormat, FtypeString, FtypeFlag, FtypeInt, FtypeWrite } FlagType;


/* The flag_options table contains data describing all of the valid flag
   options.  It includes the text of the flag (ie. -update), text for the
   generation of the help message (arg_note, help_message), data for
   processing the flag (type, seen, option) and an address where the result
   of flag processing is to be placed (address) */

static struct flag_record {
    char	*flag;			/* The name of the flag */
    char	*arg_note;		/* The argument notation */
    char	*help_message;		/* The associated help message */
    FlagType	 type;			/* The type of the flag */
    short	 seen;			/* Mark here if we've seen one. */
    short	 option;		/* Select options */
    void	*address;		/* Where to store the result */
} flag_options[] = {
   {	"-database",	"name",	"Sybase database to be used",
	FtypeString,	0,	0,	&flagDatabase			},

   {	"-server",	"name",	"Sybase server to be used",
	FtypeString,	0,	0,	&flagServer			},

   {	"-user",	"name",	"Sybase login name (default is Unix login)",
	FtypeString,	0,	0,	&flagUser			},

   {	"-password",  "string",	"Password for Sybase login (prompt if omitted)",
	FtypeString,	0,	0,	&flagPassword			},

   {	"-iso",		"",	"Output articles in ISO Latin 1 character set",
	FtypeFlag,	0,	0,	&flagISO			},

   {	"-write",	"format file",	"Write a given format to a given file",
	FtypeWrite,	0,	0,	NULL				},

   {	"-format",	"name object symbol",
					"Define how to write a given format",
	FtypeFormat,	0,	0,	NULL				},

   {	"-config",	"file",		"Override default configuration file",
	FtypeString,	0,	0,	&flagConfigFile			},

   {	"-verbose",	"",	"Print out job status information",
	FtypeFlag,	0,	0,	&flagVerbose			},

   {	"-debug",	"",	"Verbose output & rollback changes (debugging)",
	FtypeFlag,	0,	0,	&flagDebug			},

   {	"-trace",	"",	"Print out queries executed (debugging)",
	FtypeFlag,	0,	1,	&flagTrace			},

   {	"-help",	"",	"Print usage message and runtime parameters",
	FtypeFlag,	0,	0,	&flagHelp			},
};
static int flag_option_count =
	(sizeof(flag_options)/sizeof(struct flag_record));


/*
	isint			Is a string an integer value?

	This procedure returns TRUE if the string is an integer value or
	FALSE if not.  A leading + or - is permitted.

	Parameters:

	    s		A pointer to the string to be tested.

	Returns:

	    TRUE if a string representation of an integer, FALSE
	    otherwise.
*/

/*
*		isint(s)
*/
int	isint( s )
	char	*s;
{
    /* NULL's ain't numeric! */

    if( s == NULL ) return( FALSE );

    /* Leading + or - followed by at least one digit */

    if( *s == '+' || *s == '-' ) s++;
    if( ! isdigit( *s ) ) return( FALSE );

    /* Everything else must be a digit */

    while( isdigit( *s ) ) s++;
    return( (*s == NULL) ? TRUE : FALSE );
}


/*
	init_command_globals	Internal procedure to initialize defaults.

	This procedure uses the init_env_table table to establish default
	values for certain string variables using environment variables
	which may override certain hard-wired defaults.  These defaults
	can then be overridden by switch options as well.

	This procedure also initializes the programName global variable.

	Parameters:

	    argv0		The name of the program (possibly with a
				directory path).  Used to initialize the
				programName variable.

	Returns:	nothing
*/

static
/*
*		init_command_globals(argv0)
*/
void	init_command_globals( argv0 )
	char	*argv0;
{
    register int	i;

   /* Set the programName global variable */

    programName = rindex( argv0, '/' );
    programName = ( programName == NULL ) ? argv0 : programName + 1;

   /* Initialize some variables from the environment */

    for( i = 0; i < init_env_count; i++ ) {
	char	*v = getenv( init_env_table[i].env );
	*init_env_table[i].var = (v == NULL) ? init_env_table[i].def_value : v;
    }
}


/* The ARGUMENT_CHECK macro checks to see if the argument at index I in
   the argv vector (V argument) has an argument (not at the end as
   determined by argc (C argument) and the next item does not start with
   a "-" indicating another flag. */

#define	ARGUMENT_CHECK2(C,V,F,I) if((I) >= (C) || ((V)[I][0] == '-' \
		&& (V)[I][1] != (char) 0)) { \
	fprintf(stderr, "%s: missing or invalid arguments for option %s\n", \
		programName, (V)[F] ); error_noted = TRUE; break; }
#define	ARGUMENT_CHECK(C,V,I) ARGUMENT_CHECK2(C,V,I,I+1)
#if 0
#define	ARGUMENT_CHECK(C,V,I) if(((I)+1) >= (C) || (V)[(I)+1][0] == '-' ) { \
	fprintf(stderr, "%s: option %s requires an argument\n", \
		programName, (V)[I] ); error_noted = TRUE; break; }
#endif

/*
	process_flag_options	Internal routine to process flag arguments.

	This procedure interprets the flag arguments passed in argc and
	argv and decodes them into the variables to receive their values.

	Parameters:

	    argc		The number of arguments in argv.

	    argv		An array of strings containing the arguments.

	    argument_label	Label for non-option arguments.

	Returns:
	    The index of the first non-flag argument (file name) if there
	    was no error, or the negative of this value if there was an
	    error.
*/

static
/*
*		process_flag_options(argc,argv,argument_label)
*/
int	process_flag_options( argc, argv, argument_label )
	int	  argc;
	char	**argv;
	char	 *argument_label;
{
    register int	 a;
    short		 error_noted = FALSE;
    short		 mode_option = 0;

    /*	Loop over the command line extracting the command options.  The
	first unprotected token (ie. not a flag argument) which does not
	start with "-" is treated as the start of the input file list.  */

    for( a = 1; a < argc && argv[a][0] == '-'; a++ ) {
	register int i;

	/* Look the flag up in the argument list */

	for( i = 0; i < flag_option_count; i++ )
	    if( strcmp( flag_options[i].flag, argv[a] ) == 0 )
		break;

	/* Process the flag */

	if( i < flag_option_count ) {
	    switch( flag_options[i].type ) {
		case FtypeString:	/* Character string arguments */
		    ARGUMENT_CHECK(argc,argv,a);
		    *((char **) flag_options[i].address) = argv[++a];
		    break;
		case FtypeFlag:		/* Simple Boolean flag arguments */
		    *((short *) flag_options[i].address) = 1;
		    break;
		case FtypeInt:		/* Integer (long) value arguments */
		    ARGUMENT_CHECK(argc,argv,a);
		    ++a;
		    if(flag_options[i].option && strcmp(argv[a], "all") == 0) {
			*((long *) flag_options[i].address) = -1L;
			break;
		    }
		    if( ! isint( argv[a] ) || atol( argv[a] ) < 0) {
			fprintf(stderr,
			    "%s: %s argument not an positive integer value.\n",
			    programName, argv[a-1] );
			break;
		    }
		    *((long *) flag_options[i].address) = atol(argv[a]);
		    break;
		case FtypeWrite:	/* Add an output stream option */
		    ARGUMENT_CHECK(argc, argv, a);
		    ARGUMENT_CHECK2(argc, argv, a, (a+2));
		    if( outputStreamCount >= MAX_OUTPUT_STREAMS ) {
			fprintf( stderr,
	"%s: Only %d output streams are allowed, %s flag ignored!\n",
				 MAX_OUTPUT_STREAMS, argv[a] );
			error_noted = TRUE;
		    } else {
			outputStreams[outputStreamCount].format = argv[a+1];
			outputStreams[outputStreamCount].file = argv[a+2];
			outputStreams[outputStreamCount].handle = NULL;
			outputStreamCount++;
		    }
		    a += 2;
		    break;
		default:		/* This shouldn't happen */
		    fprintf(stderr,
			"%s: program error: bad flag_options[%d].type for %s\n",
			programName, i, argv[a] );
	    }

	    /* Flag an error on duplicates */

	    if( flag_options[i].seen == 1) {
		fprintf(stderr, "%s: option %s seen more than once\n",
		    programName, argv[a] );
		error_noted = TRUE;
	        flag_options[i].seen++;	/* We've seen this one! */
	    }
	} else {
	    /* Invalid flag, issue diagnostic and skip protected argument */

	    fprintf(stderr, "%s: invalid option (%s)\n", programName, argv[a] );
	    if( argc > (a+2) && argv[a+1][0] != '-' && argv[a+2][0] == '-' )
		a++;			/* allow one protected argument */
	    error_noted = TRUE;
	}
    }

    /* Final argument is start of file list */

    return( error_noted ? -a : a );
}


/*
	process_arguments	Internal procedure to check file arguments

	This procedure checks the non-option arguments.  It makes sure that
	each file exists and is readable.  When done, it stores a pointer
	to the input file list in argumentList and the number of input
	files in argumentCount.

	Parameters:

	    arg_index		Index into argv pointing to the first
				putative input file name.

	    argc		The number of command line arguments.

	    argv		The list of command line arguments.

	Returns:
	    TRUE if the input list was OK, FALSE if there were errors.
*/
/*
 *   process_arguments( arg_index, argc, argv, check_procedure)
 */
static
int	process_arguments( arg_index, argc, argv, check_procedure )
	int	  arg_index;
	int	  argc;
	char	**argv;
	int	(*check_procedure)();
{
    register int	i;
    int			error_noted = FALSE;

    /*	Check all of the input arguments */

    for(i = arg_index; i < argc; i++ )
	error_noted |= (! check_procedure( i - arg_index, argv[i] ) );

    /* Set the input file count and list */

    argumentCount = argc - arg_index;
    argumentList = &argv[arg_index];

    /* Done */

    return( ! error_noted );
}


/*
	usage_line	Internal procedure to print an option help line

	This procedure prints out the output line describing the option
	flag with index 'i' in the flag_options table.

	Parameters:

	    i		The index into the flag_options table giving
			the line to be printed.

	Returns:	nothing
*/

static
/*
*		usage_line(i)
*/
void	usage_line( i )
	int	i;
{
    char option[100];

    /* Generate the full option and argument string */

    sprintf( option, "%.40s %.40s", flag_options[i].flag,
	flag_options[i].arg_note );

    /* Now print out the usage line using this */

    fprintf( stderr, "    %-20.20s %.50s\n", option,
	flag_options[i].help_message );
}


/*
	output_command_summary	    Internal procedure to output parameters.

	This procedure scans the flag_options table to produce a list of
	the run parameters for this program as determined from the hard-
	wired defaults, environment variables and command line arguments.
	It is part of the extensive output generated by the -help command.

	Parameters:

	    argument_label		Label for non-option arguments

	    check_procedure		Procedure to validate non-option
					arguments.

	Returns		nothing.
*/

/*
 * output_command_summary()
*/
static
void	output_command_summary( argument_label, check_procedure )
	char	*argument_label;
	int	(*check_procedure)();
{
    register int i;

    fprintf(stderr, "\nParameters after command line processing:\n\n" );
    for(i = 0; i < flag_option_count; i++ ) {
	char	*arg;

	if( flag_options[i].type != FtypeFormat ) {
	    fprintf(stderr, "    %10.10s: ", flag_options[i].flag );
	    switch( flag_options[i].type ) {
		case FtypeString:
		    arg = *((char **) flag_options[i].address);
		    fprintf(stderr, "%s\n", (arg == NULL) ? "(default)" : arg );
		    break;
		case FtypeInt:
		    fprintf(stderr, "%ld\n",
			*((long *) flag_options[i].address) );
		    break;
		case FtypeWrite:
		    fprintf(stderr, "(see output conversion table below)\n" );
		    break;
		case FtypeFlag:
		    fprintf(stderr, "%s\n",
			*((short *) flag_options[i].address) ? "on" : "off" );
		    break;
		default:
		    fprintf(stderr, "<<ERROR>>\n" );
	    }
	}
    }

   fprintf(stderr, "\nOutput conversions (specified by -write options)\n");
   if( outputStreamCount > 0 ) {
	for(i = 0; i < outputStreamCount; i++)
	    fprintf(stderr, "%4d) %s to %s\n", i+1, outputStreams[i].format,
		strcmp(outputStreams[i].file, "-") == 0 ? "standard output" :
		outputStreams[i].file );
    } else {
	fprintf( stderr, "   1) %s to standard output (default)\n",
	    DEFAULT_OUTPUT_FORMAT );
    }

    if( argumentCount > 0 ) {
	fprintf(stderr, "\n%s: (n = %d)\n\n", argument_label, argumentCount );
	for( i = 0; i < argumentCount; i++ ) {
	    fprintf(stderr, "%5d) %s [%sok]\n", i+1,
		argumentList[i],
		check_procedure( i, argumentList[i] ) ? "" : "not " );
	}
    }
}


/*
	usage_error	Internal procedure to output the entire usage message

	This procedure prints out the entire usage message and then
	exits from the program.  The error_noted flag indicates whether
	the program exits with a normal status (no error noted) or a
	usage error status (if an error was noted).  The normal status
	exit occurs when the -help flag was used and there were no
	command line errors.

	Parameters:

		error_noted		If true, exit with usage error
					status; otherwise, exit with
					normal status.

		argument_label		Label to use on on-option arguments.

	Returns:

	    This procedure does not return to the calling program, it
	    exits from the program with either EXIT_STATUS_OK (normal
	    exit status) or EXIT_STATUS_USAGE (usage error status)
*/

/*
 * usage_error()
*/
static
void	usage_error( error_noted, argument_label, check_procedure )
	int	error_noted;
	char	*argument_label;
	int	(*check_procedure)();
{
    register int	i;

    /* Print out the basic usage error message */

    fprintf(stderr, "\nUsage: %s [ options ] [ input_file ... ]\n\n",
	programName );

    /* Print out the mode options */

    fprintf(stderr, "Usage mode options: (exclusive)\n" );
    for(i = 0; i < flag_option_count; i++ )
	if( flag_options[i].type == FtypeFormat )
	    usage_line( i );

    /* Print out the other options */

    fprintf(stderr, "\nOther options:\n" );
    for(i = 0; i < flag_option_count; i++ )
	if( flag_options[i].type != FtypeFormat )
	    usage_line( i );

    /* Print out the analysis of the command line options */

    if( flagHelp ) output_command_summary( argument_label, check_procedure );

    /* Exit with the appropriate error status */

    exit( error_noted ? EXIT_STATUS_USAGE : EXIT_STATUS_OK );
}


/*
	ProcessCommandLine:	Parse the Unix style command line.

	This procedure parses the Unix command line and stores the results
	of the parse in a set of global areas defined above.  If an error
	occurs, this procedure will generate a usage error diagnostic and
	continue exit.

	Parameters:

	    argc		The number of command line arguments.

	    argv		The list of command line arguments.

	Return value:	none.
*/
/*
 * ProcessCommandLine()
*/
void	ProcessCommandLine( argc, argv, argument_label, check_procedure )
	int	  argc;
	char	**argv;
	char	 *argument_label;
	int	(*check_procedure)();
{
    int		arg_index;

    /* Initialize the command line global variables */

    init_command_globals( *argv );

    /* Process the command line flag arguments */

    arg_index = process_flag_options( argc, argv, argument_label );
    if( flagDebug ) {
	flagVerbose = TRUE;		/* Debug => verbose too */
    }

    /* Check the input file names */

    if( ! process_arguments( abs(arg_index), argc, argv, check_procedure ) ||
		arg_index < 0 )
	usage_error( TRUE, argument_label, check_procedure );

    /* If this is a -help run, just print the diagnostics */

    if( flagHelp ) usage_error( FALSE, argument_label, check_procedure );

    /* Setup a few things */

    /*if( flagTrace ) SybaseQueryPrintCount = -1;*/
    if( outputStreamCount < 1 ) {
	/* Default the output if necessary */
	outputStreamCount = 1;
	outputStreams[0].format = DEFAULT_OUTPUT_FORMAT;
	outputStreams[0].file = DEFAULT_OUTPUT_FILE;
	outputStreams[0].handle;
    }

    /* Done */
}
