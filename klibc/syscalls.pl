#!/usr/bin/perl
($arch) = @ARGV;

while ( defined($line = <STDIN>) ) {
    chomp $line;
    $line =~ s/\s*\#.*$//;	# Strip comments and trailing blanks

    if ( $line =~ /^\s*(\<[^\>]+\>\s+|)([^\(\<\>]+[^:A-Za-z0-9_])([A-Za-z0-9_]+)(|\:\:[A-Za-z0-9_]+)\s*\(([^\:\)]*)\)\s*$/ ) {
	$archs = $1;
	$type = $2;
	$sname = $3;
	$fname = $4;
	$argv = $5;

	$doit = 1;
	if ( $archs ne '' ) {
	    die "$0: Internal error"
		unless ( $archs =~ /^\<(|\!)([^\>\!]+)\>/ );
	    $not = $1;
	    $list = $2;

	    $doit = ($not eq '') ? 0 : 1;

	    @list = split(/,/, $list);
	    foreach  $a ( @list ) {
		if ( $a eq $arch ) {
		    $doit = ($not eq '') ? 1 : 0;
		    last;
		}
	    }
	}
	next if ( ! $doit );

	$type =~ s/\s*$//;

	if ( $fname eq '' ) {
	    $fname = $sname;
	} else {
	    $fname =~ s/^\:\://;
	}

	@args = split(/\s*\,\s*/, $argv);

	open(OUT, "> syscalls/${fname}.c")
	    or die "$0: Cannot open syscalls/${fname}.c\n";
	print OUT "#include \"syscommon.h\"\n\n";
	
	if ( $fname ne $sname ) {
	    print OUT "#undef __NR_${fname}\n";
	    print OUT "#define __NR_${fname} __NR_${sname}\n\n";
	}

	print OUT "_syscall", scalar(@args), "(", $type, ',', $fname;
	$i = 0;
	foreach $arg ( @args ) {
	    print OUT ",", $arg, ",a",$i++;
	}
	print OUT ");\n";
	close(OUT);
    }
}
