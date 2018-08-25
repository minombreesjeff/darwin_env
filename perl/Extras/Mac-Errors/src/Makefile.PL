# $Id: Makefile.PL,v 1.6 2004/09/03 00:06:03 comdog Exp $
use ExtUtils::MakeMaker;
	
eval "use Test::Manifest";

unless( $@ )
	{
	* ExtUtils::MM_Any::test_via_harness = sub
		{
		my($self, $perl, $tests) = @_;
		
		return qq|\t$perl "-MTest::Manifest" | .
			qq|"-e" "run_t_manifest(\$(TEST_VERBOSE), '\$(INST_LIB)', | .
			qq|'\$(INST_ARCHLIB)')"\n|;
		}
	}

WriteMakefile(
	'NAME'              => 'Mac::Errors',
	'VERSION_FROM'      => 'lib/Errors.pm',
	'PM'                => { 'lib/Errors.pm' => '$(INST_LIBDIR)/Errors.pm' },
	'PREREQ_PM'         => {
		'Test::More'  => '0',
		},
	'EXE_FILES'         => [ 'bin/macerror' ],
	'MAN3PODS'          => {
		'lib/Errors.pm'    => '$(INST_MAN3DIR)/Mac::Errors.3',
		},

	clean               => { FILES => 'Mac-Errors-*' },
);
