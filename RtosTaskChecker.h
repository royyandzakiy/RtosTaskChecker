/**
 * RtosTaskChecker.h
 * 
 * This is a simple library that can be used to check the size of tasks within an 
 * RTOS style Arduino Framework based project. Note that this project uses Serial
 * as it's buffer, if your project uses a different buffer please change accordingly
 */

#pragma once
#include <esp_task_wdt.h>

class RtosTaskSizeChecker {
  private:
    UBaseType_t sizeGiven;
  public:
    RtosTaskSizeChecker(int _sizeGiven) {
      // enter the size initially given to the task
      sizeGiven = _sizeGiven;
    }

    UBaseType_t memoryRemaining() {
      return uxTaskGetStackHighWaterMark( NULL );
    }

    UBaseType_t memoryUsed() {
      return sizeGiven - memoryRemaining();
    }

    void printStatus() {
      Serial.println("memoryUsed = " + String((memoryUsed()*100)/sizeGiven) + "% (" + memoryUsed() + "/" + sizeGiven + " bytes); memoryRemining = " + String(memoryRemaining()) + " bytes;");
    }
};

//========== EXAMPLE CODE ============
/*

#include "RtosTaskSizeChecker.h"

//OUTPUT EXAMPLE:
//Setup done
//checkThisTask started; memoryUsed = 54% (656/1200 bytes); memoryRemining = 544 bytes;
//lastCheck = 0 ms; memoryUsed = 82% (988/1200 bytes); memoryRemining = 212 bytes;
//lastCheck = 1028 ms; memoryUsed = 82% (988/1200 bytes); memoryRemining = 212 bytes;
//lastCheck = 2128 ms; memoryUsed = 82% (988/1200 bytes); memoryRemining = 212 bytes;
//lastCheck = 3228 ms; memoryUsed = 82% (988/1200 bytes); memoryRemining = 212 bytes;
//lastCheck = 4328 ms; memoryUsed = 82% (988/1200 bytes); memoryRemining = 212 bytes;
//lastCheck = 5428 ms; memoryUsed = 82% (988/1200 bytes); memoryRemining = 212 bytes;
//lastCheck = 6528 ms; memoryUsed = 82% (988/1200 bytes); memoryRemining = 212 bytes;

// --- TASK TO BE CHECKED ---
void checkThisTask(void*pv) {  
  long lastCheck = 0;
  RtosTaskSizeChecker rtosTaskSizeChecker(1200);

  Serial.print("checkThisTask started; ");
  rtosTaskSizeChecker.printStatus();

  for(;;) {
    if (millis() - lastCheck > 1000) {
      // print the 'remaining' stack memory on the run
      Serial.print("lastCheck = " + String(lastCheck) + " ms; ");
      rtosTaskSizeChecker.printStatus();

      lastCheck = millis();
    }
    vTaskDelay(100 / portTICK_RATE_MS); // don't forget to add this, or else the watch dog will get triggered
  }
  
  vTaskDelete(NULL);
}

// --- MAIN ---
void setup() {
  Serial.begin(115200);
  // 1200 is the number of bytes being allocated, this can be reduced based on the actual 
  // stack usage of this task, this can be found out by checking the remaining memory/stack 
  // using the uxTaskGetStackHighWaterMark function
  xTaskCreate(checkThisTask, "checkThisTask", 1200, NULL, 1, NULL); // will start the lastCheck loop

  Serial.println("Setup done");
}

void loop() {
  vTaskDelete(NULL); // just end the main loop tasks
}

*/