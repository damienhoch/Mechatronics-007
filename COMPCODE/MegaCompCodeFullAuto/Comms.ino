void comms(bool VERBOSE) {
  if (Serial3.available()) {
    if (VERBOSE) Serial.println("Serial Available");
    if (Serial3.peek() == 255) {
      if (VERBOSE) Serial.println("Start bit received");
      Serial3.read();
      String input = String(Serial3.readStringUntil('\n'));
      if (VERBOSE) Serial.println(input);
      input.trim();                     // Removes extra spaces if any
      int space1 = input.indexOf(' ');  // indexs untill there is a space
      int space2 = input.indexOf(' ', space1 + 1);
      int space3 = input.indexOf(' ', space2 + 1);   // If there is no third space it is equal to -1
      if (space1 > 0 && space2 > 0 && space3 > 0) {  // at least 3 valid inputs
        Command = input.charAt(0);
        if (VERBOSE) Serial.println(Command);
        direction = input.charAt(space1 + 1);
        distAngle = input.substring(space2 + 1, space3).toInt();
        speedRadius = input.substring(space3 + 1).toInt();

      } else if (space1 > 0 && space2 > 0 && space3 == -1) {
        Command = input.charAt(0);
        direction = input.charAt(space1 + 1);
        if (Command == 'U' || Command == 'u') {
          speedRadius = input.substring(space2 + 1).toInt();
        } else {
          distAngle = input.substring(space2 + 1).toInt();
          speedRadius = 0;  // default to 0 when not provided
        }
      } else if (space1 > 0){
        Command = input.charAt(0);
        direction = input.charAt(space1 + 1);
      }else Command = input.charAt(0);  // If less than 2 inputs, stop
      tStart = micros() / 1000000.;
    } else {
      Serial3.read();
    }
  }
}