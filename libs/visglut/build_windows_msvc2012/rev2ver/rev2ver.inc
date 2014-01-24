#
# rev2ver.pl
# Copyright (c) 2009, VISUS
# (Visualization Research Center, Universitaet Stuttgart)
# All rights reserved.
#
# See: LICENCE.TXT or
# https://svn.vis.uni-stuttgart.de/utilities/rev2ver/LICENCE.TXT
#
use strict;
use Class::Struct;
use File::Spec::Functions qw{rel2abs};
use File::stat;
use IO::Dir;

my $svnOLE;

struct revisionInfo => [
    rev => '$',
    author => '$',
    dirty => '$',
    uri => '$',
    date => '$',
    modDate => '$'
];

sub recurse {
    my $dir;
    $dir = shift;
    my $d;
    $d = new IO::Dir($dir);
    $svnOLE->GetWCInfo($dir, 1, 0);
    my $baseURI = $svnOLE->Url;
    $baseURI =~ s/\/$//;

    my $newest = 0;
    while(defined(my $x = $d->read)) {
        if ($x ne "." && $x ne ".." && $x ne ".svn") {
            $svnOLE->GetWCInfo($dir."\\".$x, 1, 0);
            my $childURI = $svnOLE->Url;
            $childURI =~ s/\/$//;
            if (($childURI eq "$baseURI/$x") && ($svnOLE->HasModifications == 1 || ($svnOLE->MaxRev != $svnOLE->MinRev))) {
                if (-d $dir."\\".$x) {
                    my $mod = recurse($dir."\\".$x);
                    if ($mod > $newest) {
                        $newest = $mod;
                    }
                } else {
                    my $mod = stat($dir."\\".$x)->[9];
                    if ($mod > $newest) {
                        $newest = $mod;
                        #print $dir."\\".$x." is newest with $mod\n";
                    }
                }
            }
        }
    }
    $d->close;
    return $newest;
}

# getRevisionInfo(path)
# extract information from the working copy at path
sub getRevisionInfo($) {
    my $path = rel2abs(shift);
    my $rev;
    my $author;
    my $dirty;
    my $uri;
    my $date;
    my $newest = 0;
    my @modifiedFiles = ();

    my $oleModule = "Win32::OLE";
    eval "use $oleModule";
    if ($@ || !defined($oleModule) || !defined($svnOLE = $oleModule->new('SubWCRev.object', ''))) {
        # cannot use OLE, no tortoise installed, either linux or windows command line
        undef $/;
        open(HURZ, "svn info -R --xml $path |");
        my $result = <HURZ>;
        close(HURZ);
        if (!($result =~ /<\?xml version="1.0"\?>/g)) {
            # default values
            $rev = 0;
            $author = "";
            $dirty = 1;
            $uri = "";
            $date = "";
        } else {
            my $maxRev = 0;
            foreach my $line ($result =~ /<commit.*?<\/commit>/sg) {
                $line =~ /revision="(.*?)"/s;
                if ($1 > $maxRev) {
                    $maxRev = $1;
                    $line =~ /<author>(.*?)<\/author>/s;
                    $author = $1;
                    $line =~ /<date>(.*?)<\/date>/s;
                    $date = $1;
                }
            }
            $result =~ /<url>(.*?)<\/url>/s;
            $uri = $1;
            $rev = $maxRev;
            $dirty = 0;
            $maxRev = -1;
            foreach my $line ($result =~ /<entry[^>]*?revision="(.*?)"[^>]*?>/sg) {
                if ($maxRev == -1) {
                    $maxRev = $line;
                } elsif ($maxRev != $line) {
                    $dirty = 1;
                    last;
                }
            }
            open(HURZ, "svn status --ignore-externals --xml $path |");
            my $result = <HURZ>;
            close(HURZ);
            foreach my $line ($result =~ /<entry.*?<\/entry>/sg) {
                if ($line =~/<wc-status[^>]*item="modified"/s) {
                    $dirty = 1;
                    $line =~ /path="([^"]+)"/s;
                    #push @modifiedFiles, $1;
                    my $mtime = stat($1)->[9];
                    if ($mtime > $newest) {
                        $newest = $mtime;
                        print "modified: $1 at $mtime\n";
                    }
                }
            }
        }
    } else {
        $svnOLE->GetWCInfo($path, 1, 0);
        $rev = $svnOLE->Revision; # = "Last Changed Revision" !
        $author = $svnOLE->Author;
        $dirty = $svnOLE->HasModifications == 1 || ($svnOLE->MaxRev != $svnOLE->MinRev);
        if (!$dirty) {
            $dirty = 0;
        }

        $uri = $svnOLE->Url;
        $date = $svnOLE->Date();
        $date =~ s/\//-/g;
        $date =~ s/ /T/;
        $date .= 'Z';

        if ($dirty == 1) {
            my $mod = recurse($path);
            if ($mod > $newest) {
                $newest = $mod;
            }
        }

    }

    my $ret = revisionInfo->new();
    $ret->rev($rev);
    $ret->author($author);
    $ret->dirty($dirty);
    $ret->uri($uri);
    $ret->date($date);
    $ret->modDate($newest);
    return $ret;
}

# processFile(outFile, inFile, keywordMap)
# load inFile
# foreach k (keys keywordMap) {
#   replace all occurrences of k with keywordMap{k}
# }
# write outFile
sub processFile($$%) {
    my $outFile = shift;
    my $inFile = shift;
    my %hash = %{shift()};
    my $k;

    my $outputBuffer = "";

    if (open IN, $inFile) {
        while(<IN>) {
            foreach $k (keys %hash) {
                #print $k . " -> " . $hash{$k} . "\n";
                s/\Q$k\E/$hash{$k}/g;
            }
            $outputBuffer .= $_;
        }
        close IN;
    } else {
        print "could not read from " . $inFile . "\n";
    }

    writeFileChanges($outFile, $outputBuffer);
}

# writeFileChanges(outFile, content)
# write content to outFile only if the content is changed
sub writeFileChanges($$) {
    my $outFile = shift;
    my $outputBuffer = shift;
    
    if (open IN, $outFile) {
        undef $/;

        my $old = <IN>;
        close IN;

        if ($old eq $outputBuffer) {
            return;
        }
    }

    if (open OUT, ">".$outFile) {
        print OUT $outputBuffer;
        close OUT;
    } else {
        print "could not write to " . $outFile . "\n";
    }
}

return 1;