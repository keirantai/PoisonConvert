//
//    Copyright 2014 CutePoisonX (CutePoisonXI@gmail.com)
//
//    This file is part of PoisonConvert.
//
//    PoisonConvert is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    PoisonConvert is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with PoisonConvert.  If not, see <http://www.gnu.org/licenses/>.
//

#include "SettingsMode.h"
#include "UserInterface.h"
#include "YesNoSetting.h"
#include "FolderSetting.h"
     
SettingsMode::SettingsMode(UserInterface& ui)
: Mode(ui)
{
  //----------------------------------------------------------------------------
  //initialize settings
  settings_vector_.push_back(new Settings(ui, "config-file", "Selecting filename of config - file", 
  "Please enter the filename of the configuration-file (if it does not exist it will be created)", "EmptyConfig"));
  
  settings_vector_.push_back(new FolderSetting(ui, "config-path", "Location of config file",
  "Please enter the path to the configuration-file", "/opt/etc/PoisonConvert_Config-files/"));
    
  settings_vector_.push_back(new YesNoSetting(ui, "delete", "Delete original file after conversion",
  "Do you want to delete the original file after the successful conversion ([y/n])?", "No"));
  
  settings_vector_.push_back(new YesNoSetting(ui, "optimize", "Optimize file for streaming (mp4/m4v/mov only)",
  "Do you want to optimize file for streaming (mov/mp4/m4v only , [y/n])?", "No"));
 
  settings_vector_.push_back(new FolderSetting(ui, "log-path", "Location for saved logfiles",
  "Please enter the path where logfiles should be saved", ""));
  
  settings_vector_.push_back(new FolderSetting(ui, "movies-path", "Where to look for movies",
  "Please enter the path where poisonconvert should look after files", ""));
  
  settings_vector_.push_back(new FolderSetting(ui, "destination", "Where to save processed movies",
  "Please enter the path where poisonconvert should save processed movies", ""));
  //----------------------------------------------------------------------------
}

SettingsMode::SettingsMode(const SettingsMode& orig)
: Mode(orig)
{
}

SettingsMode::~SettingsMode()
{
}

int SettingsMode::executeCommand()
{
  bool endthis = false;
  bool going_back = true;
  bool enter_options_prompt = false;

  do
  {
    if (going_back == true)
    {
      standartExecutePrompt();
      going_back = false;
    }

    input_ = ui_.readStringNoCapitalize();
    enter_options_prompt = true;
    for (unsigned int i = 0; i < settings_vector_.size(); i++)
    {
      if (input_ == settings_vector_.at(i)->getName())
      {
        settings_vector_.at(i)->setParam();
        going_back = true;
        enter_options_prompt = false;
      } else if (input_ == "exit")
      {
        return -1;
      }
    }
    if (enter_options_prompt == true)
    {
      ui_.writeString("Please enter one of the options listed above (without ");
      ui_.writeString("the []).", true);
      endthis = false;
    }
  } while (endthis == false);
}

int SettingsMode::listSettings()
{
  ui_.writeString("--------------------------------------------------------", true);
  for(unsigned int i=0; i < settings_vector_.size(); i++)
  {
    ui_.writeString(" -> ");  ui_.writeString(settings_vector_.at(i)->getDescription()); 
    ui_.writeString(": "); ui_. writeString(settings_vector_.at(i)->getParam(), true, "green");
  }
  ui_.writeString("--------------------------------------------------------", true);
}

void SettingsMode::standartExecutePrompt()
{
  unsigned int align_len;
  
  ui_.writeString("", true);
  ui_.writeString("Settings Mode!", true, "red");
  ui_.writeString("These are your currrent settings:", true);
  listSettings();
  ui_.writeString("Which of them do you want to change?", true);
  
  align_len=IdentifyLongestName();
  
  for(unsigned int i=0; i < settings_vector_.size(); i++)
  {
    ui_.writeString("  [");
    ui_.writeString(settings_vector_.at(i)->getName(), false, "yellow");
    ui_.writeString("] ");
    AlignSpaces(align_len, settings_vector_.at(i)->getName().size());
    ui_.writeString("- ");
    ui_.writeString(settings_vector_.at(i)->getDescription(), true);
  }
  
  ui_.writeString("  ["); ui_.writeString("exit", false, "yellow");
  ui_.writeString("]");
  AlignSpaces(align_len, 4);
  ui_.writeString(" - go back to main screen");
  
  ui_.writeString("", true);
}

void SettingsMode::AlignSpaces(unsigned int align_len,
                               unsigned int name_size)
{
  unsigned int space_difference = align_len - name_size;
  
  for(int i=1; i<=space_difference; i++)
  {
    ui_.writeString(" ");
  }
}

unsigned int SettingsMode::IdentifyLongestName()
{
  unsigned int length = settings_vector_.at(0)->getName().size();

  for (unsigned int i=0; i<settings_vector_.size(); i++)
  {
    //identify longest setting_name
    if (settings_vector_.at(i)->getName().size() > length)
    {
      length = settings_vector_.at(i)->getName().size();
    }
  }
  
  return length;
}
