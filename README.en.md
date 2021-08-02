OpenHamony# linkboy-hos
####Introduction
This project is a plug-in extension of openharmony operating system, which can support linkboy to program openharmony
####Software architecture
This project includes:
1. Encapsulation of openharmony function (HAL: GPIO, UART, timer,...)
2. Provide a simplified virtual machine implementation, which can run the back-end bytecode sequence of linkby programming language
3. It contains some special drivers, such as infrared decoding, stepping motor drive, temperature and humidity DS18B20, DHT series
####Instructions for use
1. Place the Vos folder in the application \ sample \ WiFi IOT \ app folder of the openharmony source code;
2. Change the build.gn file content of the parent folder to:
import("//build/lite/config/component/lite_ component.gni")
lite_ component("app") {
features = [
"vos:main"
]
}
####Participation contribution