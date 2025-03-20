# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/augustin/esp/esp-idf/components/bootloader/subproject"
  "/home/augustin/esp/Tp_IoT_2022_LoRa_Bluetooth/mqtt5/build/bootloader"
  "/home/augustin/esp/Tp_IoT_2022_LoRa_Bluetooth/mqtt5/build/bootloader-prefix"
  "/home/augustin/esp/Tp_IoT_2022_LoRa_Bluetooth/mqtt5/build/bootloader-prefix/tmp"
  "/home/augustin/esp/Tp_IoT_2022_LoRa_Bluetooth/mqtt5/build/bootloader-prefix/src/bootloader-stamp"
  "/home/augustin/esp/Tp_IoT_2022_LoRa_Bluetooth/mqtt5/build/bootloader-prefix/src"
  "/home/augustin/esp/Tp_IoT_2022_LoRa_Bluetooth/mqtt5/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/augustin/esp/Tp_IoT_2022_LoRa_Bluetooth/mqtt5/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/augustin/esp/Tp_IoT_2022_LoRa_Bluetooth/mqtt5/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
