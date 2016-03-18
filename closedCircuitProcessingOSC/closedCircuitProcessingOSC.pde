import oscP5.*;        //  Load OSC P5 library
import processing.serial.*;    //  Load serial library

Serial arduinoPort;        //  Set arduinoPort as serial connection
OscP5 oscP5;            //  Set oscP5 as OSC connection

int speed = 500;
int mode = 1;
int intensity = 1;

final int STROBE = 1;
final int CHASE = 2;
final int TWINKLE = 3;

void setup() {
  oscP5 = new OscP5(this, 8000);  // Start oscP5, listening for incoming messages at port 8000}
  for(String thing : Serial.list()) {
    println("serial thing: " + thing);
  }
  arduinoPort = new Serial(this, Serial.list()[1], 9600);    // Set arduino to 9600 baud
}

void oscEvent(OscMessage theOscMessage) {   //  This runs whenever there is a new OSC message

  String addr = theOscMessage.addrPattern();  //  Creates a string out of the OSC message

  if (addr.indexOf("/lasers/speed") != -1) {  
    speed = int(theOscMessage.get(0).floatValue());
    println("speed " + speed);
  }
  if (addr.indexOf("/lasers/mode/1") !=-1) {  
    mode = CHASE;
    println("mode " + mode);
  }
  if (addr.indexOf("/lasers/mode/2") !=-1) {  
    mode = STROBE;
    println("mode " + mode);
  }
  if (addr.indexOf("/lasers/mode/3") !=-1) {  
    mode = TWINKLE;
    println("mode " + mode);
  }
  if (addr.indexOf("/lasers/intensity") !=-1) {  
    intensity = int(theOscMessage.get(0).floatValue());
    println("intensity " + intensity);
  }
}

void draw() {
  writeToArduino();
  //println("speed " + speed);
  String val = arduinoPort.readStringUntil('\n');
  if(val != null) {
    //println("arduino value: " + val);
  }
}

void writeToArduino() {
  arduinoPort.write("T"); // time
  arduinoPort.write(speed);
  arduinoPort.write("M"); // mode
  arduinoPort.write(mode);
  arduinoPort.write("I"); // intensity
  arduinoPort.write(intensity);
}