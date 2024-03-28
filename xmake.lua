add_rules("mode.debug", "mode.release")

add_requires("openvr")

target("ovr_tracker_native")
    set_kind("shared")
    set_languages("cxx20")

    add_files("native/*.cpp")
    add_headerfiles("native/*.h")

    add_shflags("-static")

    add_packages("openvr")