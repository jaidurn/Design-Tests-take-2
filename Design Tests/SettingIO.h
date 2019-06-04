#pragma once
//==========================================================================================
// File Name: SettingIO.h
// Author: Brian Blackmon
// Date Created: 5/1/2019
// Purpose: 
// Loads in settings from a file.
//==========================================================================================
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <stdio.h>

class SettingIO
{
public:
	enum IO_MODE
	{
		CLOSED = -1,
		READ = 0,
		WRITE = 1
	};

	SettingIO()
		:m_currentFile(""), m_mode(CLOSED)
	{

	}

	~SettingIO()
	{
		if (isOpen())
		{
			close();
		}
		
		m_mode = CLOSED;
	}

	bool isOpen()
	{
		return m_stream.is_open();
	}

	void open(std::string fileName, IO_MODE mode)
	{
		if(!isOpen())
		{
			switch(mode)
			{
			case READ:
			{
				openRead(fileName);
				break;
			}
			case WRITE:
			{
				openWrite(fileName);
				break;
			}
			}

			if(!isOpen())
			{
				std::cout << "Failed to open: '"
					<< fileName
					<< "'!\n";
				m_mode = CLOSED;
			}
			else
			{
				m_currentFile = fileName;
			}
		}
	}

	void close()
	{
		if(isOpen())
		{
			m_stream.close();
		}
	}

	IO_MODE mode() { return m_mode; }
	
	std::string loadSetting(std::string settingName)
	{
		std::string setting = "";

		if (isOpen())
		{
			m_stream.clear();
			m_stream.seekg(0);

			std::string tempSetting = "";
			bool settingFound = false;

			while (m_stream >> tempSetting && !settingFound)
			{
				if (tempSetting == settingName)
				{
					m_stream >> setting;
					settingFound = true;
				}
			}
		}

		return setting;
	}

	template <typename T>
	void saveSetting(std::string settingName, T settingValue)
	{
		open(m_currentFile, WRITE);

		if (isOpen())
		{
			// Create a temporary file
			std::fstream tempSave;

			std::string tempFile = tempFileName();

			tempSave.open(tempFile, std::ios::out);

			if (tempSave.is_open())
			{
				std::string tempSetting = "";
				std::string test = "";
				bool settingFound = false;

				while(std::getline(m_stream, tempSetting))
				{
					if(tempSetting.find(settingName) != -1)
					{
						std::stringstream tempString(tempSetting);

						tempString >> test;

						if(test == settingName)
						{
							tempSave << test;
							tempSave << " ";
							tempSave << settingValue;
							tempSave << "\n";

							settingFound = true;
						}
						
						tempString.flush();
					}
					else
					{
						tempSave << tempSetting;
						tempSave << "\n";
					}
				}

				if (!settingFound)
				{
					tempSave << settingName;
					tempSave << " ";
					tempSave << settingValue;
					tempSave << "\n";
				}

				// Delete the old file and rename the temp one.
				close();
				tempSave.close();

				std::remove(m_currentFile.c_str());
				std::rename(tempFile.c_str(), m_currentFile.c_str());
			}
		}
	}

	bool settingExists(std::string settingName)
	{
		bool settingFound = false;

		if(isOpen())
		{
			m_stream.clear();
			m_stream.seekg(0);

			std::string tempSetting = "";

			while(m_stream >> tempSetting && !settingFound)
			{
				if(tempSetting == settingName)
				{
					settingFound = true;
				}
			}
		}

		return settingFound;
	}

	int settingCount()
	{
		int settingLineCount = 0;

		if (isOpen())
		{
			m_stream.clear();
			m_stream.seekg(0);

			std::string tempSetting = "";

			while(std::getline(m_stream, tempSetting))
			{
				if (tempSetting != "")
				{
					settingLineCount++;
				}
			}
		}

		return settingLineCount;
	}

private:

	void openRead(std::string fileName)
	{
		m_stream.open(fileName, std::ios::in);

		m_mode = READ;
	}

	void openWrite(std::string fileName)
	{
		m_stream.open(fileName, std::ios::in);

		m_mode = WRITE;
	}

	std::string tempFileName()
	{
		std::string tempFile = m_currentFile;
		std::string tempName = "";

		auto sit = tempFile.begin();

		while(sit != tempFile.end())
		{
			if(*sit == '.')
			{
				tempName += "temp";
			}

			tempName += *sit;
			
			sit++;
		}

		return tempName;
	}

	std::fstream m_stream;
	std::string m_currentFile;
	IO_MODE m_mode;
};

