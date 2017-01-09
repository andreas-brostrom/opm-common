# -*- mode: cmake; tab-width: 2; indent-tabs-mode: t; truncate-lines: t; compile-command: "cmake -Wdev" -*-
# vim: set filetype=cmake autoindent tabstop=2 shiftwidth=2 noexpandtab softtabstop=2 nowrap:

# defines that must be present in config.h for our headers
set (opm-flowdiagnostics-applications_CONFIG_VAR
	)

# dependencies
set (opm-flowdiagnostics-applications_DEPS
	# compile with C99 support if available
	"C99"
	# compile with C++0x/11 support if available
	"CXX11Features REQUIRED"
	"Boost 1.44.0
		COMPONENTS filesystem regex system unit_test_framework REQUIRED"
	"ERT REQUIRED"
	# prerequisite OPM modules
	"opm-common REQUIRED;
	opm-flowdiagnostics REQUIRED;
	opm-core REQUIRED"
	)
