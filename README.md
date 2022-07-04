# RtosTaskChecker

This is a simple library that can be used to check the size of tasks within an RTOS style Arduino Framework based project. 

A use case of mine is to define the optimal stack size when initially setting up an xTaskCreate statement, hence reducing the chance of stack overflow, yet not using too much memory.

Note: that this project uses Serial as it's buffer, if your project uses a different buffer please change accordingly

Here is The Example Code that you can run directly by creating a new project on the Arduino IDE
```
#include "RtosTaskSizeChecker.h"

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

```

Here is the Example Output
```
Setup done
checkThisTask started; memoryUsed = 54% (656/1200 bytes); memoryRemining = 544 bytes;
lastCheck = 0 ms; memoryUsed = 82% (988/1200 bytes); memoryRemining = 212 bytes;
lastCheck = 1028 ms; memoryUsed = 82% (988/1200 bytes); memoryRemining = 212 bytes;
lastCheck = 2128 ms; memoryUsed = 82% (988/1200 bytes); memoryRemining = 212 bytes;
lastCheck = 3228 ms; memoryUsed = 82% (988/1200 bytes); memoryRemining = 212 bytes;
lastCheck = 4328 ms; memoryUsed = 82% (988/1200 bytes); memoryRemining = 212 bytes;
lastCheck = 5428 ms; memoryUsed = 82% (988/1200 bytes); memoryRemining = 212 bytes;
lastCheck = 6528 ms; memoryUsed = 82% (988/1200 bytes); memoryRemining = 212 bytes;
```