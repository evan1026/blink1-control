Structure
=========

## Thread 1
* CommandManager runs commands and based on outputs determines what patterns should be active and sends that to the PatternManager
* PatternManager generates super-pattern
    * Loops through all active patterns to see which are compatible with each other
        * Two patterns are compatible if neither uses LED 0 and if the sets of their LEDs don't intersect
    * Based on pattern compatibility, generate groups of patterns that can run together
        * Try to maximize similarity between the pattern lengths
        * Potentially also look at patterns which have similar lengths when one is repeated some number of times
    * For each group generate a super-pattern
        * In code, just a regular pattern
        * Created by combining the commands of each of the patterns into a single pattern that is equivalent to running
          all patterns in the group at the same time
    * Send all super-patterns to the other thread to be played

## Thread 2
* PatternPlayer loops through list of patterns in order and plays them, similar to what main.cpp does now
* When a new set of patterns comes in, it is buffered until we finish the current pattern
* Once finished, we pull in the new set of patterns and start playing them
    * Potentially try to find what the next pattern would have been in the new list and start playing from there

## Misc
* Make sure to turn device off on exit (set LED0 to #000000)
    * I guess we could do that in the destructor? But I'm not sure I want to to that in the library so maybe
      I'll make a derived class for a blink1 device that turns it of when destructed
