project('cppmacs', 'cpp', default_options: ['cpp_std=c++20'])

glfw = dependency('glfw3')
freetype = dependency('freetype2')
glm = dependency('glm')
gl = dependency('gl')
opengl = dependency('opengl')
glew = dependency('glew')
thread = dependency('threads')

backend = static_library('cppmacs', 
	'src/backend/Rope.h',
	'src/backend/CppMacs.cpp',
	'src/backend/Buffer.cpp',
	'src/backend/BufferCursor.cpp',
	'src/backend/BufferManager.cpp',

	'src/modules/EditModule.cpp',

	'src/misc/Tests.cpp',
	'src/misc/Logger.cpp',
	'src/misc/Util.cpp',

	dependencies: [thread]
)

executable('cppmacs',
	'src/frontend/Entry.cpp',
	'src/frontend/Window.cpp',
	'src/frontend/Program.cpp',
	'src/frontend/Frame.cpp',
        'src/frontend/FrameTree.cpp',
	'src/frontend/Canvas.cpp',
        'src/frontend/InputManager.cpp',

	'src/modules/EditModule.cpp',

	'src/misc/Tests.cpp',
	'src/misc/Logger.cpp',
	'src/misc/Util.cpp',

	link_with: backend,
	dependencies: [glfw, freetype, glm, gl, opengl, glew, thread])
