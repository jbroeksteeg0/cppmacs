project('cppmacs', 'cpp', default_options: ['cpp_std=c++20'])

glfw = dependency('glfw3')
freetype = dependency('freetype2')
glm = dependency('glm')
gl = dependency('gl')
opengl = dependency('opengl')
glew = dependency('glew')

backend = static_library('cppmacs', 
	'src/backend/Rope.h',
	'src/backend/CppMacs.cpp',
	'src/backend/Buffer.cpp',
	'src/backend/BufferCursor.cpp',

	'src/misc/Tests.cpp',
	'src/misc/Logger.cpp',
	'src/misc/Util.cpp',
)

executable('cppmacs',
	'src/frontend/Entry.cpp',
	'src/frontend/Window.cpp',
	'src/frontend/Program.cpp',

	'src/misc/Tests.cpp',
	'src/misc/Logger.cpp',
	'src/misc/Util.cpp',

	link_with: backend,
	dependencies: [glfw, freetype, glm, gl, opengl, glew])
