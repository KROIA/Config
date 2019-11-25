#include "config.h"

Config::Config()
{
    Config("");
}
Config::Config(QString filename)
{
    this->filename(filename);
    _deleteMultipleParameter = false;
    _update                  = true;
    this->resetCommentPrefixList();
}
Config::~Config()
{

}

const QString Config::version()
{
    return QString(CONFIG_VERSION);
}
const QString Config::autor()
{
    return QString(CONFIG_AUTOR);
}

void Config::filename(QString filename)
{
    _filename = filename;
}
QString Config::filename()
{
    return _filename;
}

void Config::read()
{    
    _textList.clear();
    _fileHead.clear();

    QFile file(_filename);
    bool readFileHead = true;

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Config::read() Can't open file \""<<_filename<<"\"";
        return;
    }
    QTextStream stream(&file);
    QString line;
    while (stream.readLineInto(&line)) {

        while(line.indexOf(" ") == 0 || line.indexOf("\t") == 0)
        {
            line = line.mid(1);
        }
        if(line.indexOf("\n") != -1)
        {
            line = line.mid(0,line.indexOf("\n"));
        }
        if(readFileHead)
        {
            for(int commandPrefix=0; commandPrefix<_commentPrefixList.size(); commandPrefix++)
            {
                if(line.indexOf(_commentPrefixList[commandPrefix]) != -1)
                {
                    line = line.mid(0,line.indexOf(_commentPrefixList[commandPrefix]));
                    _fileHead.push_back(line);
                    break;
                }
                else
                {
                    readFileHead = false;
                }
            }
        }
        _textList.push_back(line);
    }
    file.close();
    readParameter();
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

    QFile file(_filename);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "Config::save() Can't open file \""<<_filename<<"\"";
        return;
    }
    for(int line=0; line<_fileHead.size(); line++)
    {
        /*
            Nur eine neue Zeile wenn nicht schon eine da ist und
            es nicht die letzte Zeile ist.
         */
        if(line == _fileHead.size()-1 || _fileHead[line].indexOf("\n") != -1)
        {
            continue;
        }
        _fileHead[line]+= "\n";
    }
    for(int line=0; line<_textList.size(); line++)
    {
        /*
            Nur eine neue Zeile wenn nicht schon eine da ist und
            es nicht die letzte Zeile ist.
         */
        if(line == _textList.size()-1 || _textList[line].indexOf("\n") != -1)
        {
            continue;
        }
        _textList[line]+= "\n";
    }
    for(int a=0; a<_fileHead.size(); a++)
    {
        //fprintf(_file,"%s%s\n",_commentPrefixList[0].c_str(),_fileHead[a].c_str());
        file.write((_commentPrefixList[0]+_fileHead[a]).toStdString().c_str());
    }
    for(int a=0; a<_textList.size(); a++)
    {
        //fprintf(_file,"%s",_textList[a].c_str());
        file.write(_textList[a].toStdString().c_str());
    }
    file.close();
    /*

    _file = fopen(_filename.c_str(),"w");
    if(!_file)
    {
       // throw std::runtime_error("Config::save() Can't open file \""+_filename+"\"");
    }
    else
    {
        for(unsigned int a=0; a<_fileHead.size(); a++)
        {
            fprintf(_file,"%s%s\n",_commentPrefixList[0].c_str(),_fileHead[a].c_str());
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
    }*/
}
void Config::fileHead(QStringList head)
{
    _fileHead = head;
}
QStringList Config::fileHead()
{
    return _fileHead;
}
void Config::parameter(QString parameterName, QString parameter)
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
void Config::parameter(QString parameterName, QString &parameter)
{
    this->parameter(parameterName,parameter,"r");
}
void Config::parameter(QString parameterName, QString &parameter, QString mode)
{
    if(mode != "r" || mode != "w" || mode != "rw" || mode != "wr")
    {
     //   throw std::runtime_error("Config::parameter("+parameterName+","+parameter+","+mode+") Error: unknown mode: "+mode+" Correct modes are: \"r\" , \"w\" , \"rw\" , \"wr\"");
    }

    int paramIndex = parameterindex(parameterName);
   // std::cout <<"name "<<parameterName.toStdString() << " "<<paramIndex<<"\n";
    if(paramIndex != -1)
    {
        if(mode.indexOf("r") != -1)
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
        //std::cout <<"cant find "<<parameterName.toStdString() << "\n";
        if(mode.indexOf("w") != -1)
        {
            _parameterNameList.push_back(parameterName);
            _parameterList.push_back(parameter);
            _update = true;
        }
    }
    //writeText();
}
void Config::parameterShort(QString parameterName, short &parameter, QString mode)
{
    QString tmp = QString::number(parameter);
    this->parameter(parameterName,tmp,mode);
    parameter = tmp.toShort();
}
void Config::parameterUShort(QString parameterName, unsigned short &parameter, QString mode)
{
    QString tmp = QString::number(parameter);
    this->parameter(parameterName,tmp,mode);
    parameter = tmp.toUShort();
}
void Config::parameterInt(QString parameterName, int &parameter, QString mode)
{
    QString tmp = QString::number(parameter);
    this->parameter(parameterName,tmp,mode);
    parameter = tmp.toInt();
}
void Config::parameterUInt(QString parameterName, unsigned int &parameter, QString mode)
{
    QString tmp = QString::number(parameter);
    this->parameter(parameterName,tmp,mode);
    parameter = tmp.toUInt();
}
void Config::parameterDouble(QString parameterName, double &parameter, QString mode)
{
    QString tmp = QString::number(parameter);
    this->parameter(parameterName,tmp,mode);
    parameter = tmp.toDouble();
}
void Config::parameterBool(QString parameterName, bool &parameter, QString mode)
{
    QString tmp = "false";
    if(parameter)
        tmp = "true";
    this->parameter(parameterName,tmp,mode);

    if(tmp.indexOf("true") != -1 || tmp.indexOf("TRUE") != -1 || tmp == "1")
        parameter = true;
    else
        parameter = false;
}
QString Config::line(int line)
{
    if(_textList.size() <= line || line < 0)
    {
        qDebug() << "Config::line(int ["<<line<<"]) paramerer 0 out of boundry: min: 0 max:"<<_textList.size()-1;
        return "";
       // throw std::runtime_error("Config::line("+std::to_string(line)+") parameter 0 is out of range! command ignored. Maximum is "+std::to_string(_textList.size()-1));
    }
    else
    {
        return _textList[line];
    }
}
QStringList    Config::parameterList()
{
    return _parameterNameList;
}
QStringList    Config::parameterValue()
{
    return _parameterList;
}
int Config::parameters()
{
    return _parameterNameList.size();
}
QStringList Config::text()
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
void Config::resetCommentPrefixList()
{
    _commentPrefixList.clear();
    this->addCommentPrefix(CONFIG_DEFAULT_COMMENT_PREFIX);
}
void Config::addCommentPrefix(QString prefix)
{
    for(int a=0; a<_commentPrefixList.size(); a++)
    {
        if(_commentPrefixList[a] == prefix)
        {
            return;
        }
    }
    _commentPrefixList.push_back(prefix);
}
void Config::deleteCommentPrefix(QString prefix)
{
    if(prefix == CONFIG_DEFAULT_COMMENT_PREFIX)
        return;
    for(int a=0; a<_commentPrefixList.size(); a++)
    {
        if(_commentPrefixList[a] == prefix)
        {
            _commentPrefixList.erase(_commentPrefixList.begin()+a);
            return;
        }
    }
}
void Config::commentPrefixList(QStringList prefixList)
{
    _commentPrefixList = prefixList;
}
QStringList    Config::commentPrefixList()
{
    return _commentPrefixList;
}

int Config::getParamRow(QString paramName)
{
    int row = -1;
    for(int a=0; a<_textList.size(); a++)
    {
        QString line = _textList[a];
        for(int commandPrefix=0; commandPrefix<_commentPrefixList.size(); commandPrefix++)
        {
            if(line.indexOf(_commentPrefixList[commandPrefix]) != -1)
            {
                line = line.mid(0,line.indexOf(_commentPrefixList[commandPrefix]));
            }
        }
        if(line.indexOf(paramName) != -1)
        {
            if(row != -1)
            {
                qDebug() <<"file: "<<_filename<<" multiple parameter: "<<paramName;
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
    QStringList textList = _textList;
    for(int a=0; a<textList.size(); a++)
    {
        for(int commandPrefix=0; commandPrefix<_commentPrefixList.size(); commandPrefix++)
        {
            if(textList[a].indexOf(_commentPrefixList[commandPrefix]) != -1)
            {
                textList[a] = textList[a].mid(0,textList[a].indexOf(_commentPrefixList[commandPrefix]));
            }
        }
        if(textList[a] != "")
        {
            textList[a].replace("\t"," ");
            while(textList[a].indexOf(" ") == 0)
            {
                textList[a] = textList[a].mid(1);
            }
            QString parameterName = "";
            QString parameter = "";
            if(textList[a].indexOf(" ") != -1)
            {
              //  parameterName = textList[a].substr(0,textList[a].find("\t"));

                if(textList[a].indexOf(" ") != -1)
                {
                    parameterName = textList[a].mid(0,textList[a].indexOf(" "));
                    textList[a] = textList[a].mid(textList[a].indexOf(" "));
                }
                while(textList[a].indexOf(" ") == 0)
                {
                    textList[a] = textList[a].mid(1);
                }
                parameter = textList[a].mid(0,textList[a].indexOf("\n"));

            }
            else
            {
                parameterName = textList[a];
            }
            bool save = true;
            for(int b=0; b<_parameterNameList.size(); b++)
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
    for(int a=0; a<_parameterNameList.size(); a++)
    {
        int row = getParamRow(_parameterNameList[a]);
        if(row > -1)
        {
            _textList[row] = _parameterNameList[a] + "\t" + _parameterList[a];
        }
        else if(row == -2)
        {
            for(int b=0; b<_textList.size(); b++)
            {
                if(_textList[b].indexOf(_parameterNameList[a]) == 0)
                {
                    if(_deleteMultipleParameter == true)
                    {
                        _textList.erase(_textList.begin()+b);
                    }
                    else
                    {
                        _textList[b] = _commentPrefixList[0]+"multiple_param: "+_textList[b];
                    }
                }
            }
            _textList.push_back(_parameterNameList[a]+"\t"+_parameterList[a]);
        }else
        {
            _textList.push_back(_parameterNameList[a]+"\t"+_parameterList[a]);
        }
    }
    _update = false;
}
int Config::parameterindex(QString parameterName)
{
    for(int a=0; a<_parameterNameList.size(); a++)
    {
        if(_parameterNameList[a] == parameterName)
        {
            return a;
        }
    }
    /*std::cout <<"Parameterlist\n";
    for(int a=0; a<_parameterNameList.size(); a++)
    {
        std::cout<<_parameterNameList[a].toStdString()<<"\n";
    }
    getchar();*/
    return -1;
}
