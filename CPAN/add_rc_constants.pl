
package MY;

#------------------------------------------------------------------------
# - add RC_CFLAGS to both compiling and linking phases
#------------------------------------------------------------------------
sub constants {
    my $self = shift;
    $self->{DEFINE} = defined($self->{DEFINE}) && $self->{DEFINE} ne ''
        ? "$ENV{RC_CFLAGS} $self->{DEFINE}"
        : $ENV{RC_CFLAGS};
    my $dlib = $self->{dynamic_lib};
    $dlib = $self->{dynamic_lib} = {} unless defined($dlib);
    $dlib->{OTHERLDFLAGS}
    = defined($dlib->{OTHERLDFLAGS}) && $dlib->{OTHERLDFLAGS} && $dlib->{OTHERLDFLAGS} ne ''
            ? "$ENV{RC_CFLAGS} $dlib->{OTHERLDFLAGS}"
            : $ENV{RC_CFLAGS};
    return $self->SUPER::constants();
}

