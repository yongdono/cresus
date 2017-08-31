cmake_minimum_required(VERSION 2.8)
project(cresus)

# src for cresus input
add_executable(snowball
  ema_snowball.c
  )
target_link_libraries(snowball cresus lm)
