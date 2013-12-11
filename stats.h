#ifndef _STATS_H_
#define _STATS_H_
#include <vector>
#include "ability.h"
//#include "item.h"


class Stats{
 public:
  enum ClassName{WARRIOR,MAGE,ARCHER};
  Stats();

  Stats(ClassName characterClass); 
  std::vector<Ability> statusEffects;
  void reset();
  void updateStatusEffects(int ticks);
  
  //bool Attack(Stats s, Weapon * w);
  void Spell(const Ability &);
  

  ClassName characterClass;
  int RawHP;
  int RawMP;
  
  int currentHP;
  int currentMP;
  int HP;
  int PA;
  int MA;
  int MP;
  int atkRange;
  int Sp;
  int speedCnt;

  // class specific
  //Mutlipliers(M) and Increments(C) 
  int move;  
  int jump;

  int HPM; //HP
  int HPC; 
  int MPM; //MP
  int MPC;
  int SpM; //Speed
  int SpC; 
  int PAM; //Physical Attack
  int PAC; 
  int MAM; //Magic Attack
  int MAC;
  
  
};

#endif
