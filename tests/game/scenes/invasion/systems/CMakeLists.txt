target_sources(
  ${PROJECT_NAME}_tests
  PRIVATE
  animation_test.cpp
  collision_detection_test.cpp
  #deletion_test.cpp // TODO bring this back
  position_following_test.cpp
  velocity_test.cpp
)