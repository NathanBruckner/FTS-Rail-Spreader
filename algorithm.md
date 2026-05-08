**FTS Rail Gauger**

---
The FTS Rail Gauger Codebase is a modular codebase designed to be easily understood.
The Gauger.ino is the main file, where the looping occurs and where all other functions are called.
Classes for 
1. Button
1. Display
1. Sensor
1. Solenoid
1. Controller
No class inheritance or composition is necessary. Everything runs cleanly in main.
The job of the button, display, sensor, and solenoid classes is to simplify the code to control them and keep it in a tidy place. The buttons needed this class because the physical buttons have memory that need to be reset. Instead of a momentary switch, they are a toggle switch.
The controller controls the logic concerning the interaction between the solenoid and the sensor. This is where the tuning parameters are kept, and where the auto-shutoff logic is found.
Together, these classes work cohesively and as intended.


