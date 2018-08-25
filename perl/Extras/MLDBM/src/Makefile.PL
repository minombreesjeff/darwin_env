use ExtUtils::MakeMaker;
WriteMakefile(
              NAME => "MLDBM",
              DISTNAME => "MLDBM",
              VERSION_FROM => 'lib/MLDBM.pm',
	      PREREQ_PM => { Data::Dumper => '2.08' },
              'dist' => {COMPRESS=>'gzip -9f', SUFFIX => 'gz'}
             );
