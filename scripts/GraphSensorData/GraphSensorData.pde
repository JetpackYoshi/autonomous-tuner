import processing.serial.*;
import java.nio.ByteBuffer;

Serial myPort;  // Create object from Serial class
PFont font;
float[] xvals;
float[] yvals;
int arrayindex = 0;
float[] sensorData = {0.0, 0.0};
float tC = 0.0;
float tF = 0.0;
float tCK = 0.0;
float tFK = 0.0;

void setup() 
{
  size(640, 640);
  noSmooth();
  myPort = new Serial(this, "COM7", 115200);
  //font = loadFont("CourierNewPS-BoldMT-32.vlw");
  //textFont(font);
  
  xvals = new float[width];
  yvals = new float[width];
}

void draw() {
  background(0);
      
  if(myPort.available() > 0) {
    String inString = myPort.readStringUntil((int) '\n');
    
    if(inString != null && inString.length() == 20) {
      //println("   data len = " + inString.length());
      sensorData = decodeFloat(inString.substring(0, 18)); // discard the '\r''\n'
      
      //tC = 100.0*sensorData[0] - 50;
      //tF = tC * 1.8 + 32.0;
      //tCK = 100.0*sensorData[1] - 50;
      //tFK = tCK * 1.8 + 32.0;
      
      tF = sensorData[0];
      tFK = sensorData[1];
  
      println(sensorData[0] + ", " + sensorData[1] + " --> " + tF + "°, " + tFK + "°");
    }
  }
  
  for(int i = 1; i < width; i++) { 
    xvals[i-1] = xvals[i]; 
    yvals[i-1] = yvals[i];
  } 
  
  // Add the new values to the end of the array 
  xvals[width-1] = map(sensorData[0], 0.7, 0.75, height, 0); 
  yvals[width-1] = map(sensorData[1], 0.7, 0.75, height, 0);

  for(int i=1; i<width; i++) {
    stroke(255, 0, 0);
    line(i-1, xvals[i-1], i, xvals[i]);
    stroke(0, 0, 255);
    line(i-1, yvals[i-1], i, yvals[i]);
  }  
  
  fill(255, 0, 0);
  text("   Raw: " + nf(tF,2,3) + "°", 30, 40);
  fill(0, 0, 255);
  text("Kalman: " + nf(tFK,2,3) + "°", 30, 70);
}

float[] decodeFloat(String inString) {
  float [] outData = new float[2];
  byte [] inData = new byte[4];
  
  String tempString = inString.substring(1, 9); // discard the leading "#"
  inData[0] = (byte) unhex(tempString.substring(0, 2));
  inData[1] = (byte) unhex(tempString.substring(2, 4));
  inData[2] = (byte) unhex(tempString.substring(4, 6));
  inData[3] = (byte) unhex(tempString.substring(6, 8));      
  int intbits = (inData[3] << 24) | ((inData[2] & 0xff) << 16) | ((inData[1] & 0xff) << 8) | (inData[0] & 0xff);
  outData[0] = Float.intBitsToFloat(intbits);
  
  tempString = inString.substring(10, 18); // discard the leading "#"
  inData[0] = (byte) unhex(tempString.substring(0, 2));
  inData[1] = (byte) unhex(tempString.substring(2, 4));
  inData[2] = (byte) unhex(tempString.substring(4, 6));
  inData[3] = (byte) unhex(tempString.substring(6, 8));      
  intbits = (inData[3] << 24) | ((inData[2] & 0xff) << 16) | ((inData[1] & 0xff) << 8) | (inData[0] & 0xff);
  outData[1] = Float.intBitsToFloat(intbits);
  
  return outData;
}
