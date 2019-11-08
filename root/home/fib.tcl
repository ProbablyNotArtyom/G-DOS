proc fib {x} {
	if {<= $x 1} {
		return 1
	}
	return [+ [fib [- $x 1]] [fib [- $x 2]]]
}

set i 0
puts "Calculating the first 20 values of the fibonacci sequence"
while {< $i 15} {
	puts [fib $i]
	incr i
}
