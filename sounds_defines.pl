BEGIN { $count = 0 }

chomp;
next if m{^#} or m{^$};
my ($def, $file) = /(\S+)\s+(\S+)/;
print "#define $def $count\n";
$count++;

END { print "#define SND_MAX $count\n" }
