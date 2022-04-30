# EffectController
Arduino code for controlling model railroad special effects
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
