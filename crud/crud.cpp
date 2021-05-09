#include <iostream>
#include <fstream>
#include <cctype>
#include <string>
#include <vector>
#include <map>
#include "definitions.h"

class crud
{
public:
    crud()
    {
        system("clear");
        start();
    }
public:
    void start()
    {
        do{
            std::cout << "slav@slav-Q551LN" << ">";
            getline(std::cin, query);
            commandLineLength = query.length();
            command = GetCommandName();
            if(!CheckCommand())
            {
                continue;
            }
        } while(!over);
    }
private:
    std::string GetCommandName()
    {
        while(isalpha(query[++currIndex]) && (currIndex < commandLineLength)){}
        return query.substr(0, currIndex);
    }

    bool CheckCommand()
    {
        if(command == NEW_TYPE_COMMAND)
        {
            CreateCommand();
            return true;
        }
        if(command == EXIT_COMMAND)
        {
            return (over = true);
        }
        if(command == CLEAR_COMMAND)
        {
            system("clear");
            Reset();
            return true;
        }
        if(command == NEW_TYPE_CONSTRUCT_COMMAND)
        {
            ConstructCommand();
            return true;
        }
        if(command == GET_WHERE_COMMAND)
        {
            GetWhereCommand();
            return true;
        }
        std::cout << INVALID_COMMAND << std::endl;
        Restart();
        return false;
    }

    //start of main commands
    void CreateCommand()
    {
        while(isblank(query[++currIndex]) && currIndex < commandLineLength) {}
        int startPos = currIndex;
        while(isalnum(query[++currIndex]) && currIndex < commandLineLength) {}
        CheckRange();
        std::string newTypeName = query.substr(startPos, currIndex - startPos);
        if(CheckTypeNameExistance(newTypeName))
        {
            std::cout << EXISTING_TYPE << std::endl;
            Restart();
        }
        while(query[++currIndex] != '{' && currIndex < commandLineLength) {}
        std::map<std::string, std::string> dataforCurrentType;
        bool done = false;
        do{
            while(!isalpha(query[++currIndex]) && currIndex < commandLineLength) {}
            int keyStartPos = currIndex;
            --currIndex;
            while(isalnum(query[++currIndex]) && currIndex < commandLineLength) {}
            CheckRange();
            std::string key = query.substr(keyStartPos, currIndex - keyStartPos);
            dataforCurrentType.insert(std::pair<std::string, std::string>(key, "#"));
            --currIndex;
            while(query[++currIndex] != ',' && currIndex < commandLineLength) {}
            if(query[currIndex] == '}' || currIndex == commandLineLength)
            {
                done = true;
            }
        } while(!done);
        std::vector<std::map<std::string, std::string>> pairForCurrentDataType;
        pairForCurrentDataType.push_back(dataforCurrentType);
        pairsForEachDataType.push_back(pairForCurrentDataType);
        Reset();
        std::cout << "OK" << std::endl;
    }

    void ConstructCommand()
    {
        while(isblank(query[++currIndex]) && currIndex < commandLineLength) {}
        int typeStartPos = currIndex;
        while(isalnum(query[++currIndex]) && currIndex < commandLineLength) {}
        CheckRange();
        std::string existingTypeName = query.substr(typeStartPos, currIndex - typeStartPos);
        if(!CheckTypeNameExistance(existingTypeName))
        {
            std::cout << NON_EXISTING_TYPE << std::endl;
            Restart();
        }
        int ix = GetTypeNameIndex(existingTypeName);
        std::vector<std::map<std::string, std::string>> currTypePairs = pairsForEachDataType[ix];
        std::map<std::string, std::string> currTypePair;
        std::ofstream currTypeInFile;
        currTypeInFile.open((existingTypeName + ".txt"), std::ios::app);
        bool done = false;
        do{
            while(!isalpha(query[++currIndex]) && currIndex < commandLineLength) {}
            int keyStartPos = currIndex;
            --currIndex;
            while(isalnum(query[++currIndex]) && currIndex < commandLineLength) {}
            CheckRange();
            std::string key = query.substr(keyStartPos, currIndex - keyStartPos);
            if(currTypePairs[0].find(key) == currTypePairs[0].end())
            {
                std::cout << key << NON_EXISTING_KEY << std::endl;
                Restart();
            }
            --currIndex;
            while(query[++currIndex] != '=' && currIndex < commandLineLength) {}
            --currIndex;
            while(!isalnum(query[++currIndex]) && currIndex < commandLineLength) {}
            int valueStartPos = currIndex;
            while(isalnum(query[++currIndex]) && currIndex < commandLineLength) {}
            CheckRange();
            std::string valueForCurrKey = query.substr(valueStartPos, currIndex - valueStartPos);
            --currIndex;
            currTypePair.insert(std::pair<std::string, std::string>(key, valueForCurrKey));
            currTypeInFile << key << ": " << valueForCurrKey << '\n';
            while(query[++currIndex] != ',' && currIndex < commandLineLength) {}
            if(query[currIndex] == '}' || currIndex == commandLineLength)
            {
                done = true;
            }
        } while(!done);
        currTypeInFile << SEPARATOR;
        currTypeInFile.close();
        pairsForEachDataType[ix].push_back(currTypePair);
        Reset();
        /*std::fstream currTypeInFile(existingTypeName + ".txt");
        for(std::vector<std::map<std::string, std::string>>::iterator it = pairsForEachDataType[ix].begin(); it != pairsForEachDataType[ix].end(); ++it)
        {
            currTypeInFile << *it->first << ": " << *it->second << '\n';
        }*/
        std::cout << "OK" << std::endl;
    }

    void GetWhereCommand()
    {
        while(isblank(query[++currIndex]) && currIndex < commandLineLength) {}
        int typeStartPos = currIndex;
        while(isalnum(query[++currIndex]) && currIndex < commandLineLength) {}
        CheckRange();
        std::string existingTypeName = query.substr(typeStartPos, currIndex - typeStartPos);
        if(!CheckTypeNameExistance(existingTypeName))
        {
            std::cout << NON_EXISTING_TYPE << std::endl;
            Restart();
        }
        int ix = GetTypeNameIndex(existingTypeName);
        std::vector<std::map<std::string, std::string>> currTypePairs = pairsForEachDataType[ix];
        while(!isalpha(query[++currIndex]) && currIndex < commandLineLength) {}
        int keyNameStartPos = currIndex;
        while(isalnum(query[++currIndex]) && currIndex < commandLineLength) {}
        CheckRange();
        std::string keyName = query.substr(keyNameStartPos, currIndex - keyNameStartPos);
        if(currTypePairs[0].find(keyName) == currTypePairs[0].end())
        {
            std::cout << keyName << NON_EXISTING_KEY << std::endl;
            Restart();
        }
        --currIndex;
        while(query[++currIndex] != '=' && currIndex < commandLineLength) {}
        while(!isalnum(query[++currIndex]) && currIndex < commandLineLength) {}
        int valStartPos = currIndex;
        while(isalnum(query[++currIndex]) && currIndex < commandLineLength) {}
        CheckRange();
        std::string value = query.substr(valStartPos, currIndex - valStartPos);
        int size = currTypePairs.size();
        int count = 0;
        std::ofstream result("getWhere_" + existingTypeName + "_" + keyName + "_:_" + value + ".txt", std::ios::app);
        for(int i = 1; i < size; ++i)
        {
            /*std::map<std::string, std::string>::iterator it = currTypePairs[i].find(keyName);
            std::cout << it->first << it->second;
            if(value == it->second)
            {
                printMap(currTypePairs[i]);
            }*/
            for(std::map<std::string, std::string>::iterator it = currTypePairs[i].begin(); it != currTypePairs[i].end(); ++it)
            {
                if(it->first == keyName && it->second == value)
                {
                    //printMap(currTypePairs[i]);
                    for(auto it = currTypePairs[i].begin(); it != currTypePairs[i].end(); ++it)
                    {
                        result << it->first << ": " << it->second << '\n';
                    }
                    result << SEPARATOR;
                }
            }
        }
        std::cout << "OK" << std::endl;
        Reset();
    }
    //end of main commands

    void printMap(const std::map<std::string, std::string>& m)
    {
        int size = m.size();
        for(std::map<std::string, std::string>::const_iterator it = m.begin(); it != m.end(); ++it)
        {
            std::cout << it->first << ": " << it->second << std::endl;
        }
        std::cout << SEPARATOR;
    }
    bool CheckTypeNameExistance(const std::string& newTypeName)
    {
        int typesSize = userDefinedTypes.size();
        for(int i = 0; i < typesSize; ++i)
        {
            if(newTypeName == userDefinedTypes[i])
            {
                return true;
            }
        }
        userDefinedTypes.push_back(newTypeName);
        std::ofstream newFile;
        newFile.open(newTypeName + ".txt");
        return false;
    }

    int GetTypeNameIndex(const std::string& existingTypename)
    {
        int size = userDefinedTypes.size();
        for(int i = 0; i < size; ++i)
        {
            if(existingTypename == userDefinedTypes[i])
            {
                return i;
            }
        }
        std::cerr << "SOMETHING WENT WRONG" << std::endl;
        exit(1);
        return -1;
    }
    bool CheckRange()
    {
        if(currIndex > commandLineLength)
        {
            std::cout << WRONG_AMOUNT_OF_ARGS << command << std::endl;
            Restart();
        }
        return true;
    }

    void Reset()
    {
        query.clear();
        command.clear();
        currIndex = -1;
    }
    void Restart()
    {
        Reset();
        start();
    }
private:
    std::string query;
    std::string command;
    int currIndex = -1;
    int commandLineLength = 0;
    bool over = false;
private:
    std::vector<std::string> userDefinedTypes;
    std::vector<std::vector<std::map<std::string, std::string>>> pairsForEachDataType;
};

int main()
{
    crud obj;
    system("clear");
    return 0;
}