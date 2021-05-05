-	Description:

     Password Based Door Lock Security System based on 2 ATmega32, the first connects with the "HMI" to interface with user, the second controls and saves and gives instructions to other components.

-	Operating mechanism:

	At first, the user should enter the desired user 2 times to create his password. If the entered passwords are matched, the UART of the 1st microcontroller send the password to the UART of 2nd microcontroller and then the EEPROM will save it which this EEPROM is attached with the 2nd microcontroller. If the entered passwords aren’t matched, the user should repeat this operation again until he entered 2 passwords which are matched. (This is the 1st operation).

	After the desired password is saved successfully, the user has choice to choose between opening the door or changing his current password with new one.

 	If the user choose to open the door, the user will enter his password again and then the door will open by the DC Motor is rotating for 15 sec clockwise and after 30 sec the door will begin closing automatically by rotating the DC Motor for 15 sec counterclockwise. If the user entered the password wrongly for 3 times, the led will be on acting like a buzzer.

	If the user chooses to change his password, the user will enter his password again and then the user will repeat all the 1st operation again.

-  Drivers designed and implemented for the project:
  Timer, UART, I2C, External EEPROM, Keypad, LCD and DC Motor.
  Drivers are implemented using configuration technique "Static and Dynamic" to support different modes in Atmega16. Different design methods are used like interrupts, call backs, tight and periodic polling.
