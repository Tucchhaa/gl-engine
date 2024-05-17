function(get_demos_source_files)
    set(DEMOS_SOURCE_FILES
            # demos
            demos/backpack-demo.cpp
            demos/backpack-demo.hpp

            demos/tunnel-demo.cpp
            demos/tunnel-demo.hpp

            demos/many-light-demo.cpp
            demos/many-light-demo.hpp
            PARENT_SCOPE
    )

    return()

endfunction(get_demos_source_files)

function(get_core_source_files)
    set(CORE_SOURCE_FILES
            # core
            core/include.hpp

            core/game-object.cpp
            core/game-object.hpp

            core/hierarchy.cpp
            core/hierarchy.hpp

            core/scene.cpp
            core/scene.hpp

            core/editor.cpp
            core/editor.hpp

            core/imesh.hpp

            # core/loader
            core/loader/loader.cpp
            core/loader/loader.hpp

            core/loader/model-parser.cpp
            core/loader/model-parser.hpp

            core/loader/material-info.cpp
            core/loader/material-info.hpp

            # core/structures
            core/structures/vectors.cpp
            core/structures/vectors.hpp

            core/structures/texture.cpp
            core/structures/texture.hpp

            core/structures/texture-options.cpp
            core/structures/texture-options.hpp
            PARENT_SCOPE
    )

    return()
endfunction(get_core_source_files)

function(get_core_components_source_files)
    set(CORE_COMPONENTS_SOURCE_FILES
            # core/components
            core/components/lights/light.cpp
            core/components/lights/light.hpp

            core/components/lights/direct-light.cpp
            core/components/lights/direct-light.hpp

            core/components/lights/point-light.cpp
            core/components/lights/point-light.hpp

            core/components/lights/spot-light.cpp
            core/components/lights/spot-light.hpp

            core/components/camera.cpp
            core/components/camera.hpp

            core/components/component.cpp
            core/components/component.hpp

            core/structures/material.cpp
            core/structures/material.hpp

            core/components/mesh.cpp
            core/components/mesh.hpp

            core/components/terrain.cpp
            core/components/terrain.hpp

            core/components/cubic-patch.cpp
            core/components/cubic-patch.hpp

            core/components/transform.cpp
            core/components/transform.hpp

            core/components/components-manager.cpp
            core/components/components-manager.hpp
            PARENT_SCOPE
    )

    return()
endfunction(get_core_components_source_files)

function(get_apis_base_source_files)
    set(APIS_BASE_SOURCE_FILES
            # base
            apis/base/iwindow.hpp
            apis/base/iwindow.cpp

            apis/base/iresource-manager.hpp

            apis/base/irender-object.hpp
            apis/base/irender-object.cpp

            apis/base/iinput.hpp
            apis/base/iinput.cpp

            apis/base/irenderer.hpp
            apis/base/irenderer.cpp

            apis/base/iengine.hpp
            apis/base/iengine.cpp

            apis/base/ieditor-view-renderer.hpp
            apis/base/ieditor-view-renderer.cpp
            PARENT_SCOPE
    )

    return()
endfunction(get_apis_base_source_files)

function(get_apis_opengl_source_files)
    set(APIS_OPENGL_SOURCE_FILES
            # apis/opengl
            apis/opengl/include.hpp

            apis/opengl/gl-check-error.cpp
            apis/opengl/gl-check-error.hpp

            apis/opengl/window.cpp
            apis/opengl/window.hpp

            apis/opengl/input.cpp
            apis/opengl/input.hpp

            apis/opengl/gl-engine.cpp
            apis/opengl/gl-engine.hpp

            # apis/opengl/renderer.cpp
            # apis/opengl/renderer.hpp

            apis/opengl/shader.cpp
            apis/opengl/shader.hpp

            apis/opengl/resource-manager.cpp
            apis/opengl/resource-manager.hpp

            apis/opengl/render-object.cpp
            apis/opengl/render-object.hpp

            apis/opengl/deferred-renderer.cpp
            apis/opengl/deferred-renderer.hpp

            apis/opengl/editor-view-renderer.cpp
            apis/opengl/editor-view-renderer.hpp
            PARENT_SCOPE
    )

    return()
endfunction(get_apis_opengl_source_files)

function(get_source_files)
    get_core_source_files()
    get_core_components_source_files()
    get_apis_base_source_files()
    get_apis_opengl_source_files()

    set(SOURCE_FILES
            ${CORE_SOURCE_FILES}
            ${CORE_COMPONENTS_SOURCE_FILES}
            ${APIS_BASE_SOURCE_FILES}
            ${APIS_OPENGL_SOURCE_FILES}
            PARENT_SCOPE
    )

    return()
endfunction(get_source_files)


