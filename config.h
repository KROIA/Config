#ifndef CONFIG_H
#define CONFIG_H
//              Autor   Alex Krieg
#define  CONFIG_VERSION "00.00.00"
//              Datum   07.10.2018

#include "config_global.h"
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <iostream>

class CONFIGSHARED_EXPORT Config
{
    public:
        Config();
        Config(std::string filename);
        ~Config();

        void filename(std::string filename);
        std::string filename();

        void read();
        void save();

        void fileHead(std::vector<std::string> head);
        std::vector<std::string> fileHead();

        void parameter(std::string parameterName, std::string parameter);
        void parameter(std::string parameterName, std::string &parameter);
        void parameter(std::string parameterName, std::string &parameter, std::string mode);
        std::string line(unsigned int line);
        std::vector<std::string> text();

        std::vector<std::string>    parameterList();
        std::vector<std::string>    parameterValue();
        unsigned int parameters();

        void deleteMultipleParameter(bool doDelete);
        bool deleteMultipleParameter();
    private:
        int getParamRow(std::string paramName);
        void readParameter();
        void writeText();
        int parameterindex(std::string parameterName);

        std::string _filename;
        std::vector<std::string> _fileHead;
        std::vector<std::string> _parameterNameList;
        std::vector<std::string> _parameterList;
        std::vector<std::string> _textList;
        bool    _deleteMultipleParameter;
        bool    _update;

        FILE *_file;
};

#endif // CONFIG_H
