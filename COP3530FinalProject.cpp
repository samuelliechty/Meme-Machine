/*
Author: Samuel Liechty
Date: 12/05/2020
*/

#include <SFML/Graphics.hpp>
#include <algorithm>
#include <fstream>
#include <random>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <array>
#include <cmath>
#include <math.h>
#include <iostream>
#include "HashTableImplementation.h"
#include "AVLTreeImplementation.h"
#pragma once
using namespace std;
using namespace std::chrono;
using namespace sf;

int main()
{
	sf::RenderWindow window(sf::VideoMode(1600, 1200), "Comparing Data Structures!");

	//Initializing various sprites and textures
	Sprite startScreen;
	Sprite loadDataScreen;
	Sprite inputScreen;
	Sprite loadingScreen;
	Sprite findingScreen;
	Sprite resultsScreen;
	Sprite failScreen;

	Texture texture;
	Texture texture2;
	Texture texture3;
	Texture texture4;
	Texture texture5;
	Texture texture6;
	Texture texture7;

	//loading image files into textures
	if (!texture.loadFromFile("./FPAssets/startScreen.png")) {

	}
	startScreen.setTexture(texture);
	startScreen.setPosition(0, 0);

	if (!texture2.loadFromFile("./FPAssets/loadTimeScreen.png")) {

	}
	loadDataScreen.setTexture(texture2);
	loadDataScreen.setPosition(0, 0);

	if (!texture3.loadFromFile("./FPAssets/inputScreen.png")) {

	}
	inputScreen.setTexture(texture3);
	inputScreen.setPosition(0, 0);

	if (!texture4.loadFromFile("./FPAssets/loadingScreen.png")) {

	}
	loadingScreen.setTexture(texture4);
	loadingScreen.setPosition(0, 0);

	if (!texture5.loadFromFile("./FPAssets/findingScreen.png")) {

	}
	findingScreen.setTexture(texture5);
	findingScreen.setPosition(0, 0);

	if (!texture6.loadFromFile("./FPAssets/resultsScreenv2.png")) {

	}
	resultsScreen.setTexture(texture6);
	resultsScreen.setPosition(0, 0);

	if (!texture7.loadFromFile("./FPAssets/failScreen.png")) {

	}
	failScreen.setTexture(texture7);
	failScreen.setPosition(0, 0);

	//initialize screen variable that will be used to determine which image to display
	int screen = 0;

	bool yetLoaded = false;
	bool findMeme = false;

	//for displaying text on the screen
	sf::Text text;
	sf::Text text2 = text;
	sf::Text text3 = text;
	sf::Font font;
	if (!font.loadFromFile("./FPAssets/arial.ttf")) {

	}
	text.setFont(font);
	
	text.setString("");
	text.setCharacterSize(175);
	text.setFillColor(sf::Color::Black);
	text.setStyle(sf::Text::Bold);
	text.setPosition(405, 400);

	//for storing and displaying user-inputted text
	sf::String userInput = "";
	int UISize = 0;

	MemesDataTree memesData;

	//variables for holding function run-times for comparison
	int hashLoadTime;
	int treeLoadTime;
	int hashSearchTime;
	int treeSearchTime;


	while (window.isOpen())
	{
		if (screen == 1 && !yetLoaded) {
			yetLoaded = !yetLoaded;

			//run and time loading memes into hash table
			auto start = high_resolution_clock::now();
			initalizeHashTable("./FPAssets/memes_data.tsv");
			auto stop = high_resolution_clock::now();
			auto duration = std::chrono::duration_cast<std::chrono:: milliseconds > (stop - start);
			hashLoadTime = int(duration.count());

			//run and time loading memes into binary search tree
			auto start2 = high_resolution_clock::now();
			ReadData(memesData,"./FPAssets/memes_data.tsv");
			auto stop2 = high_resolution_clock::now();
			auto duration2 = std::chrono::duration_cast<std::chrono::milliseconds> (stop2 - start2);
			treeLoadTime = int(duration2.count());

			text.setString(to_string(treeLoadTime));
			text.setPosition(200, 400);

			text2 = text;
			text2.setString(to_string(hashLoadTime));
			text2.setPosition(900, 400);
			
			text3 = text;
			text3.setString("Milliseconds");
			text3.setPosition(300, 600);
		}

		if (screen == 3) {
			//building whole url from user-inputted key
			string findURL = "http://i.imgflip.com/";
			findURL = findURL.append(userInput);
			findURL = findURL.append(".jpg");

			//searching for meme by key and timing the process in hash table
			auto start3 = high_resolution_clock::now();
			searchKeys(findURL);
			auto stop3 = high_resolution_clock::now();
			auto duration3 = std::chrono::duration_cast<std::chrono::milliseconds> (stop3 - start3);
			hashSearchTime = int(duration3.count());

			//searching for meme by key and timing the process in binary search tree
			auto start4 = high_resolution_clock::now();
			TreeNode* acquireMeme = memesData.KeySearch(findURL);
			auto stop4 = high_resolution_clock::now();
			auto duration4 = std::chrono::duration_cast<std::chrono::milliseconds> (stop4 - start4);
			treeSearchTime = int(duration4.count());

			text.setString(to_string(hashSearchTime));
			text.setPosition(600, 825);
			//text.setColor(sf::Color::White);
			window.draw(text);

			text2.setString(to_string(treeSearchTime));
			text2.setPosition(685, 970);
			//text2.setColor(sf::Color::White);
			window.draw(text2);

			if (acquireMeme == NULL) { //if the key didn't return a meme activate fail screen
				screen = 5;
			}
			else { //if the key successfully returned a meme, display results
				screen = 4;

				text3.setString(acquireMeme->data.caption_text);
				text3.setPosition(5, 630);
				text3.setCharacterSize(42);
				text3.setLetterSpacing(0.1);
			}

		}

		window.clear();
		sf::Event event;
		while (window.pollEvent(event))
		{

			if (event.type == sf::Event::Closed) {
				window.close();
			}
			
			if (event.type == sf::Event::MouseButtonPressed) {
				if (event.mouseButton.button == Mouse::Left) {
					if (screen == 0) { //if mouse is left-clicked while on start screen, advance to loading screen
						screen = 1;
					}
					else if (screen == 1) { //if mouse is left-clicked while on load-results screen, advance to user input screen
						screen = 2;
						text.setString("");
						userInput = "";
						text.setPosition(405, 400);
					}
				}
			}
			if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == Keyboard::Return) {
					if (screen == 2 && UISize == 6) { //if return/enter key is pressed on user input screen and a key of length 6 is inputted, advance to search screen
						screen = 3;
					}
					if (screen == 4 || screen == 5) { //if return/enter key is pressed on either results screen (succcess or fail) return to user input screen
						screen = 2;
						userInput = "";
						text.setString(userInput);
						text.setPosition(405, 400);
					}
				}
				else if (event.key.code == Keyboard::Backspace) {
					if (screen == 2) { //if backspace key is pressed while on user input screen, erase any current user input
						userInput = "";
						text.setString(userInput);
					}
					if (screen == 4 || screen == 5) { //if backspace key is pressed while on a results screen (success or fail) close the window
						window.close();
					}
				}
				else {
					UISize = userInput.getSize();
					if (screen == 2 && UISize < 6) { //if a key, a-z or 0-9 is pressed, and current userInput length is less than 6, append it to the current userInput
						if (int(event.text.unicode) < 26) {
							userInput += 97 + event.text.unicode;
						}
						else if(int(event.text.unicode) > 25 && int(event.text.unicode) < 26+10){
							userInput += 22 + event.text.unicode;
						}
						text.setString(userInput);
					}
					UISize = userInput.getSize();
				}
			}

		}
		if (screen == 0) { //screen = 0 for starting screen
			window.draw(startScreen);
		}
		else if (screen == 1) { //screen = 1 initially for loading screen then for loading results
			window.draw(loadDataScreen);

			window.draw(text);
			window.draw(text2);
			window.draw(text3);
			if (!yetLoaded) {
				window.draw(loadingScreen);
			}

		}
		else if (screen == 2) { //screen = 2 for user input
			window.draw(inputScreen);
			window.draw(text);
		}
		else if (screen == 3) { //screen = 3 for loading screen while searching for memes
			window.draw(findingScreen);
		}
		else if (screen == 4) { //screen = 4 for results screen when a meme is found successfully
			window.draw(resultsScreen);
			window.draw(text);
			window.draw(text2);
			window.draw(text3);
		}
		else if (screen == 5) { //screen = 5 for results screen when a key yields no meme
			window.draw(failScreen);
			window.draw(text);
			window.draw(text2);
		}

		window.display();
	}
	return 0;
}