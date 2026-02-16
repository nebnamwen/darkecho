BEGIN { $count = 0 }

chomp;
next if m{^#} or m{^$};
my ($key, $name, $color) = split /\s+/;
print "#define $name $count\n";
$count++;
