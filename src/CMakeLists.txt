set(controller_SRC
  ${CMAKE_CURRENT_SOURCE_DIR}/simple_humanoid_controller.cpp
)

set(controller_HDR
  ${CMAKE_CURRENT_SOURCE_DIR}/simple_humanoid_controller.h
)

# Build the base FSM library
add_library(${PROJECT_NAME} SHARED ${controller_SRC} ${controller_HDR})
target_compile_definitions(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}_EXPORTS)
target_link_libraries(${PROJECT_NAME} PUBLIC mc_rtc::mc_control_fsm)

# Install the base FSM library to the mc_rtc lib dir
install(TARGETS ${PROJECT_NAME}
  ARCHIVE DESTINATION "${MC_RTC_LIBDIR}"
  LIBRARY DESTINATION "${MC_RTC_LIBDIR}"
  RUNTIME DESTINATION bin
)

# Build the controller plugin (wrapper)
add_controller(${PROJECT_NAME}_controller lib.cpp "")

target_link_libraries(${PROJECT_NAME}_controller
  PUBLIC ${PROJECT_NAME}
)

# Install the controller plugin to the mc_rtc lib dir as well
install(TARGETS ${PROJECT_NAME}_controller
  ARCHIVE DESTINATION "${MC_RTC_LIBDIR}"
  LIBRARY DESTINATION "${MC_RTC_LIBDIR}"
  RUNTIME DESTINATION bin
)