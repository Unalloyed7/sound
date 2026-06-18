set(SOUND_PROCESSOR_SOURCES
    application.cpp
    application.h
    args_parser.cpp
    args_parser.h
    audio_utils.cpp
    audio_utils.h
    cmd_line_args_2_pipeline_converter.cpp
    cmd_line_args_2_pipeline_converter.h
    filter.h
    filter_descriptor.h
    filter_producers.cpp
    filter_producers.h
    filters.cpp
    filters.h
    pipeline.cpp
    pipeline.h
    wav_file.cpp
    wav_file.h
    waveform.cpp
    waveform.h
)

add_executable(sound_processor
    main.cpp
    ${SOUND_PROCESSOR_SOURCES}
)

add_executable(test_sound_processor
    tests/unit_tests.cpp
    ${SOUND_PROCESSOR_SOURCES}
)

target_link_libraries(test_sound_processor PRIVATE XiCatch2WithMain)
