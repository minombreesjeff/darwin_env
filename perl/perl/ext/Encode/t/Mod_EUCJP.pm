# $Id: Mod_EUCJP.pm,v 1.2 2003/05/20 22:49:21 emoy Exp $
# This file is in euc-jp
package Mod_EUCJP;
use encoding "euc-jp";
sub new {
  my $class = shift;
  my $str = shift || qw/初期文字列/;
  my $self = bless { 
      str => '',
  }, $class;
  $self->set($str);
  $self;
}
sub set {
  my ($self,$str) = @_;
  $self->{str} = $str;
  $self;
}
sub str { shift->{str}; }
sub put { print shift->{str}; }
1;
__END__
