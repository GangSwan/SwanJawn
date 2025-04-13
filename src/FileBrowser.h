#ifndef FILEBROWSER_H
#define FILEBROWSER_H

#include <Arduino.h>
#include <Adafruit_SSD1306.h>

// Maximum number of files to list from the SD card
#define MAX_FILES 20
// Maximum number of lines to store from a file (for viewing)
#define MAX_FILE_LINES 50
// Number of text lines per page on the OLED (depends on font size)
#define LINES_PER_PAGE 8

namespace FileBrowser {
  // Browser state: listing available files or viewing a selected file
  enum BrowserState {
    FILE_LIST,  // Display a list of file names
    FILE_VIEW   // Display the content of a selected file
  };

  extern BrowserState browserState;

  // Array to store filenames and related indexing:
  extern String fileNames[MAX_FILES];
  extern int fileCount;
  extern int selectedFileIndex;

  // For file viewing: store lines of text from a file and track pagination
  extern String fileLines[MAX_FILE_LINES];
  extern int fileLineCount;
  extern int currentPage;

  // Scans the SD card root directory for files and stores file names.
  void initFileBrowser();

  // Draw the file list on the OLED display.
  void updateFileListDisplay(Adafruit_SSD1306 &display);

  // Navigation helpers for the file list.
  void nextFile();
  void previousFile();

  // Open the currently selected file and load its contents.
  void openSelectedFile();

  // Draw the contents of the opened file (paged).
  void updateFileViewerDisplay(Adafruit_SSD1306 &display);

  // Page navigation in the file viewer.
  void nextPage();
  void previousPage();

  // Exit file viewing mode and return to the file list.
  void closeFileView();
}

#endif // FILEBROWSER_H
