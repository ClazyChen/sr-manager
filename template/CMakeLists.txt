cmake_minimum_required (VERSION 3.8)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ({project_name} LANGUAGES CXX)

if (MSVC)
  add_compile_options(/utf-8)
  add_compile_options(/EHsc)
endif()

add_executable ({project_name}
{cpp_files_list}
{header_files_list}
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET {project_name} PROPERTY CXX_STANDARD 20)
endif()
