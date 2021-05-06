
#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include <fstream>
//#include <Windows.h>

using namespace std;

//Saleh Ebrahimian 9822762288

int getMin (int x , int y , int z)
{
    if(x <= y && x <= z)
        return x;

    if(y <= z)
        return y;

    return z;
}

int fillMatrix (vector<vector<int>> matrix , int i , int j , string str1 , string str2)
{
    int insertCost = 1;
    int deleteCost = 1;
    int editCost = 2;

    if(i == j & i == 0)
    {
        return 0;
    }
    else  if(j == 0)
    {
        return i;
    }
    else if(i == 0)
    {
        return j;
    }

    int insert = matrix[i - 1][j] + insertCost;
    int deletec = matrix[i][j - 1] + deleteCost;
    int otherwise = 0;

    char xi = str1.at(i - 1);
    char yi = str2.at(j - 1);   

    if(xi != yi)
    {
        otherwise = matrix[i - 1][j - 1] + editCost;
    }
    else
    {
        return matrix[i - 1][j - 1];
    }

    return getMin (insert , deletec , otherwise);
}

void makeMatrix(vector<vector<int>> &matrix , string userDomain , string siteName)
{
    for(int z = 0; z < userDomain.size() + 1; z++)
    {
        matrix[z] = vector<int>(siteName.size() + 1);
    }

    for(int i = 0 ; i <= userDomain.size() ; i++)
    {
        for(int j = 0 ; j <= siteName.size() ; j++)
        {
            int result = fillMatrix(matrix , i , j , userDomain , siteName);
            matrix[i][j] = result;
        }
    }  

}

void printMatrix(vector<vector<int>> matrix , string str1 , string str2)
{
    cout << "  ";

    for(int s2 = 0 ; s2 < str2.size() ; s2++)
    {
        cout << str2[s2] << " ";
    }

    cout << '\n';

	for(int i = 0 ; i <= str1.size() ; i++)
    {
        cout << str1[i] << " ";
        for(int j = 0 ; j <= str2.size() ; j++)
        {
            cout << matrix[i][j] << " ";
        }
        cout << '\n';
    }  
    cout << '\n';
}

string getMainDomain (string domainName)
{
    string result = domainName;

    //Remove www. from domainName

    string www = "www.";
    int index = result.find(www);
    if(index == 0)
    {
        result.erase(0 , www.length());
    }

    //----------------------------

    //-----Remove Subdomains------

    vector<int> subdomains;

    for(int i = 0; i < result.length(); i++)
    {
        if(result[i] == '.')
        {
            subdomains.push_back(i);
        }
    }

    if(subdomains.size() > 0)
    {
        subdomains.pop_back(); //This is root domain '.' example: saleh.ir -> this will remove last '.' for 'ir'
        if(subdomains.size() > 0)
        {
            int removePos = subdomains.at(subdomains.size() - 1);

            result.erase(0 , removePos + 1);
        }
    }

    //----------------------------

    return result;
}

// string GetCurrentDirectory()
// {
// 	char buffer[MAX_PATH];
// 	GetModuleFileNameA(NULL, buffer, MAX_PATH);
// 	string::size_type pos = string(buffer).find_last_of("\\/");
	
// 	return string(buffer).substr(0, pos);
// }

int main()
{
    string userDomain;

    cout << "Enter Your domain Name : ";
    cin >> userDomain;

    string newDomain = getMainDomain(userDomain);
    if(newDomain != userDomain)
    {
        cout << "Your Main Domain is : " << newDomain << '\n';
        userDomain = newDomain;
    }

    cout << "Do you want to print Matrix for result at the end ? (Y/N)";

    char isPrintMatrix = 'N';

    cin >> isPrintMatrix;

    string fileName = "./top_500_domains.txt";

    
    bool readFile = true;
    
    string desiredDomain;

    char readType = 'F';

    cout << "Do you want to read from file or Enter your Custom domain ? (F for file/C for custom)" ;
    cin >> readType;

    if(readType == 'C')
    {
        cout << "Enter your custom domain to Comapre with : " << '\n';
        cin >> desiredDomain;
        readFile = false;
    }
    else
    {
        readFile = false;

        ifstream f(fileName.c_str());
        if(!f.good())
        {
            cout << "Please put 'top_500_domains.txt' next to the executable file \n";
            //cout << "Please put 'top_500_domains.txt' in \n --- " << GetCurrentDirectory() << " --- \n";

            cout << "Can't read file" << '\n';
                return 0;
        }
        else
        {
            readFile = true;
        }
    }
    

    vector<string> sites(500);

    if(readFile)
    {
        cout << "reading topDomain sitenames from file ... " << '\n';

        fstream newfile;
        newfile.open(fileName,ios::in);
        if (newfile.is_open())
        {
            string tp;
            int i = 0;
            while(getline(newfile, tp))
            {
                sites[i] = tp;
                i++;
            }
            newfile.close();
        }

        cout << "Done !" << '\n';
    }
    else
    {
        sites[0] = desiredDomain;
    }

    vector<vector<int>> matrix(userDomain.size() + 1);

    int similarity = INT8_MAX;

    vector<vector<int>> toPrintMatrix;
    string topSiteName ;

    for(int i = 0;i < sites.size(); i++)
    {
        if(sites[i] != "")
        {
            string topDomain = getMainDomain(sites[i]);

            makeMatrix(matrix , userDomain , topDomain);

            int temp = matrix[userDomain.size()][topDomain.size()];
            if(temp < similarity)
            {
                topSiteName = sites[i];
                toPrintMatrix = matrix;
                similarity = temp;
            }
        }
    }

    if(tolower(isPrintMatrix) == 'y')
        printMatrix(toPrintMatrix , userDomain , getMainDomain(topSiteName));
    
    if(similarity == 0)
    {
        cout << "Your Domain Exist";
    }
    else
    {
        cout << "Most Simialr TopDomain to your domain is : " << getMainDomain(topSiteName) << '\n';
        if(readType == 'F')
            cout << "Domain in file is : " << topSiteName << '\n';

        int baseCost = 10;
    
        if(similarity == 1)
            baseCost *= 10000;
        
        else if(similarity == 2)
            baseCost *= 8000;

        if(similarity == 3)
            baseCost *= 6000;

        if(similarity == 4)
            baseCost *= 4000;

        if(similarity == 6)
            baseCost *= 2000;

        if(similarity >= 7 && similarity <= 10)
            baseCost *= 100;

        if(similarity > 10)
            baseCost *= 10;

        cout << "Min Distance : " << similarity << '\n';

        cout << "Your domain cost is : " << baseCost;
    }

    

	return 0;
    

}
