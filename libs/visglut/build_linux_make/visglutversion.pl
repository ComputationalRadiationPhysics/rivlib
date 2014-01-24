#
# versioninfo.pl
#
# Copyright (C) 2009 by VISUS (Universitaet Stuttgart).
# Alle Rechte vorbehalten.
#
push @INC, "rev2ver";
require "rev2ver.inc";

my $path = shift;
my $infile = shift;
my $outfile = shift;

my %hash;

my $verinfo = getRevisionInfo($path);

$hash{'$VISGLUT_REVISION$'} = $verinfo->rev;
$hash{'$VISGLUT_DIRTY$'} = $verinfo->dirty;

processFile($outfile, $infile, \%hash);
