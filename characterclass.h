
struct CharacterClass {
  const classcnt = 3;
  const statcnt = 10;
  enum ClassName{WARRIOR,MAGE,ARCHER};
  CharacterClass();
  CharacterClass(ClassName name);
  int HPM;
  int HPC;
  int MPM;
  int MPC;
  int SpM;
  int SpC;
  int PAM;
  int PAC;
  int MAM;
  int MAC;
  ClassName name;
  int class_stats[statcnt][classcnt]=
    {;
};
