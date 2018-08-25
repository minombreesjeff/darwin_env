use 5.006;
use ExtUtils::MakeMaker;

WriteMakefile(
	'NAME'			=> 'Mac::Carbon',
	'VERSION_FROM'		=> 'Carbon.pm',
	'PM'			=> { 'Carbon.pm' => '$(INST_LIBDIR)/Carbon.pm' },
	'PREREQ_PM'	=> {
    		'Test::More'		=> 0,
	},
	'clean'		=> {
		'FILES'		=> 'Mac-Carbon-*'
	},
	'test'		=> {
		'TESTS' 	=> 't/*.t */t/*.t'
	},
	'NO_META'		=> 1,
);
