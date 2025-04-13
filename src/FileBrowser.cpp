#include "FileBrowser.h"
#include <SD.h>
#include "Config.h"

namespace FileBrowser {

  BrowserState browserState = FILE_LIST;

  String fileNames[MAX_FILES];
  int fileCount = 0;
  int selectedFileIndex = 0;

  String fileLines[MAX_FILE_LINES];
  int fileLineCount = 0;
  int currentPage = 0;

  // Scan the SD card for files in the root directory.
  void initFileBrowser() {
    fileCount = 0;
    File root = SD.open("/");
    if (!root) {
      Serial.println("Failed to open root directory");
      return;
    }
    // Loop through all entries in the root directory.
    while (true) {
      File entry = root.openNextFile();
      if (!entry) break; // No more files.
      if (!entry.isDirectory() && fileCount < MAX_FILES) {
        fileNames[fileCount] = String(entry.name());
        fileCount++;
      }
      entry.close();
    }
    root.close();
    selectedFileIndex = 0;
    browserState = FILE_LIST;
  }

  // Display the list of files on the OLED.
  void updateFileListDisplay(Adafruit_SSD1306 &display) {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("SD Files:");
  
    // effectiveCount is the number of files plus one "Back" option.
    int effectiveCount = fileCount + 1; 
    
    // Determine paging window based on effectiveCount instead of fileCount.
    int startIndex = 0;
    if (effectiveCount > LINES_PER_PAGE) {
      startIndex = (selectedFileIndex / LINES_PER_PAGE) * LINES_PER_PAGE;
    }
    int endIndex = startIndex + LINES_PER_PAGE;
    if (endIndex > effectiveCount) {
      endIndex = effectiveCount;
    }
    
    // Display each item
    for (int i = startIndex; i < endIndex; i++) {
      // Print the selection marker
      if (i == selectedFileIndex)
        display.print("> ");
      else
        display.print("  ");
        
      // If this index equals fileCount, then it is the "Back" option
      if (i == fileCount) {
        display.println("Back");
      } else {
        display.println(fileNames[i]);
      }
    }
    display.display();
  }
  

  void nextFile() {
    int effectiveCount = fileCount + 1;  // file names plus "Back"
    if (effectiveCount == 0) return;
    selectedFileIndex = (selectedFileIndex + 1) % effectiveCount;
  }
  
  void previousFile() {
    int effectiveCount = fileCount + 1;
    if (effectiveCount == 0) return;
    selectedFileIndex = (selectedFileIndex - 1 + effectiveCount) % effectiveCount;
  }
  

// Open the selected file and read its lines into fileLines[].
void openSelectedFile() {
  // If the "Back" option is selected, do not open a file.
  if (selectedFileIndex == fileCount) {
    Serial.println("Back selected. Exiting File Browser...");
    // Optionally, you can reset the state here (or let InputHandler handle a state change).
    return;
  }
  
  String fname = fileNames[selectedFileIndex];
  // Ensure absolute path:
  if (!fname.startsWith("/")) {
    fname = "/" + fname;
  }
  
  File file = SD.open(fname);
  if (!file) {
    Serial.println("Failed to open file: " + fname);
    return;
  }
  fileLineCount = 0;
  while (file.available() && fileLineCount < MAX_FILE_LINES) {
    fileLines[fileLineCount] = file.readStringUntil('\n');
    fileLineCount++;
  }
  file.close();
  currentPage = 0;
  browserState = FILE_VIEW;
}


  // Display a page of file contents on the OLED.
  void updateFileViewerDisplay(Adafruit_SSD1306 &display) {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("Viewing:");
    display.println(fileNames[selectedFileIndex]);
    display.println("----------------");

    int startLine = currentPage * LINES_PER_PAGE;
    int endLine = startLine + LINES_PER_PAGE;
    if (endLine > fileLineCount) {
      endLine = fileLineCount;
    }
    for (int i = startLine; i < endLine; i++) {
      display.println(fileLines[i]);
    }
    // Display current page indicator
    display.setCursor(0, SCREEN_HEIGHT - 10);
    display.print("Pg ");
    display.print(currentPage + 1);
    display.print(" / ");
    int totalPages = (fileLineCount + LINES_PER_PAGE - 1) / LINES_PER_PAGE;
    display.print(totalPages);
    display.display();
  }

  // Move to the next page of file content, if available.
  void nextPage() {
    int totalPages = (fileLineCount + LINES_PER_PAGE - 1) / LINES_PER_PAGE;
    if (currentPage < totalPages - 1) {
      currentPage++;
    }
  }

  // Move to the previous page of file content.
  void previousPage() {
    if (currentPage > 0) {
      currentPage--;
    }
  }

  // Exit the file view mode and return to the file list.
  void closeFileView() {
    browserState = FILE_LIST;
  }
}
