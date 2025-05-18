# Assembly interpreter
Reads a custom Assembly instruction and interprets its instruction in C++

## Architecture
64 bytes of memory, each memory address is 1 byte.
Each register can only hold 1 byte of data
Each flag is 1 bit

## Supported instructions (also yes source and destination is flipped from the usual Assembly)
INPUT [REGISTER DESTINATION] -> Enter a character into a register<br>
DISPLAY [REGISTER SOURCE] -> Displays the value at the source register<br>
MOV [Source], [Destination] -> Stores a value from the destination to the source<br>
ADD [Source], [Destination] -> Adds the value from the destination to the value of the source and store in the destination<br>
SUB [Source], [Destination] -> Subtracts the value from the destination to the value of the source and store in the destination<br>
MUL [Source], [Destination] -> Multiplies the value from the destination to the value of the source and store in the destination<br>
DIV [Source], [Destination] -> Dividess the value from the destination to the value of the source and store in the destination<br>
INC [Destination] -> Adds 1 to the value at destination<br>
DEC [Destination] -> Subtracts 1 to the value at destination<br>
ROL [Destination], [count] -> Rotate the binary value at destination to the left by count values<br>
ROR [Destination], [count] -> Rotate the binary value at destination to the right by count values<br>
SHL [Destination], [count] -> Shift the binary values at destination to the left by count values<br>
SHR [Destination], [count] -> Shift the binary values at destination to the right by count values<br>
LOAD [ADDRESS SOURCE], [Destination] -> Stores value at the source memory address into a destination<br>
STORE [Source], [ADDRESS DESTINATION] -> Stores value at the source into a destination memory address<br>

## Flags
Four flags
1. Underflow (UF)
2. Overflow (OF)
3. Carry Flag (CF)
4. Zero Flag (ZF)

## Credits
This is an assignment for the course CCP6114 - Programming Fundamentals in Multimedia University<br>
Lecturer: Mr. Sharaf El-Deen Sami Mohammed Al-Horani<br>
Tutorial section: TT1L<br>
Group: 03<br>
Group Members:<br>
1. Imran Firdaus Mohd Firdaus (251UC24107)
2. Shawn Huang Qi Yang (251UC240CK)
3. Elsa Zara Binti Fakhurrazi (251UC2502P)
4. Pritaasheniy A/P Ananthan (242UC244QE)
