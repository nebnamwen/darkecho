chomp;
next if m{^#} or m{^$};
my ($def, $file) = /(\S+)\s+(\S+)/;
print "LoadSound($def, \"$file\");\n";
