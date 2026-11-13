include_guard(GLOBAL)
include(CPM)

CPMAddPackage(
	NAME piggle
	# Preserve the pinned source after upstream rewrote its branch history.
	# A fresh git clone no longer includes this commit; its archive is unchanged.
	URL https://codeload.github.com/Thunderspies/Piggle/tar.gz/184fe00ac370d90c3faa814c3ed4364e36d4aae9
	URL_HASH SHA256=847f042aebbb93bac8d3adb2cde2867fc04c0a78216b0c457c0fd7712bd1b595
	OPTIONS "PIGGLE_BUILD_CLI OFF" "BUILD_TESTING OFF"
)
