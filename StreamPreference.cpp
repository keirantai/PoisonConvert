/* 
 * File:   StreamPreference.cpp
 * Author: CutePoisonX
 * 
 * Created on 04. Februar 2013, 09:40
 */

#include "StreamPreference.h"

StreamPreference::StreamPreference(string param1, string param2, 
                                   string param3, string param4, 
                                   string param5)
: param1_(param1), param2_(param2), param3_(param3), param4_(param4),
  param5_(param5)
{
}

StreamPreference::StreamPreference(const StreamPreference& orig)
: param1_(orig.param1_), param2_(orig.param2_), param3_(orig.param3_), 
  param4_(orig.param4_), param5_(orig.param5_)
{
}

StreamPreference::~StreamPreference()
{
}

int StreamPreference::saveTarget(unsigned int target)
{
  link_target_.push_back(target);
}

int StreamPreference::editTarget(unsigned int new_target,
                                 unsigned int position)
{
  link_target_.at(position) = new_target;
  return 0;
}

//Erases all targets with index = "unsigned int target"
int StreamPreference::deleteAllTargets(unsigned int target)
{
  unsigned int how_many_being_deleted = 0;

  for (unsigned int i = 0; i < link_target_.size(); i++)
  {
    if (link_target_.at(i) == target)
    {
      how_many_being_deleted++;
      //i--;
    }
  }
  if (how_many_being_deleted >= link_target_.size())
  {
    return -1;
  } else
  {
    for (unsigned int i = 0; i < link_target_.size(); i++)
    {
      if (link_target_.at(i) == target)
      {
        vector<unsigned int>::iterator it = link_target_.begin() + i;
        link_target_.erase(it);
        i--;
      }
    }
    return 0;
  }
}
