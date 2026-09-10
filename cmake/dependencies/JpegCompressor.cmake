include_guard(GLOBAL)
include(CPM)

CPMAddPackage(
	NAME jpgd
	GITHUB_REPOSITORY Thunderspies/jpeg-compressor
	GIT_TAG a385a2045aa01584987a6bafc0f7e0c420d28c82
	EXCLUDE_FROM_ALL YES
	OPTIONS "BUILD_SHARED_LIBS OFF" "jpge_INCLUDE_PACKAGING OFF"
)

target_compile_features(jpgd PUBLIC cxx_std_11)
target_compile_features(jpge PUBLIC cxx_std_11)
