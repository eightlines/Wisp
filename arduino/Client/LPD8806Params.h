/* 
 * array stores 16 parameters per particle. In order these represent:
 * particle active (0 or 1)
 * particle starting position (0 to 31, assuming a 32 LED strip)
 * rotation direction (1 = clockwise, 0 = ccw)
 * particle speed (Lower is faster. 1 means the particle moves on every frame update. 4 would be a movement every fourth frame.)
 * red val (127 max)
 * green val
 * blue val
 * brightness 
 * leading corona length
 * trailing corona length 
 * 6 empty slots, for future expansion
*/

int particleParams1[] = {
    1, 0, 0, 2, 66, 66, 66, 127, 1, 1, 0, 0, 0, 0, 0, 0,  
    1, 0, 1, 2, 66, 66, 66, 127, 1, 1, 0, 0, 0, 0, 0, 0,  
    0, 0, 0, 4, 127, 0, 0, 127, 0, 1, 0, 0, 0, 0, 0, 0,  
    0, 0, 1, 4, 0, 0, 127, 127, 0, 1, 0, 0, 0, 0, 0, 0,  
    0, 0, 1, 2, 127, 127, 127, 127, 0, 1, 0, 0, 0, 0, 0, 0
};

int particleParams2[] = {
    1, 0, 0, 4, 43, 43, 43, 127, 1, 1, 0, 0, 0, 0, 0, 0,  
    1, 0, 1, 4, 0, 0, 127, 127, 1, 1, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 2, 66, 66, 66, 127, 0, 1, 0, 0, 0, 0, 0, 0,  
    0, 0, 1, 2, 66, 66, 66, 127, 0, 1, 0, 0, 0, 0, 0, 0,  
    0, 0, 1, 2, 127, 127, 127, 127, 0, 1, 0, 0, 0, 0, 0, 0
};

int particleParams3[] = {
    1, 0, 0, 2, 0, 0, 127, 127, 0, 0, 0, 0, 0, 0, 0, 0,  
    0, 0, 1, 2, 66, 66, 66, 127, 0, 1, 0, 0, 0, 0, 0, 0,  
    0, 0, 0, 4, 127, 0, 0, 127, 0, 1, 0, 0, 0, 0, 0, 0,  
    0, 0, 1, 4, 0, 0, 127, 127, 0, 1, 0, 0, 0, 0, 0, 0,  
    0, 0, 1, 2, 127, 127, 127, 127, 0, 1, 0, 0, 0, 0, 0, 0
};

int particleParams4[] = {
    1, 0, 0, 2, 43, 43, 43, 127, 0, 1, 0, 0, 0, 0, 0, 0,  
    1, 11, 0, 2, 0, 66, 66, 127, 1, 1, 0, 0, 0, 0, 0, 0,  
    1, 22, 0, 2, 0, 0, 127, 127, 0, 1, 0, 0, 0, 0, 0, 0,  
    0, 0, 1, 4, 0, 0, 127, 127, 1, 1, 0, 0, 0, 0, 0, 0,  
    0, 0, 1, 2, 127, 127, 127, 127, 0, 1, 0, 0, 0, 0, 0, 0
};

int particleParams5[] = {
    1, 0, 0, 2, 66, 66, 66, 127, 1, 1, 0, 0, 0, 0, 0, 0,  
    1, 16, 0, 2, 66, 66, 66, 127, 0, 1, 0, 0, 0, 0, 0, 0,  
    0, 0, 0, 4, 127, 0, 0, 127, 1, 1, 0, 0, 0, 0, 0, 0,  
    0, 0, 1, 4, 0, 0, 127, 127, 0, 1, 0, 0, 0, 0, 0, 0,  
    0, 0, 1, 2, 127, 127, 127, 127, 0, 1, 0, 0, 0, 0, 0, 0
};

int activeParticleParams[80];

/* arrays store xx parameters defining a ring pattern. Params are:
 * style - 0: pulse
 * rate
 * red val (127 max)
 * green val
 * blue val
 * brightness
 * 6 empty slots, for future expansion 
*/
int ringParams1[] = {1, 32, 43, 43, 43, 127, 0, 0, 0, 0, 0, 0};
int ringParams2[] = {1, 32, 0, 66, 66, 127, 0, 0, 0, 0, 0, 0};
int ringParams3[] = {0, 64, 0, 66, 66, 127, 0, 0, 0, 0, 0, 0};
int ringParams4[] = {1, 32, 0, 0, 127, 127, 0, 0, 0, 0, 0, 0};
int ringParams5[] = {1, 64, 0, 0, 127, 127, 0, 0, 0, 0, 0, 0};

int activeRingParams[12];
