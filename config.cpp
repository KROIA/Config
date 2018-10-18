#include "config.h"

Config::Config()
{
    Config("");
}
Config::Config(std::string filename)
{
    this->filename(filename);
    _deleteMultipleParameter = false;
    _update                  = true;
}
Config::~Config()
{

}

void Config::filename(std::string filename)
{
    _filename = filename;
}
std::string Config::filename()
{
    return _filename;
}

void Config::read()
{
    _textList = std::vector<std::string>();
    _fileHead = std::vector<std::string>();
    _file = fopen(_filename.c_str(),"r");
    if(!_file)
    {
        //throw std::string("Config::read() Unknown file \""+_filename+"\"");
        qDebug() << "Config::read() Unknown file \""<<QString::fromStdString(_filename)<<"\"";
    }
    else
    {
        char inBuff[255];
        bool readFileHead = true;
        while(!feof(_file))
        {
           for(int a=0; a<255; a++)
           {inBuff[a] = NULL;}
            fgets(inBuff,255,_file);
            std::string buf = inBuff;

            while(buf.find(" ") == 0 || buf.find("\t") == 0)
            {
                buf = buf.substr(1);
            }
            if(buf.find("\n") != -1)
            {
                buf = buf.substr(0,buf.find("\n"));
            }
            if(readFileHead)
            {
                if(buf.find("//") != -1 || buf.find("#") != -1)
                {
                    if(buf.find("//") == 0){buf= buf.substr(2);}else
                    if(buf.find("#") == 0){buf= buf.substr(1);}

                    _fileHead.push_back(buf);
                    continue;
                }else
                {
                    readFileHead = false;
                }
            }
            _textList.push_back(buf);
        }
        fclose(_file);

        readParameter();
    }
}
void Config::save()
{
    if(_update)
    {
        writeText();
    }
    if(_textList.size() == 0)
    {
        return;
    }
    _file = fopen(_filename.c_str(),"w");
    if(!_file)
    {
       // throw std::runtime_error("Config::save() Can't open file \""+_filename+"\"");
    }
    else
    {
        for(unsigned int a=0; a<_fileHead.size(); a++)
        {
            fprintf(_file,"#%s\n",_fileHead[a].c_str());
        }
        for(unsigned int a=0; a<_textList.size(); a++)
        {
            fprintf(_file,"%s",_textList[a].c_str());
            if(a != _textList.size()-1)
            {
                fprintf(_file,"\n");
            }
        }
        fclose(_file);
    }
}
void Config::fileHead(std::vector<std::string> head)
{
    _fileHead = head;
}
std::vector<std::string> Config::fileHead()
{
    return _fileHead;
}
void Config::parameter(std::string parameterName, std::string parameter)
{
    int paramIndex = parameterindex(parameterName);
    if(paramIndex != -1)
    {
        _parameterList[paramIndex] = parameter;
        _update                    = true;
    }
    else
    {
        _parameterNameList.push_back(parameterName);
        _parameterList.push_back(parameter);
        _update = true;
    }
   // writeText();
}
void Config::parameter(std::string parameterName, std::string &parameter)
{
    this->parameter(parameterName,parameter,"r");
}
void Config::parameter(std::string parameterName, std::string &parameter, std::string mode)
{
    if(mode != "r" || mode != "w" || mode != "rw" || mode != "wr")
    {
     //   throw std::runtime_error("Config::parameter("+parameterName+","+parameter+","+mode+") Error: unknown mode: "+mode+" Correct modes are: \"r\" , \"w\" , \"rw\" , \"wr\"");
    }
    int paramIndex = parameterindex(parameterName);
    if(paramIndex != -1)
    {
        if(mode.find("r") != -1)
        {
            parameter = _parameterList[paramIndex];
        }
        if(mode == "w")
        {
            _parameterList[paramIndex] = parameter;
            _update                    = true;
        }
    }
    else
    {
        if(mode.find("w") != -1)
        {
            _parameterNameList.push_back(parameterName);
            _parameterList.push_back(parameter);
            _update = true;
        }
    }
    //writeText();
}
std::string Config::line(unsigned int line)
{
    if(_textList.size() <= line)
    {
       // throw std::runtime_error("Config::line("+std::to_string(line)+") parameter 0 is out of range! command ignored. Maximum is "+std::to_string(_textList.size()-1));
    }
    else
    {
        return _textList[line];
    }
}
std::vector<std::string>    Config::parameterList()
{
    return _parameterNameList;
}
std::vector<std::string>    Config::parameterValue()
{
    return _parameterList;
}
unsigned int Config::parameters()
{
    return _parameterNameList.size();
}
std::vector<std::string> Config::text()
{
    if(_update)
    {
        writeText();
    }
    return _textList;
}
void Config::deleteMultipleParameter(bool doDelete)
{
    _deleteMultipleParameter = doDelete;
}
bool Config::deleteMultipleParameter()
{
    return _deleteMultipleParameter;
}
int Config::getParamRow(std::string paramName)
{
    int row = -1;
    for(unsigned int a=0; a<_textList.size(); a++)
    {
        std::string line = _textList[a];
        if(line.find("#") != -1)
        {
            line = line.substr(0,line.find("#"));
        }
        if(line.find("//") != -1)
        {
            line = line.substr(0,line.find("//"));
        }
        if(line.find(paramName) != -1)
        {
            if(row != -1)
            {
               //   printf("file: %s multiple parameter: %s",_filename.c_str(),paramName.c_str());
                row = -2;
                return row;
            }
            row = a;
        }
    }
    return row;
}
void Config::readParameter()
{
    std::vector<std::string> textList = _textList;
    for(unsigned int a=0; a<textList.size(); a++)
    {
        if(textList[a].find("#") != -1)
        {
            textList[a] = textList[a].substr(0,textList[a].find("#"));
        }
        if(textList[a].find("//") != -1)
        {
            textList[a] = textList[a].substr(0,textList[a].find("//"));
        }
        if(textList[a] != "")
        {
            while(textList[a].find(" ") == 0 || textList[a].find("\t") == 0)
            {
                textList[a] = textList[a].substr(1);
            }
            std::string parameterName = "";
            std::string parameter = "";
            if(textList[a].find(" ") != -1 || textList[a].find("\t") != -1)
            {
              //  parameterName = textList[a].substr(0,textList[a].find("\t"));

                if(textList[a].find(" ") < textList[a].find("\t") && textList[a].find(" ") != -1)
                {
                    parameterName = textList[a].substr(0,textList[a].find(" "));
                    textList[a] = textList[a].substr(textList[a].find(" "));
                }
                else
                {
                    parameterName = textList[a].substr(0,textList[a].find("\t"));
                    textList[a] = textList[a].substr(textList[a].find("\t"));
                }
                while(textList[a].find(" ") == 0 || textList[a].find("\t") == 0)
                {
                    textList[a] = textList[a].substr(1);
                }
                parameter = textList[a].substr(0,textList[a].find("\n"));

            }
            else
            {
                parameterName = textList[a];
            }
            bool save = true;
            for(unsigned int b=0; b<_parameterNameList.size(); b++)
            {
                if(_parameterNameList[b] == parameterName)
                {
                    save = false;
                }
            }
            if(save)
            {
                _parameterNameList.push_back(parameterName);
                _parameterList.push_back(parameter);
            }
        }
    }
}
void Config::writeText()
{
    for(unsigned int a=0; a<_parameterNameList.size(); a++)
    {

        int row = getParamRow(_parameterNameList[a]);
       // printf("row: %i a: %i %s\t%s\n",row,a,_parameterNameList[a].c_str(),_parameterList[a].c_str());
        if(row > -1)
        {
            _textList[row] = _parameterNameList[a] + "\t" + _parameterList[a];
        }
        else if(row == -2)
        {
         //   printf("0\n");
            for(unsigned int b=0; b<_textList.size(); b++)
            {
               // printf("1\n");
                if(_textList[b].find(_parameterNameList[a]) == 0)
                {


                    if(_deleteMultipleParameter == true)
                    {
                        _textList.erase(_textList.begin()+b);
                    }
                    else
                    {
                        _textList[b] = "#multiple_param: "+_textList[b];
                    //    printf("2\t b: %i\ttext: %s\n",b,_textList[b].c_str());
                    }
                }
            }
            _textList.push_back(_parameterNameList[a]+"\t"+_parameterList[a]);
        }else
        {
            _textList.push_back(_parameterNameList[a]+"\t"+_parameterList[a]);
            //printf("push_back: %s\n",_textList[_textList.size()-1].c_str());
        }
    }
    _update = false;
}
int Config::parameterindex(std::string parameterName)
{
    for(unsigned int a=0; a<_parameterNameList.size(); a++)
    {
        if(_parameterNameList[a] == parameterName)
        {
            return a;
        }
    }
    return -1;
}
