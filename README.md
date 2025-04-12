/*
 * SWAN-OS - The Swan Jawn Firmware Project
 * --------------------------------------------------
 *
 * Project Overview:
 * -----------------
 * SWAN-OS v0.1 is a modular, cyberpunk-inspired embedded firmware project
 * designed for the ESP32-WROOM-32 (DOIT Devkit V1). The firmware powers a multifunction
 * device featuring an OLED display, rotary encoder input, audio feedback via a piezo buzzer,
 * and planned Wi-Fi packet sniffing, data logging, and battery monitoring.
 *
 * Hardware Components:
 * ----------------------
 * - ESP32-WROOM-32 Devkit
 * - 0.96" OLED display (SSD1306, I2C interface)
 * - Rotary encoder with integrated push-button
 * - Passive piezo buzzer
 * - Common cathode RGB LED
 * - MicroSD card module for storage/logging
 * - 18650 Lithium-ion battery, TP4056 charging module, MT3608 boost converter
 * - Voltage divider for battery monitoring
 *
 * Software Architecture:
 * -----------------------
 * The project is built using PlatformIO with the Arduino framework and organized into separate modules:
 *
 *   • CoreSystem       - Master loop and global state management
 *   • BootSequence     - Startup animations (splash screen) and boot tones
 *   • DisplayUI        - OLED rendering, including menu and UI navigation
 *   • InputHandler     - Processes rotary encoder and button inputs; manages menu selection
 *   • SoundFX          - Audio feedback logic (e.g., Giorno's Theme)
 *   • BatteryMonitor   - Reads battery voltage and controls RGB LED based on battery level
 *   • Sniffer          - (Planned) Wi-Fi promiscuous mode for packet sniffing and data logging
 *   • Storage          - (Planned) SD card file logging (boot logs, packet logs, etc.)
 *   • EEPROMConfig     - (Planned) Persistent storage for configuration settings
 *   • MotorControl     - (Planned) Mechanical features (e.g., motorized effects)
 *
 * Development Roadmap:
 * ----------------------
 * Phase 0: Environment setup, serial testing, and basic configuration.
 * Phase 1: System initialization (LED flash, boot logs, startup tone, splash screen).
 * Phase 2: OLED UI & input integration (rotary encoder, menu navigation, display updates).
 * Phase 3: Wi-Fi packet sniffing and data parsing.
 * Phase 4: Battery monitoring and RGB LED control.
 * Phase 5: Silent Mode (muting the buzzer and LEDs).
 * Phase 6: SD card logging and error handling.
 * Phase 7: UI polish, animations, and additional fun features (e.g., Disco Mode).
 *
 * Usage Notes:
 * -------------
 * - Rapid prototyping was performed in a monolithic sketch on Wokwi.
 * - This repository organizes the project into modular files for clarity and maintenance.
 * - The UI is menu-driven, featuring multiple states (e.g., MAIN_MENU, BUZZER_MENU, SNIFF_MENU).
 * - Future enhancements include remote triggers, EEPROM-based configuration, and additional visual flair.
 *
 * This context is intended to help tools like GitHub Copilot and developers understand the
 * project structure and design philosophy, ensuring relevant code completions and suggestions.
 */
