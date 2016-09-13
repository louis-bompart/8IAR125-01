#ifndef MESSAGE_TYPES
#define MESSAGE_TYPES

#include <string>

enum message_type
{
  Msg_HiHoneyImHome,
  Msg_StewReady,
  Msg_BobIsInTheBar,
  Msg_EndOfFight,
  Msg_LouisIsAngry
};


inline std::string MsgToStr(int msg)
{
  switch (msg)
  {
  case Msg_HiHoneyImHome:
    
    return "HiHoneyImHome"; 

  case Msg_StewReady:
    
    return "StewReady";

  case Msg_BobIsInTheBar:

	  return "BobIsInTheBar";

  case Msg_EndOfFight:

	  return "EndOfFight";

  case Msg_LouisIsAngry:

	  return "LouisIsAngry";

  default:

    return "Not recognized!";
  }
}

#endif