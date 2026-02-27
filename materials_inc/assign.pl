chomp;
next if m{^#} or m{^$};
my ($key, $name, $sound, $color) = split /\s+/;

print "
        case '$key':
          material = $name;
          break;
";
