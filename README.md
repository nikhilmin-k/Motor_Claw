# Motor_Claw

This is Arduino code for a basic robot that uses a stepper motor for the jaw and a servo motor for the arm movement.  A DC motor is used in the project wiring, but does not function properly alongside the servo motor.  

##Controls: potControl is pot controlling the DC motor speed
	buttonPin is a button controlling the DC motor direction
	potControl2 is a pot controlling the stepper motor position
	potControl3 is a pot controlling the servo motor position
	bigbuttonPin starts a cycle of the claw opening and closing, pressing it again makes it stop. 


##Known Issues:

-This code will operate only under USB power, the reason why is currently unknown

-servo used in circuit tested was not powerful enough to lift the arm.  This may need to be adapted for more powerful servos.  

-The stepper motor does not have abosolute positioning, make sure that the jaw of the claw is closed when pressing the button to start the jaw routine.  

Further Information is available at my blog post: https://edblogs.columbia.edu/comsx3930-001-2021-1/?p=825&preview=true
