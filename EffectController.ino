// EffectController
//
// controls Arduino pins/relays which control effects such as lights or sounds
// a table of effects defines
// -- pin for output
// -- when activated, how long the pin should stay high
// -- after the pin was high, how long it should stay low
// -- in the round-robin processing of the effects, the probability it will be activated
//
// at the start of each cycle we generate a random probability (0-99)
// and continue scanning the table of effects
// don't allow more than MaxActives pins high at any time
// 
// *************************************************************************
// Revision history:
//   2022/02/14 J. Schmidt 1.1 Add FectOff pin control
//   2022/02/06 J. Schmidt 1.0 
//   2022/02/03 J. Schmidt 0.1b Start
// *************************************************************************
//
// *************************************************************************
// USER TUNEABLE Defines
// *************************************************************************
// Define the number of effects
#define NumFects 9
// Define the maximum actives
#define MaxActives 4
// delay in seconds for main loop sampling 
#define MainLoopDlySecs 5
// Define the probability base
#define ProbBase 100
// ****************** Trace & Debug
// 0 - no trace; 1 - events; 
#define Trace 0
// ******************* Variable definitions
typedef struct {
    int Dpin;            // pin number for effect
    int FectOff;         // value for turning effect off
    long int ActveSecs;  // time the effect is up
    long int NActveSecs; // time the pin is idle
    int Problty;         // probability to activate pin
} FectDef;
//
// DEFINE YOUR EFFECTS HERE
FectDef FectDat [NumFects] = {
//     Dpin,  FectOff,  ActveSecs, NActveSecs, Problty
       2,     LOW,      20,       30,          50, // cows
       3,     LOW,      30,       40,          60, // siren
       4,     HIGH,     40,       30,          20, // factory
       5,     LOW,      20,       40,          30, // whistle
       6,     LOW,      30,       30,          50, // hens
       7,     HIGH,     40,       40,          80, // telegraph
       8,     LOW,      20,       30,          40, // car
       9,     LOW,      30,       40,          70, // truck
       10,    HIGH,     10,       10,          70  // thing
};
//
unsigned long ActveTM [NumFects];
unsigned long IdleTM  [NumFects];
//
int Fidx, Lidx, RandFact;
int Actives;
//
// *************************************************************************
void setup() {
// *************************************************************************
  // begin setup
  #if Trace > 0
    Serial.begin(9600);
    Serial.print  ("******* Startup Trace = ");
    Serial.println(Trace);
    #endif
  for (Fidx = 0; Fidx < NumFects; ++Fidx){
    ActveTM[Fidx] = 0;
    IdleTM [Fidx] = 0;
    pinMode     (FectDat[Fidx].Dpin, OUTPUT);
    digitalWrite(FectDat[Fidx].Dpin, FectDat[Fidx].FectOff);
    } // for
  randomSeed(millis());
  Fidx = 0;
}

// *************************************************************************
void loop() {
// *************************************************************************
  // mail loop code
  Actives = 0;
  // check effect going inactive, and count
  for (Lidx = 0; Lidx < NumFects; ++Lidx){
    if (ActveTM[Lidx] > millis()){
      ++Actives;
      continue;} // not yet expired
      else
      {// active expired
        if (ActveTM[Lidx] > 0 && IdleTM [Lidx] == 0){
          // Active just expired
          digitalWrite(FectDat[Lidx].Dpin, FectDat[Fidx].FectOff);
          IdleTM [Lidx] = millis() + FectDat[Lidx].NActveSecs * 1000;
          ActveTM[Lidx] = 0;
          #if Trace > 0
            Serial.print  ("Pin ");
            Serial.print  (FectDat[Lidx].Dpin);
            Serial.print  (" expired idle until ");
            Serial.print  (IdleTM [Lidx]);
            Serial.print  (" ");
            Serial.println(millis());
            #endif
        } // Active just expired 
      }// not active
    } // for Lidx
          #if Trace > 0
            Serial.print  ("Actives ");
            Serial.print  (Actives);
            Serial.print  (" at ");
            Serial.println(millis());
            #endif
  
  for (Lidx = 0; Lidx < NumFects; ++Lidx){
    ++Fidx;
    if (Fidx >= NumFects) {Fidx = 0;}
    if (Actives >= MaxActives){break;};
    if (ActveTM[Fidx] > 0)       {continue;}// still active
    if (IdleTM [Fidx] > millis()){continue;}// not yet expired
    if (IdleTM [Fidx] > 0){// idle just expired
          #if Trace > 0 
            Serial.print  ("Pin ");
            Serial.print  (FectDat[Fidx].Dpin);
            Serial.print  (" idle expired ");
            Serial.print  (IdleTM [Fidx]);
            Serial.print  (" ");
            Serial.println(millis()); 
            #endif
      IdleTM [Fidx] = 0;
    }// idle just expired
    else
    {// been idle
    RandFact = random(ProbBase);
        #if Trace > 0
          Serial.print  ("Pin ");
          Serial.print  (FectDat[Fidx].Dpin);
          Serial.print  (" Random/Probability ");
          Serial.print  (RandFact);
          Serial.print  ("/");
          Serial.print  (FectDat[Fidx].Problty);
          #endif
    if (RandFact <= FectDat[Fidx].Problty){// activate
      digitalWrite(FectDat[Fidx].Dpin, (FectDat[Fidx].FectOff == LOW)?HIGH:LOW);
      ActveTM[Fidx] = millis() + FectDat[Fidx].ActveSecs * 1000;
      ++Actives;
          #if Trace > 0
            Serial.print  (" activated until ");
            Serial.print  (ActveTM [Fidx]);
            #endif
    } // if threshold
          #if Trace > 0
            Serial.print  (" ");
            Serial.println(millis());
            #endif
    } // been idle 
    } // for Lidx
delay(MainLoopDlySecs * 1000);
} // end loop
