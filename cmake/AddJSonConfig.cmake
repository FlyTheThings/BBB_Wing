MACRO(ADD_JSON_CONFIG config_file)
    file(READ ${config_file} LOCAL_CONFIG)
    string(LENGTH ${LOCAL_CONFIG} LENGTH)
    math(EXPR LENGTH "${LENGTH} - 4")
    string(SUBSTRING ${LOCAL_CONFIG} 1 ${LENGTH} LOCAL_CONFIG)
    if (DEFINED CONFIG)
        set(CONFIG ${CONFIG} "," ${LOCAL_CONFIG} PARENT_SCOPE)
    else ()
        set(CONFIG ${CONFIG} "{" ${LOCAL_CONFIG} PARENT_SCOPE)
    endif()
ENDMACRO()
