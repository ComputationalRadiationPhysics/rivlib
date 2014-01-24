while (<>) {
	
	if ($indefinition == 1) {
		if (/^#define\s+$extname\s+1/) {
			$inprototypes = 1;
			print "$extname {\n";
		}
	}
	
	if ($inprototypes == 1) {
		# extern void APIENTRY glTexParameterIivEXT
		if (/^\s*(extern|GLAPI)\s+\S.*\S\s*APIENTRY\s+(\S+)\s*\(.*\)/) {
			print "$2\n";
		}
	}
	if (/^#endif/ && $inprototypes == 1) {
		$inprototypes = 0;
		$indefinition = 0;
		print "}\n\n";
	}

	if (/^#ifndef\s+(GL_\S+)/) {
		$extname = $1;
		$indefinition = 1;
	}
}
