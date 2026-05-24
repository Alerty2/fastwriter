# Fastwriter
A lightweight productivity application written in C which lets you write a dictionary of which words do you want to get substituted with which text and substitutes them automaticaly on the fly while you are writing on any application.

## Compiling
First of all, select your OS. Open fastwritter.c and change the #define OS line with your corresponding OS.

<img width="463" height="44" alt="imagen" src="https://github.com/user-attachments/assets/0ea371d6-ff7a-4838-81e9-72b16b007931" />

Then execute:
```
bash build.bash
```
The program should be compiled.

## Usage
For using Fastwriter correctly you will need to configre your config.json correctly.
To archieve this you just hace to write and object titeled with the name of the word you want to substitute and give it as value the string for which it needs to be substituted when written. Finally you need to count how many objects there are and write it it in the length object (the length object also needs to be counted).

This is the default config.json:
```
{
    "Length": "5",
    "job": "Social Media Manager for Corporation .corp",
    "goodbye": "I hope you hace a nice holiday\nHugs and kisses!",
    "special": "I can write special characters! ñáíúó",
    "mail": "Dear Director,\n\nI would like to send you my new invention which consists on a nuclear rocket\n\nhttps://   drive.google.com/file/d/12tyD7Y0lu6VBhNBVrTBU85t8c06Ita2u/view?usp=sharing\n\nHope you like it,\n\nAlerty.\n"
}
```
For using it, just execute the program and it will run in the background substituting the words you need.

## Interface (WIP)
Currently, I am making a graphical interface with GTK which will make Fastwriter much more easy-to-use making it easir to configure and define when it should be launched and when not. This interface will be released in a separate repository.

## Compatibility
Fastwriter works perfectly on Linux and it should work on Windows put it has bot been tested. MAC has also not been tested but in theory, the only problem that should arrise when using this platforms is the usage of special characters (á, é, í, ñ...) which in Windows should work but hasn't been tested yet and currently it isn't supported for MAC.
