proc sum {a b} {
	return [+ $a $b]
}

proc count_to {a} {
    set x 0
    while {< $x $a} {
        puts $x
        set x [sum 1 $x]
    }
}

count_to {10}
