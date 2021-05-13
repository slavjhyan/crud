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
            std::cout << "slav@slav-Q551LN:" << ">";
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
            system("clear");
            Reset();
            exit(1);
        }
        if(command == CLEAR_COMMAND)
        {
            system("clear");
            Reset();
            return true;
        }
        if(command == NEW_TYPE_CONSTRUCT_COMMAND)
        {
            BuildCommand();
            return true;
        }
        if(command == GET_WHERE_COMMAND)
        {
            GetWhereCommand();
            return true;
        }
        if(command == UPDATE_COMMAND)
        {
            UpdateCommand();
            return true;
        }
        if(command == DELETE_COMMAND)
        {
            DeleteCommand();
            return true;
        }
        if(command == REMOVE_COMMAND)
        {
            RemoveCommand();
            return true;
        }
        std::cout << INVALID_COMMAND << std::endl;
        Restart();
        return false;
    }

    //start of main commands
    void CreateCommand()
    {
        std::string newTypeName = GetType();
        if(CheckTypeNameExistance(newTypeName, true))
        {
            std::cout << EXISTING_TYPE << std::endl;
            Restart();
        }
        --currIndex;
        while(query[++currIndex] != '{' && currIndex < commandLineLength) {}
        CheckRange();
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
        CreateNewType(newTypeName);
        std::vector<std::map<std::string, std::string>> pairForCurrentDataType;
        pairForCurrentDataType.push_back(dataforCurrentType);
        pairsForEachDataType.push_back(pairForCurrentDataType);
        Reset();
        std::cout << "OK" << std::endl;
    }

    void BuildCommand()
    {
        std::string existingTypeName = GetType();
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
        for(std::map<std::string, std::string>::iterator it = currTypePairs[0].begin(); it != currTypePairs[0].end(); ++it)
        {
            if(currTypePair.find(it->first) == currTypePair.end())
            {
                currTypeInFile << it->first << ": " << "unknown\n"; 
            }
        }
        currTypeInFile << SEPARATOR;
        currTypeInFile.close();
        pairsForEachDataType[ix].push_back(currTypePair);
        Reset();
        std::cout << "OK" << std::endl;
    }

    void GetWhereCommand()
    {
        std::string existingTypeName = GetType();
        if(!CheckTypeNameExistance(existingTypeName))
        {
            std::cout << NON_EXISTING_TYPE << std::endl;
            Restart();
        }
        std::map<std::string, std::string> updateConditions;
        std::string fileName = existingTypeName + ":_";
        bool done = false;
        do{
            while(!isalnum(query[++currIndex]) && currIndex < commandLineLength) {}
            int keyNameStartPos = currIndex;
            while(isalnum(query[++currIndex]) && currIndex < commandLineLength) {}
            CheckRange();
            std::string keyName = query.substr(keyNameStartPos, currIndex - keyNameStartPos);
            SearchForKeyInMap(existingTypeName, keyName);
            --currIndex;
            while(query[++currIndex] != '=' && currIndex < commandLineLength) {}
            while(!isalnum(query[++currIndex]) && currIndex < commandLineLength) {}
            int valStartPos = currIndex;
            while(isalnum(query[++currIndex]) && currIndex < commandLineLength) {}
            CheckRange();
            std::string value = query.substr(valStartPos, currIndex - valStartPos);
            updateConditions.insert(std::pair<std::string, std::string>(keyName, value));
            fileName += "_" + keyName + "=" + value;
            while(query[currIndex] != ',')
            {
                if(query[currIndex] == '\0')
                {
                    done = true;
                    break;
                }
                ++currIndex;
            }
        } while(!done);
        std::ofstream result(fileName + ".txt", std::ios::app);
        int ix = GetTypeNameIndex(existingTypeName);
        int size = pairsForEachDataType[ix].size();
        for(std::vector<std::map<std::string, std::string>>::iterator it = pairsForEachDataType[ix].begin(); it != pairsForEachDataType[ix].end(); ++it)
        {
            if(MeetsConditions(*it, updateConditions))
            {
                OutputMapToFile(*it, result);
            }
        }
        std::cout << "OK" << std::endl;
        Reset();
    }

    void UpdateCommand()
    {
        std::string existingTypeName = GetType();
        if(!CheckTypeNameExistance(existingTypeName))
        {
            std::cout << NON_EXISTING_TYPE << std::endl;
            Restart();
        }
        while(!isalpha(query[++currIndex]) && currIndex < commandLineLength) {}
        int startPos = currIndex;
        --currIndex;
        while(isalpha(query[++currIndex]) && currIndex < commandLineLength) {}
        CheckRange();
        std::string subCommand = query.substr(startPos, currIndex - startPos);
        std::map<std::string, std::string> updateConditions;
        if(subCommand == "where")
        {
            bool done = false;
            do{
                while(!isalnum(query[++currIndex]) && currIndex < commandLineLength) {}
                startPos = currIndex;
                --currIndex;
                while(isalnum(query[++currIndex]) && currIndex < commandLineLength) {}
                CheckRange();
                std::string keyName = query.substr(startPos, currIndex - startPos);
                --currIndex;
                SearchForKeyInMap(existingTypeName, keyName);
                while(query[++currIndex] != '=' && currIndex < commandLineLength) {}
                while(!isalnum(query[++currIndex]) && currIndex < commandLineLength) {}
                int valueStartPos = currIndex;
                while(isalnum(query[++currIndex]) && currIndex < commandLineLength) {}
                CheckRange();
                std::string value = query.substr(valueStartPos, currIndex - valueStartPos);
                --currIndex;
                updateConditions.insert(std::pair<std::string, std::string>(keyName, value));
                while(query[currIndex] != ',')
                {
                    if(query[currIndex] == '|')
                    {
                        done = true;
                        break;
                    }
                    ++currIndex;
                }
            } while(!done);
        }
        else if(subCommand != "all")
        {
            std::cout << INVALID_ARGUMENT << command << std::endl;
        };
        while(!isalnum(query[++currIndex]) && currIndex < commandLineLength) {}
        CheckRange();
        int typeIx = GetTypeNameIndex(existingTypeName);
        --currIndex;
        std::map<std::string, std::string> newValues;
        do{
            while(!isalnum(query[++currIndex]) && currIndex < commandLineLength) {}
            int keyStartPos = currIndex;
            --currIndex;
            while(isalnum(query[++currIndex]) && currIndex < commandLineLength) {}
            CheckRange();
            std::string key = query.substr(keyStartPos, currIndex - keyStartPos);
            if(pairsForEachDataType[typeIx][0].find(key) == pairsForEachDataType[typeIx][0].end())
            {
                std::cout << key << NON_EXISTING_KEY << std::endl;
                Restart();
            }
            for(std::map<std::string, std::string>::iterator it = newValues.begin(); it != newValues.end(); ++it)
            {
                if(it->first == key)
                {
                    std::cout << EXISTING_KEY << std::endl;
                    Restart();
                }
            }
            --currIndex;
            while(query[++currIndex] != '=' && currIndex < commandLineLength) {}
            --currIndex;
            while(!isalnum(query[++currIndex]) && currIndex < commandLineLength) {}
            int valueStartPos = currIndex;
            while(isalnum(query[++currIndex]) && currIndex < commandLineLength) {}
            CheckRange();
            std::string newValueForCurrKey = query.substr(valueStartPos, currIndex - valueStartPos);
            newValues.insert(std::pair<std::string, std::string>(key, newValueForCurrKey));
            --currIndex;
            while(query[++currIndex] != ',' && currIndex < commandLineLength) {}
            if(currIndex == commandLineLength)
            {
                break;
            }
        } while(true);
        if(updateConditions.empty())
        {
            for(std::vector<std::map<std::string, std::string>>::iterator it = pairsForEachDataType[typeIx].begin(); it != pairsForEachDataType[typeIx].end(); ++it)
            {
                for(std::map<std::string, std::string>::iterator mit = newValues.begin(); mit != newValues.end(); ++mit)
                {
                    (it->find(mit->first))->second = mit->second;
                }
            }
        }
        else
        {
            for(std::vector<std::map<std::string, std::string>>::iterator it = pairsForEachDataType[typeIx].begin(); it != pairsForEachDataType[typeIx].end(); ++it)
            {
                if(MeetsConditions(*it, updateConditions))
                {
                    for(std::map<std::string, std::string>::iterator mit = newValues.begin(); mit != newValues.end(); ++mit)
                    {
                        (it->find(mit->first))->second = mit->second;
                    }
                }
            }
        }
        RefreshFile(existingTypeName);
        std::cout << "OK" << std::endl;
        Reset();
    }

    void DeleteCommand()
    {
        std::string existingTypeName = GetType();
        if(!CheckTypeNameExistance(existingTypeName))
        {
            std::cout << NON_EXISTING_TYPE << std::endl;
            Restart();
        }
        while(!isalpha(query[++currIndex]) && currIndex < commandLineLength) {}
        int startPos = currIndex;
        --currIndex;
        while(isalpha(query[++currIndex]) && currIndex < commandLineLength) {}
        CheckRange();
        std::string subCommand = query.substr(startPos, currIndex - startPos);
        std::map<std::string, std::string> deleteConditions;
        if(subCommand == "where")
        {
            bool done = false;
            do{
                while(!isalnum(query[++currIndex]) && currIndex < commandLineLength) {}
                startPos = currIndex;
                --currIndex;
                while(isalnum(query[++currIndex]) && currIndex < commandLineLength) {}
                CheckRange();
                std::string keyName = query.substr(startPos, currIndex - startPos);
                --currIndex;
                SearchForKeyInMap(existingTypeName, keyName);
                while(query[++currIndex] != '=' && currIndex < commandLineLength) {}
                while(!isalnum(query[++currIndex]) && currIndex < commandLineLength) {}
                int valueStartPos = currIndex;
                while(isalnum(query[++currIndex]) && currIndex < commandLineLength) {}
                CheckRange();
                std::string value = query.substr(valueStartPos, currIndex - valueStartPos);
                --currIndex;
                std::cout << keyName << ' ' << value << '|' << std::endl;
                deleteConditions.insert(std::pair<std::string, std::string>(keyName, value));
                while(query[currIndex] != ',')
                {
                    if(currIndex == commandLineLength - 1)
                    {
                        done = true;
                        break;
                    }
                    ++currIndex;
                }
            } while(!done);
        }
        else if(subCommand != "all")
        {
            std::cout << INVALID_ARGUMENT << command << std::endl;
        }
        int typeIx = GetTypeNameIndex(existingTypeName);
        if(deleteConditions.empty())
        {
            pairsForEachDataType[typeIx].erase(pairsForEachDataType[typeIx].begin() + 1, pairsForEachDataType[typeIx].end());
            ClearFile(existingTypeName);
        }
        else
        {
            for(std::vector<std::map<std::string, std::string>>::iterator it = pairsForEachDataType[typeIx].begin(); it != pairsForEachDataType[typeIx].end(); ++it)
            {
                if(MeetsConditions(*it, deleteConditions))
                {
                    pairsForEachDataType[typeIx].erase(it);
                    --it;
                }
            }
            RefreshFile(existingTypeName);
        }
        std::cout << "OK" << std::endl;
        Reset();
    }

    void RemoveCommand()
    {
         std::string existingTypeName = GetType();
        if(!CheckTypeNameExistance(existingTypeName))
        {
            std::cout << NON_EXISTING_TYPE << std::endl;
            Restart();
        }
        int typeIx = GetTypeNameIndex(existingTypeName);
        if(std::remove((existingTypeName + ".txt").c_str()))
        {
            std::cout << "Something went wrong" << std::endl;
        }
        pairsForEachDataType.erase(pairsForEachDataType.begin() + typeIx);
        userDefinedTypes.erase(userDefinedTypes.begin() + typeIx);
        Reset();
    }
    //end of main commands

    bool MeetsConditions(const std::map<std::string, std::string>& currMap, const std::map<std::string, std::string>& conditionMap)
    {
        for(std::map<std::string, std::string>::const_iterator it = conditionMap.begin(); it != conditionMap.end(); ++it)
        {
            if((currMap.find(it->first))->second != it->second)
            {
                return false;
            }
        }
        return true;
    }
    void OutputMapToFile(const std::map<std::string, std::string>& m, std::ofstream& file)
    {
        int size = m.size();
        for(std::map<std::string, std::string>::const_iterator it = m.begin(); it != m.end(); ++it)
        {
            file << it->first << ": " << it->second << '\n';
        }
        file << SEPARATOR;
    }

    void RefreshFile(const std::string& type)
    {
        if(!CheckTypeNameExistance(type))
        {
            std::cout << NON_EXISTING_TYPE << std::endl;
            Restart();
        }
        std::ofstream file(type + ".txt");
        int typeIx = GetTypeNameIndex(type);
        std::vector<std::map<std::string, std::string>>::iterator it = pairsForEachDataType[typeIx].begin();
        for(++it; it != pairsForEachDataType[typeIx].end(); ++it)
        {
            OutputMapToFile(*it, file);
        }
    }

    void ClearFile(const std::string& type)
    {
        std::ofstream file(type + ".txt");
    }
    void SearchForKeyInMap(const std::string& existingTypeName, const std::string& keyName)
    {
        int ix = GetTypeNameIndex(existingTypeName);
        std::vector<std::map<std::string, std::string>> currTypePairs = pairsForEachDataType[ix];
        if(currTypePairs[0].find(keyName) == currTypePairs[0].end())
        {
            std::cout << keyName << NON_EXISTING_KEY << std::endl;
            Restart();
        }
    }
    bool CheckTypeNameExistance(const std::string& newTypeName, bool needToCreateType = false)
    {
        int typesSize = userDefinedTypes.size();
        for(int i = 0; i < typesSize; ++i)
        {
            if(newTypeName == userDefinedTypes[i])
            {
                return true;
            }
        }
        return false;
    }

    void CreateNewType(const std::string& newTypeName)
    {
            userDefinedTypes.push_back(newTypeName);
            std::ofstream newFile;
            newFile.open(newTypeName + ".txt");
    }
    std::string GetType()
    {
        while(isblank(query[++currIndex]) && currIndex < commandLineLength) {}
        int typeStartPos = currIndex;
        while(isalnum(query[++currIndex]) && currIndex < commandLineLength) {}
        CheckRange();
        return query.substr(typeStartPos, currIndex - typeStartPos);
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
    crud db;
    return 0;
}