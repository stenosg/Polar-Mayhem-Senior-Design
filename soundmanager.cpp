#include "soundmanager.h"

ALboolean SoundManager::loadALData()
{
      // Variables to load into.

  ALenum format;
  ALsizei size;
  ALvoid* data;
  ALsizei freq;
  ALboolean loop;

   // Load wav data into a buffer.

  alGenBuffers(1, &Buffer);

  if(alGetError() != AL_NO_ERROR)
    return AL_FALSE;

  alutLoadWAVFile((ALbyte*)file.c_str(), &format, &data, &size, &freq, &loop);
  alBufferData(Buffer, format, data, size, freq);
  alutUnloadWAV(format, data, size, freq);

      // Bind the buffer with the source.

  alGenSources(1, &Source);

  if(alGetError() != AL_NO_ERROR)
  {
    std::cerr<<"Error loading data."<<std::endl;
    return AL_FALSE;
  }

  alSourcei (Source, AL_BUFFER,   Buffer   );
  alSourcef (Source, AL_PITCH,    1.0      );
  alSourcef (Source, AL_GAIN,     2.0      );
  alSourcefv(Source, AL_POSITION, SourcePos);
  alSourcefv(Source, AL_VELOCITY, SourceVel);
  alSourcei (Source, AL_LOOPING,  loop     );

      // Do another error check and return.

  if(alGetError() == AL_NO_ERROR)
  {
    std::cerr<<"Successfully loaded data."<<std::endl;
    return AL_TRUE;
  }

  std::cerr<<"Error loading data."<<std::endl;

  return AL_FALSE;
}
/*
 * void SetListenerValues()
 *
 *  We already defined certain values for the Listener, but we need
 *  to tell OpenAL to use that data. This function does just that.
 */
 void SoundManager::setListenerValues()
 {
  alListenerfv(AL_POSITION,    ListenerPos);
  alListenerfv(AL_VELOCITY,    ListenerVel);
  alListenerfv(AL_ORIENTATION, ListenerOri);
}



    /*
 * void KillALData()
 *
 *  We have allocated memory for our buffers and sources which needs
 *  to be returned to the system. This function frees that memory.
 */
 void SoundManager::killALData()
 {
  initialized = false;
  alDeleteBuffers(1, &Buffer);
  alDeleteSources(1, &Source);
}

SoundManager::SoundManager():file(""),playing(false),initialized(0)
{
  SourcePos[0] = 0.0;
  SourcePos[1] = 0.0;
  SourcePos[2] = 0.0;
  // Velocity of the source sound.
  SourceVel[0] = 0.0;
  SourceVel[1] = 0.0;
  SourceVel[2] = 0.0;
  // Position of the Listener.
  ListenerPos[0] = 0.0;
  ListenerPos[1] = 0.0;
  ListenerPos[2] = 0.0;
  // Velocity of the Listener.
  ListenerVel[0] = 0.0;
  ListenerVel[1] = 0.0;
  ListenerVel[2] = 0.0;
    // Orientation of the Listener. (first 3 elements are "at", second 3 are "up")
    // Also note that these should be units of '1'.

  ListenerOri[0] = 0.0;
  ListenerOri[1] = 0.0;
  ListenerOri[2] = -1.0;
  ListenerOri[3] = 0.0;
  ListenerOri[4] = 1.0;
  ListenerOri[5] = 0.0;

}
bool SoundManager::initialize(std::string fn)
{
  if(file == fn)
  {
    std::cerr<<"Error already initialized this file"<<std::endl;
    return false;
  }
  if(initialized)
  {
    killALData();
  }
  file = fn;
  // Initialize OpenAL and clear the error bit.
  alutInit(NULL, 0);
  alGetError();
  // Load the wav data.
  if(loadALData() == AL_FALSE)
  {
    return false;
  }
  setListenerValues();
  initialized = true;
  return true;

}

bool SoundManager::play()
{
  if(!playing)
  {
    alSourcePlay(Source);
    playing = true;
  }
}
bool SoundManager::pause()
{
  alSourcePause(Source);
  playing = false;
}
bool SoundManager::stop()
{
  alSourceStop(Source);
  playing = false;
}
void SoundManager::setLooping(bool loop)
{
  alSourcei (Source, AL_LOOPING,  loop);
}