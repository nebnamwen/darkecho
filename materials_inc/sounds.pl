BEGIN { $count = 0 }

chomp;
next if m{^#} or m{^$};
my ($key, $name, $sound, $color) = split /\s+/;
print "sound_for_material[$name] = $sound;\n";
$count++;
