#ifndef _SOUND_MANAGER_H_
#define _SOUND_MANAGER_H_

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>
#include <string>
#include <iostream>

class SoundManager{

  private:
    // Position of the source sound.
    ALfloat SourcePos[3];
    // Velocity of the source sound.
    ALfloat SourceVel[3];
    // Position of the Listener.
    ALfloat ListenerPos[3];
    // Velocity of the Listener.
    ALfloat ListenerVel[3];
    // Orientation of the Listener. (first 3 elements are "at", second 3 are "up")
    // Also note that these should be units of '1'.
    ALfloat ListenerOri[6];
    ALboolean playing;
    // Buffers to hold sound data.
    ALuint Buffer;
    // Sources are points of emitting sound.
    ALuint Source;
    
    ALboolean initialized;
    std::string file;
  
    ALboolean loadALData();
    void setListenerValues();
    void killALData();

  public:
    SoundManager();
    bool initialize(std::string);
    bool play();
    bool pause();
    bool stop();
    void setLooping(bool loop);


};

#endif
