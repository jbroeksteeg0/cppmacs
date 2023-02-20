project('cppmacs', 'cpp')

fmt = dependency('fmt')

backend = static_library('cppmacs', 
	'src/backend/Rope.h',
	'src/backend/CppMacs.cpp',

	'src/misc/Tests.cpp',
	'src/misc/Logger.cpp',
	'src/misc/Util.cpp',

	dependencies: fmt
)

executable('cppmacs',
	'src/frontend/Entry.cpp',

	'src/misc/Tests.cpp',
	'src/misc/Logger.cpp',
	'src/misc/Util.cpp',

	dependencies: fmt,
	link_with: backend)
