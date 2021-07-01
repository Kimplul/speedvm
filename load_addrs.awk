/<START_INSNS>:/{
	init_addr = strtonum($1);
}

/<[A-Z]+>:/{
	gsub(/[<:>]/, "", $2);
	addrs[$2] = strtonum($1) - init_addr;
}

END{
	print "#ifndef INSTR_H"
	print "#define INSTR_H"
	print "enum INSTR {";
	
	for (addr in addrs) {
		print addr, "=", addrs[addr], ",";
	}

	print "};"
	print "#endif // INSTR_H"
}
