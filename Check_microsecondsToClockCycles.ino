void setup() {
  Serial.begin(38400);

#define _F_CPU 18432000

// Arduino style
#define _clockCyclesPerMicrosecond_() ( _F_CPU / 1000000L )
#define _microsecondsToClockCycles_(a) ( (a) * _clockCyclesPerMicrosecond_() )

  Serial.print(F("Demo clock freq: "));
  Serial.println( _F_CPU );

  Serial.print(F("Arduino style clockcyclesPerMicrosecond: "));
  Serial.println( _clockCyclesPerMicrosecond_()  );
  
  Serial.println(F("Arduino style microsecondsToClockCycles:"));
  Serial.print(F("3 - "));
  Serial.print( _microsecondsToClockCycles_(3) );
  Serial.print(F(" ;   777 - "));
  Serial.println( _microsecondsToClockCycles_(777) );
  Serial.println();

  Serial.println(F("SAMD core style:"));
  Serial.print(F("3 - "));
  Serial.print( (3) * (_F_CPU / 1000000L) );
  Serial.print(F(" ;   777 - "));
  Serial.println( (777) * (_F_CPU / 1000000L) );
  Serial.println();


  Serial.println(F("Microcore style (the best):"));
  Serial.print(F("3 - "));
  Serial.print( ((3) * (_F_CPU / 1000L)) / 1000L );
  Serial.print(F(" ;   777 - "));
  Serial.println( ((777) * (_F_CPU / 1000L)) / 1000L );
  Serial.println();


  Serial.println(F("Could be most accurate, but compiler owerflow:"));
  Serial.print(F("3 - "));
  Serial.print( ((_F_CPU * 3) / 1000000L) );
  Serial.print(F(" ;   777 - "));
  Serial.println( ((_F_CPU * 777) / 1000000L) );
  Serial.println();
}

void loop() {}
