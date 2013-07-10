/* 
 * File:   main.cpp
 * Author: CutePoisonX
 *
 * Created on 03. Februar 2013, 23:23
 */

#include <cstdlib>
#include <string>
#include "VectorSourceManager.h"
#include "UserInterface.h"
#include "ConfigMode.h"
#include "FileManager.h"
#include "FileReadException.h"
#include "SettingsMode.h"
#include "StartMode.h"
#include "AnalyzeMedia.h"
#include "Settings.h"
#include "YesNoSetting.h"

 
using namespace std;

const string version = "Version: 1.3";

int main(int argc, char** argv)
{
  string input;
  string startargument = " ";
  string filenameargument = " ";
  bool going_back = true;
  bool next_file = false;
  bool settings_fail = false;
  
  if(argv[1] != 0)
    startargument = argv[1];
  if(argv[2] != 0)
    filenameargument = argv[2];
  
  UserInterface ui;
  VectorSourceManager vecman;
  AnalyzeMedia analyze;
  ConfigMode configmode(ui, vecman);
  SettingsMode settingsmode(ui);
  
  FileManager filemanager(settingsmode, vecman, analyze);
  StartMode startmode(ui, vecman, filemanager, analyze, settingsmode);

  
  try
  {
    filemanager.readSettings();
  } catch (OpenFileException)
  {
    ui.writeString("Can not open settings-file. Please check if file is present in /opt/etc and filename is \"PoisonConvert_Settings\".", true, "red");
    ui.writeString("Please check settings-menu.", true, "red");
    ui.readString(false);
  } catch (exception)
  {
    ui.writeString("There occured an error at reading out the \"Settings\"-file! Please enter settings.", true, "red");
    ui.writeString("Please check settings-menu.", true, "red");
    ui.readString(false);
  }
  
  if(startargument == "version")
  {
    ui.writeString(version, true, "green"); 
    return 0;
  }
  if (startargument != "start")
  {
    try
    {
      filemanager.readPreferences();
    } catch (OpenFileException)
    {
      ui.writeString("Can not open Configuration-file! Please check if file is present and you entered filename and path correctly.", true, "red");
      ui.writeString("Please go to settings-menu and enter correct settings.", true, "red");
      vecman.clearAllInstances();
      ui.readString(false);
    } catch (exception)
    {
      ui.writeString("There occured an error at reading out the configuration-file! Seems, that it is corrupt.", true, "red");
      ui.writeString("Loaded without any configuration.", true, "red");
      vecman.clearAllInstances();
      ui.readString(false);
    }
    system("clear");
    ui.writeString("", true);
    ui.writeString("**************************************************************", true);
    ui.writeString("****************** ");
    ui.writeString("WELCOME TO POISONCONVERT", false, "green");
    ui.writeString(" ******************", true);
    ui.writeString("**************************************************************", true);
    ui.writeString("", true);

    while (input != "exit")
    {
      if (going_back == true)
      {
        ui.writeString("Please select one of the following options:", true, "red");
        ui.writeString("  [");
        ui.writeString("start", false, "blue");
        ui.writeString("]    - start conversion with custom settings.", true);
        ui.writeString("  [");
        ui.writeString("config", false, "blue");
        ui.writeString("]   - videostream management.", true);
        ui.writeString("  [");
        ui.writeString("settings", false, "blue");
        ui.writeString("] - enter some settings.", true);
        ui.writeString("  [");
        ui.writeString("exit", false, "blue");
        ui.writeString("]     - quit program.", true);
        ui.writeString("", true);
        going_back = false;
      }
      input = ui.readStringNoCapitalize();
      if (input == "start")
      {
        settings_fail = false;
        for (int i = 0; i < settingsmode.getVectorLen(); i++)
        {
          if (settingsmode.getSettingsParam(i) == "")
          {
            ui.writeString("Empty setting: ", false, "red");
            ui.writeString(settingsmode.getSettingsName(i), true, "red");
            settings_fail = true;
          }
        }

        if (vecman.getVectorLen(SRCVIDEO) != 0 || vecman.getVectorLen(SRCAUDIO) != 0 ||
            vecman.getVectorLen(SRCSUB) != 0)
        {
          if (settings_fail == false)
          {
            try
            {
              startmode.executeCommand();
            } catch (exception)
            {
              system("clear");
              ui.writeString("It seems that there occured an unknown error. Please report it, so we can fix this bug together.", true, "red");
              analyze.clearAllInstances();
              vecman.clearAllInstances();
              return -1;
            }
            going_back = true;
          }
        } else
        {
          ui.writeString("You have to specify rules first. Go to 'config - menu' to do so.", true, "green");
          ui.readString(false);
          going_back = false;
        }
        if (settings_fail == true)
        {
          ui.writeString("Please enter listed setting(s). Go to 'settings - menu' to do so.", true, "green");
          ui.readString(false);
          going_back = false;
        }
      } else if (input == "config")
      {
        try
        {
          if( filemanager.checkPathToConfig() == "" )
          {
            ui.writeString("Warning: You did not specify a path to the config file in settings. Changes will not be saved", true, "red");
            ui.readString(false);
          }
          configmode.executeCommand();
          if(filemanager.savePreferencesToFile() == -1)
          {
            ui.writeString("Did not save changes because you did not specify a path to a config file in settings.", true, "red");
            ui.readString(false);
          }
        } catch (FileWriteException)
        {
          ui.writeString("Can not write configuration-file! Please check if poisonconvert has enough rights to do so.", true, "red");
        } catch (exception)
        {
          system("clear");
          ui.writeString("It seems that there occured an unknown error. Please report it, so we can fix this bug together.", true, "red");
          analyze.clearAllInstances();
          vecman.clearAllInstances();
          return -1;
        }
        going_back = true;
      } else if (input == "settings")
      {
        try
        {
          settingsmode.executeCommand();
          filemanager.saveSettingsToFile();
        } catch (FileWriteException)
        {
          ui.writeString("Can not write settings-file! Please check if poisonconvert has enough rights to do so.", true, "red");
          ui.readString(false);
        } catch (exception)
        {
          system("clear");
          ui.writeString("It seems that there occured an unknown error. Please report it, so we can fix this bug together.", true, "red");
          analyze.clearAllInstances();
          vecman.clearAllInstances();
          return -1;
        }
        try
        {
          vecman.clearAllInstances();
          filemanager.readPreferences();
        } catch (OpenFileException)
        {
          ui.writeString("Created configuration-file.", true, "red");
          ui.readString(false);
        } catch (exception)
        {
          ui.writeString("There occured an error at reading out the configuration-file! Seems, that it is corrupt.", true, "red");
          ui.writeString("Loaded without any configuration.", true, "red");
          vecman.clearAllInstances();
          ui.readString(false);
        }
        going_back = true;
      } else if (input != "exit")
      {
        ui.writeString("Please enter one of the listed options!", true);
      }
    }
  } else
  {
    if(argc > 2) //that means that the user specified one or more filenames
    {
      next_file = false;
      for (unsigned int arg = 2; arg < argc; arg++)
      {
        filenameargument = argv[arg];
        settingsmode.writeParam(filenameargument, CONFIGNAME);

        try
        {
          filemanager.readPreferences();
        } catch (OpenFileException)
        {
          ui.writeString("Can not open Configuration-file: ", false, "red");
          ui.writeString(filenameargument, true, "red");
          vecman.clearAllInstances();
          next_file = true;
        } catch (exception)
        {
          ui.writeString("There occured an error at reading out the configuration-file: ", false, "red");
          ui.writeString(filenameargument, true, "red");
          vecman.clearAllInstances();
          next_file = true;
        }
        if (next_file == false);
        {
          if (vecman.getVectorLen(SRCVIDEO) != 0 || vecman.getVectorLen(SRCAUDIO) != 0 ||
              vecman.getVectorLen(SRCSUB) != 0)
          {
            try
            {
              startmode.executeCommand();
              analyze.clearAllInstances();
              vecman.clearAllInstances();
            } catch (exception)
            {
              system("clear");
              ui.writeString("It seems that there occured an unknown error. Please report it, so we can fix this bug together.", true, "red");
              analyze.clearAllInstances();
              vecman.clearAllInstances();
              return -1;
            }
          } else
          {
            ui.writeString("Cannot start. You have to specify rules first.", true, "red");
            analyze.clearAllInstances();
            vecman.clearAllInstances();
            return -1;
          }
        }
      }
    } else //the user only specified "start" - so we are loading the default config-file
    {
      try
      {
        filemanager.readPreferences();
      } catch (OpenFileException)
      {
        ui.writeString("Can not open Configuration-file! Please check if file is present and you entered filename and path correctly.", true, "red");
        ui.writeString("Loaded without any configuration.", true, "red");
        vecman.clearAllInstances();
        ui.readString(false);
      } catch (exception)
      {
        ui.writeString("There occured an error at reading out the configuration-file! Seems, that it is corrupt.", true, "red");
        ui.writeString("Loaded without any configuration.", true, "red");
        vecman.clearAllInstances();
        ui.readString(false);
      }
      if (vecman.getVectorLen(SRCVIDEO) != 0 || vecman.getVectorLen(SRCAUDIO) != 0 ||
          vecman.getVectorLen(SRCSUB) != 0)
      {
        try
        {
          startmode.executeCommand();
          analyze.clearAllInstances();
          vecman.clearAllInstances();
        } catch (exception)
        {
          system("clear");
          ui.writeString("It seems that there occured an unknown error. Please report it, so we can fix this bug together.", true);
          analyze.clearAllInstances();
          vecman.clearAllInstances();
          return -1;
        }
      } else
      {
        ui.writeString("Cannot start. You have to specify rules first.", true, "red");
        analyze.clearAllInstances();
        vecman.clearAllInstances();
        return -1;
      }
    }
  }
  analyze.clearAllInstances();
  vecman.clearAllInstances();
  return 0;
}
