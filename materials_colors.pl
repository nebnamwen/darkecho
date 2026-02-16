chomp;
next if m{^#} or m{^$};
my ($key, $name, $color) = split /\s+/;
my ($r, $g, $b) = split /,\s*/, $color;

print "
      case $name:
        r = $r; g = $g; b = $b;
        break;
";
