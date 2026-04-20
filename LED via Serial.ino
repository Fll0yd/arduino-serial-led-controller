const int LED_PIN = 13;

bool ledState = false;
unsigned long bootTime = 0;

String readCommand() {
  if (!Serial.available()) {
    return "";
  }

  String command = Serial.readStringUntil('\n');
  command.trim();
  command.toUpperCase();
  return command;
}

void setLed(bool state) {
  ledState = state;
  digitalWrite(LED_PIN, ledState ? HIGH : LOW);
}

void printStatus() {
  Serial.print("OK: LED is ");
  Serial.print(ledState ? "ON" : "OFF");
  Serial.print(" | Uptime(ms): ");
  Serial.println(millis() - bootTime);
}

void printHelp() {
  Serial.println("Available commands:");
  Serial.println("  ON      - Turn LED on");
  Serial.println("  OFF     - Turn LED off");
  Serial.println("  STATUS  - Show LED state and uptime");
  Serial.println("  BLINK   - Blink LED 5 times");
  Serial.println("  HELP    - Show this help message");
}

void blinkLed(int times, int onMs, int offMs) {
  for (int i = 0; i < times; i++) {
    digitalWrite(LED_PIN, HIGH);
    delay(onMs);
    digitalWrite(LED_PIN, LOW);
    delay(offMs);
  }

  // Restore previous tracked state after blinking
  digitalWrite(LED_PIN, ledState ? HIGH : LOW);
}

void handleCommand(const String& command) {
  if (command.length() == 0) {
    return;
  }

  if (command == "ON") {
    setLed(true);
    Serial.println("OK: LED turned ON");
  }
  else if (command == "OFF") {
    setLed(false);
    Serial.println("OK: LED turned OFF");
  }
  else if (command == "STATUS") {
    printStatus();
  }
  else if (command == "BLINK") {
    Serial.println("OK: Blinking LED");
    blinkLed(5, 200, 200);
    Serial.println("OK: Blink sequence complete");
  }
  else if (command == "HELP") {
    printHelp();
  }
  else {
    Serial.print("ERROR: Unknown command -> ");
    Serial.println(command);
    Serial.println("Type HELP for available commands.");
  }
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  setLed(false);

  Serial.begin(9600);
  bootTime = millis();

  Serial.println("Serial LED Controller Ready");
  Serial.println("Type HELP for available commands.");
}

void loop() {
  String command = readCommand();
  handleCommand(command);
}
