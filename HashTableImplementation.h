/*
Author: Mikayla Hill
Date: 12/05/2020
*/
#pragma once
/*
			 EXCEL TO HASH TABLE
			 URL is hashTable[i][j].key
			 Caption Text is hashTable[i][j].value.first
			 Meme Lable hashTable[i][j].value.second<<endl;
		   */

		   //your AVL tree implementation here
#include <iostream>
#include <sstream>
#include <vector>
#include <fstream>
#include <utility>
#include <functional> 

using namespace std;
#define SIZE_BUCKET 1000000//set size of chars Max
struct hashValues {
	string key;//Caption Text
	pair<string, string> value; //URL, MemeLbl
};
vector<hashValues>  hashTable(SIZE_BUCKET);


int hashCode(string key) {// Prints out the hash Code from moding the bucket size
   //scout<<"size "<<(key.size() % SIZE_BUCKET)<<endl;
   //cout<<key<<endl;
	const char* newBase = new char[key.length() + 1];
	//strcpy(newBase, key.c_str());
	newBase = key.c_str();
	int len = strlen(newBase);

	int base = 1;
	int decimal = 0;

	for (int i = len - 1; i >= 0; i--)
	{
		if (newBase[i] >= '0' && newBase[i] <= '9') {
			decimal += (newBase[i] - 48) * base;
			base = base * 36;
		}
		if (newBase[i] >= 'a' && newBase[i] <= 'z') {
			decimal += (newBase[i] - 87) * base;
			base = base * 36;
		}
	}
	//cout<<decimal/1000<<endl;
	//delete newBase;
	return decimal / 1000;
}

void removeStr(string& line, const string& word)
{
	auto n = line.find(word);
	if (n != string::npos)
	{
		line.erase(n, word.length());
	}
}
void searchKeys(string url) {//Goes through each keys and sees if contains a given string
	vector<string>out;
	string urlCode = url;
	removeStr(urlCode, ".jpg");
	removeStr(urlCode, "http://i.imgflip.com/");
	int hashIdx = hashCode(urlCode);

	//cout<<"SEARCH FOR: "<<urlCode<<endl;
	//cout<<"SEARCH FOR: "<<hashIdx<<endl;
	if (hashIdx > hashTable.size()) {
		
	}
	else if (urlCode == hashTable[hashIdx].key) {//O(1)
		cout << "URL: http://i.imgflip.com/" << hashTable[hashIdx].key << ".jpg ";
		cout << "CAPTION: " << hashTable[hashIdx].value.first << ".jpg ";
		cout << "MEME LBL: " << hashTable[hashIdx].value.second;
		cout << endl;
		return;
	}
	else { //O(hashIdx) If not at the original position

		for (int i = hashIdx; i < hashTable.size(); i++) {
			if (urlCode == hashTable[i].key) {
				cout << "URL: http://i.imgflip.com/" << hashTable[i].key << ".jpg ";
				cout << "CAPTION: " << hashTable[i].value.first << " ";
				cout << "MEME LBL: " << hashTable[i].value.second;
				cout << endl;
				return;
			}
		}
	}
}

void insert(string key, pair<string, string> value) {// Inserts Based on Caption Text Hash Code
	int hashIdx = hashCode(key);
	struct hashValues insertData;
	insertData.key = key;
	insertData.value = value;


	if (!(hashTable[hashIdx].key).empty()) {
		//cout<<"FULL"<<hashIdx <<endl;

		while (!(hashTable[hashIdx++].key).empty()) {
			//cout<<"FOUND"<<hashIdx <<endl;
			hashTable[hashIdx] = insertData;
			break;
		}


	}
	else {
		hashTable[hashIdx] = insertData;
		//cout<<hashIdx<<endl;
	}



}
void printHash() {

	for (int i = 0; i < hashTable.size(); i++) {
		if (!(hashTable[i].key).empty()) {
			cout << "URL: http://i.imgflip.com/" << hashTable[i].key << ".jpg ";
			cout << "CAPTION: " << hashTable[i].value.first << " ";
			cout << "MEME LBL: " << hashTable[i].value.second;
			cout << endl;
		}
	}

}

void initalizeHashTable(string fileName) {// ADDS values to the table

	ifstream file(fileName);
	if (!file.is_open()) throw runtime_error("Could not open file");
	string currentRow;

	int counter = 0;
	while (getline(file, currentRow, '\n')) //Breaks based on line
	{
		//section.push_back(currentRow);
		stringstream currentLine(currentRow);
		string colInfo;
		int dataIdx = 0;
		string memeKey;
		counter++;
		pair<string, string> memeValue;
		while (getline(currentLine, colInfo, '\t')) {// breaks based on col sections

			//0 1 2
			if (dataIdx == 0 && counter != 1) {
				memeValue.first = colInfo;//CaptionTExt

			}
			if (dataIdx == 1 && counter != 1) {

				string urlCode = colInfo;
				removeStr(urlCode, ".jpg");
				removeStr(urlCode, "http://i.imgflip.com/");
				memeKey = urlCode;//ImageURL

			}
			if (dataIdx == 2 && counter != 1) {
				memeValue.second = colInfo;//MemeLbl
			}

			dataIdx++;
		}

		insert(memeKey, memeValue);
		/*
		  EXCEL TO HASH TABLE
		  Caption Text is hashTable[i][j].key
		  URL is hashTable[i][j].value.first
		  Meme Lable hashTable[i][j].value.second<<endl;
		*/
	}
	file.close();
}