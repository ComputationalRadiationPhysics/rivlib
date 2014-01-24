#
# genextfile.ps1  20.12.2006
#
# Copyright (C) 2006 by Universitaet Stuttgart (VIS). Alle Rechte vorbehalten.
#

# Use Set-ExecutionPolicy unrestricted for easy execution on Powershell

# checking argument list and setting up the parameters
if ($Args.Length -ge 1) {
	$sourceFile = $Args[0];
	if ($Args.Length -gt 1) {
	    $targetFile = $Args[1];
	} else {
        $targetFile = "extfile.txt";
    }
} else {
    write-host -foregroundcolor red "Syntax Error: Missing command line arguments";
	write-host "usage: genextfile.ps1 <path\to\glext.h> [<outputfile>]";
	write-host "Default outputfile is `"extfile.txt`"`n";
	exit -1;
}

# repeating parameters
write-host "Generating `"$targetFile`" from `"$sourceFile`"`n";

# reading content of the source file
$lines = get-content $sourceFile;

# parsing the lines
$inDefinition = 0;
$inPrototypes = 0;
$extname = "";
$output = "";
$countExtensions = 0;

foreach ($line in $lines) {
    if ($inDefinition -eq 1) {
        if ($line -match "^#define\s+$extname\s+1") {
            $inPrototypes = 1;
            $output += "$extname {`n";
        }
    }
    
    if ($inPrototypes -eq 1) {
        if ($line -match "^\s*(extern|GLAPI)\s+\S.*\S\s*APIENTRY\s+(\S+)\s*\(.*\)") {
            $output += $matches[2] + "`n";
            $countExtensions++;
        }

        if ($line -match "^#endif") {
            $inDefinition = 0;
            $inPrototypes = 0;
            $output += "}`n`n";
        }
    }

    if ($line -match "^#ifndef\s+(GL_\S+)") {
        $extname = $matches[1];
        $inDefinition = 1;
    }
}

# finally write the new file
write-host "$countExtensions Extension(s) found.";

$output = $output.TrimEnd() + "`n";
set-content $targetFile -value $output;

write-host "Done.`n";
