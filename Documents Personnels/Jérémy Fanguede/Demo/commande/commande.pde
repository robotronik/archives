
import processing.serial.*;

Serial myPort;  // Create object from Serial class  
 
char curKey = 0;
 
void setup() 
{
  size(640, 360);
  noStroke();
  background(0);
  
  String portName = Serial.list()[1];
  myPort = new Serial(this, portName, 115200);
  print(portName);
  print("\n");
}

void draw() 
{ 
  // keep draw() here to continue looping while waiting for keys
}

void keyPressed() 
{

  if (key == 'z' || key == 'Z') //avancer
  {
    if (curKey != 'z')
    {
      curKey = 'z';
      print("z (avancer)\n");
      myPort.write('z');
    }
  } 
  else if (key == 's' || key == 'S') //reculer
  {
    if (curKey != 's')
    {
      curKey = 's';
      print("s (reculer)\n");
      myPort.write('j');
    }
  }
  else if (key == 'q' || key == 'Q') //gauche
  {
    if (curKey != 'q')
    {
      curKey = 'q';
      print("q (gauche)\n");
      myPort.write('q');
    }
  }
  else if (key == 'd' || key == 'D') //droite
  {
    if (curKey != 'd')
    {
      curKey = 'd';
      print("d (droite)\n");
      myPort.write('i');
    }
  }
  else if (key == 'j' || key == 'J') //servo gauche
  {
    //if (curKey != 'j')
    //{
      //curKey = 'j';
      print("j (svg)\n");
      myPort.write('u');
    //}
  }
  else if (key == 'k' || key == 'K') //servo gauche
  {
    //if (curKey != 'k')
    //{
      //curKey = 'k';
      print("k (svg)\n");
      myPort.write('v');
    //}
  }
  else if (key == 'l' || key == 'L') //servo droit
  {
    //if (curKey != 'l')
    //{
      //curKey = 'l';
      print("l (svd)\n");
      myPort.write('w');
    //}
  }
  else if (key == 'm' || key == 'M') //servo droit
  {
    //if (curKey != 'm')
    //{
      //curKey = 'm';
      print("m (svd)\n");
      myPort.write('x');
    //}
  }
  else if (key == 'p' || key == 'P')
  {
    if (curKey != 'p')
    {
      curKey = 'p';
      print("p (automatique)\n");
      myPort.write('H');
    }
  }
  else
  {
    print("not a valid key\n");
  }

}

void keyReleased() 
{
  if (key == 'z' || key == 'Z' || key == 's' || key == 'S' || key == 'q' || key == 'Q' || key == 'd' || key == 'D' || key == 'p' || key == 'P')
  {
    curKey = 0;
    print("(stop)\n");
    myPort.write('k');
  } 

}
