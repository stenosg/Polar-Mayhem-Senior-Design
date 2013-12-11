#include "stats.h"
#include "unit.h"
#include <vector>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <iostream>
const int numStats = 12;
const int RawSp = 98304;
const int RawPA = 81920;
const int RawMA = 81920;
const int maxRawHP = 524287;
const int maxRawMP = 262143;
const int minRawHP = 491520;
const int minRawMP = 245760;
const int statDivider = 1638400;

Stats::Stats(){ Stats(WARRIOR);}

Stats::Stats(ClassName characterClass)
  :atkRange(1),speedCnt(0),characterClass(characterClass)
{
  
  std::ifstream file;
  std::string line;
  file.open("stats");
  std::cout<<"CLASS: "<<characterClass<<std::endl;
  if(!file.is_open())std::cerr<<"COULD NOT OPEN FILE"<<std::endl;//open a file
  for(int i = 0; i <= characterClass; ++i){
    getline(file,line);
  }
  file.close();				//close it  
  std::stringstream ss(line);
  std::cout<<line<<std::endl;
  ss>>move;  
  ss>>jump;
  ss>>HPM;
  ss>>HPC; 
  ss>>MPM;
  ss>>MPC;
  ss>>SpM;
  ss>>SpC; 
  ss>>PAM;
  ss>>PAC; 
  ss>>MAM;
  ss>>MAC;
  
  RawHP = (rand()%(maxRawHP-minRawHP+1))+minRawHP;
  RawMP = (rand()%(maxRawMP-minRawMP+1))+minRawMP;

  HP = (RawHP * HPM)/statDivider;
  MP = (RawMP * MPM)/statDivider;
  PA = (RawPA * PAM)/statDivider;
  MA = (RawMA * MAM)/statDivider;
  Sp = (RawSp * SpM)/statDivider;
  currentHP = HP;
  currentMP = MP;
}

void Stats::reset()
{
  currentHP = HP;
  currentMP = MP;
}

void Stats::Spell(const Ability & ab)
{
  Ability a = ab;
  if(a.isInstant()){
    //calculate damage
    currentHP += a.HPMod;
    currentMP += a.MPMod;
    
  }
  else{
    statusEffects.push_back(a);
    Sp += a.SpMod;
    PA += a.PAMod;
    MA += a.MAMod;
    currentHP += a.HPMod;
    currentMP += a.MPMod;
      

  }
  
}
void Stats::updateStatusEffects(int ticks)
{
  for (int i = 0; i<statusEffects.size();++i){
    Ability e = statusEffects[i];
    e.update(ticks);
    if(e.isFinished()){
      Sp += -e.SpMod;
      PA += -e.PAMod;
      MA += -e.MAMod;
    }
    else{
      currentHP += e.HPMod;
      currentMP += e.MPMod;
      
    }
    
  }
  
}
