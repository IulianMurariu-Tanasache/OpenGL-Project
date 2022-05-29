#pragma once
#include <string>
#include <map>

const std::string sunText =
"SUN\nStar-type: Yellow Dwarf\nAge: ~4.5 billion years"
"The Sun is the largest object in our solar\n"
"system. The Sun’s volume would need 1.3 million\n"
"Earths to fill it. The hottest part of the Sun is\n"
"its core, where temperatures top 15 million degrees Celsius.";

const std::string mercuryText =
"MERCURY\nLength of year: 88 Earth Days\nMoons: 0; Planet-type: Terrestrial\n"
"The smallest planet in our solar system\n"
"and nearest to the Sun,\n"
"Mercury is only slightly larger than Earth's Moon.";

const std::string venusText =
"VENUS\nLength of year: 225 Earth Days\nMoons: 0; Planet-type: Terrestrial\n"
"Venus is the second planet from the Sun\n"
"and is Earth’s closest planetary neighbor.\n"
"It’s one of the four inner, rocky planets\n"
"and it’s often called Earth’s twin because \n"
"it’s similar in size and density.";

const std::string earthText =
"EARTH\nLength of year: 365 Earth Days\nMoons: 1; Planet-type: Terrestrial\n"
"Our home planet is the third planet\n"
"from the Sun.While Earth is only \n"
"the fifth largest planet in the solar\n"
"system, it is the only world in\n"
"our solar system with liquid water on the surface.";

const std::string marsText = 
"MARS\nLength of year: 687 Earth Days\nMoons: 2; Planet-type: Terrestrial\n"
"Mars is the fourth planet from the Sun:\n"
"a dusty, cold, desert world with a very thin atmosphere.\n"
"Mars is also a dynamic planet with seasons,\n"
"polar ice caps, canyons, extinct volcanoes,\n"
"and evidence that it was even more active in the past.";

const std::string jupiterText =
"JUPITER\nLength of year : 4.333 Earth Days\nMoons : >75; Planet-type: Gas Giant\n"
"Fifth in line from the Sun, Jupiter \n"
"is the largest planet in the solar system:\n"
"twice as massive as all the other planets combined.\n"
"Jupiter’s iconic Great Red Spot is a giant storm \n"
"bigger than Earth that has raged for hundreds of years.";

const std::string saturnText =
"SATURN\nLength of year: 10.759 Earth Days\nMoons: ~82; Planet-type: Gas Giant\n"
"Saturn is the sixth planet from the Sun and\n"
"the second-largest planet in our solar system.\n"
"It is not the only planet to have rings:\n"
"made of chunks of iceand rock,but none are\n"
"as spectacular or as complicated as Saturn's.";

const std::string uranusText =
"URANUS\nLength of year: 30.687 Earth Days\nMoons: 27; Planet-type: Ice Giant\n"
"Uranus is the seventh planet from the Sun\n"
"and has the third-largest diameter in \n"
"our solar system. It was the first\n"
"planet found with the aid of a telescope,\n";

const std::string neptuneText =
"NEPTUNE\nLength of year: 60.190 Earth Days\nMoons: 14; Planet-type: Ice Giant\n"
"Dark, cold, and whipped by supersonic winds\n"
"ice giant Neptune is the eighth and most distant\n"
"planet in our solar system. Neptune is the\n"
"only planet in our solar system not visible\n"
"to the naked eye.";

const std::string deimosText =
"DEIMOS\nLength of year: 1.26 Earth Hours\nGreek god of dread\n"
"Deimos is the smaller of Mars' two moons.\n"
"Being only 9 by 7 by 6.8 miles in size\n"
"(15 by 12 by 11 kilometers), Deimos whirls\n"
"around Mars every 30 hours.";

const std::string phobosText =
"PHOBOS\nLength of year: 7.65 Earth Hours\nGreek god of fear\n"
"Phobos is the larger of Mars' two moons.\n"
"It orbits Mars three times a day\n"
"and is so close to the planet's surface \n"
"that in some locations on Mars it cannot always be seen.";

const std::string moonText =
"MOON\nLength of year: 27 Earth Days\nLatin for \"LUNA\"\n"
"Earth's Moon is the only place\n"
"beyond Earth where humans have set foot.\n"
"The moon is the brightest and \n"
"largest object in our night sky.";

const std::string callistoText =
"CALLISTO\nDiscovered: Jan 7, 1610\nWoman changed into bear by roman gods\n"
"Callisto is the third largest moon\n"
"in our solar system.Its surface is\n"
"the most heavily cratered of any \n"
"object in our solar system.";

const std::string ioText =
"IO\nDiscovered: Jan 8, 1610\nWoman changed into cow by roman gods\n"
"Jupiter's moon Io is the most volcanically\n"
"active world in the Solar System,\n"
"with hundreds of volcanoes, some erupting\n"
"lava fountains dozens of miles(or kilometers) high.";

const std::string europaText =
"EUROPA\nDiscovered: Jan 8, 1610\nWoman abducted by Zeus\n"
"Europa may be the most promising\n"
"place in our solar system to find\n"
"present - day environments suitable\n"
"for some form of life beyond Earth.";

const std::string titanText =
"TITAN\nBigger than Mercury\nChildren of greek gods\n"
"Titan is the only one with a substantial atmosphere.\n"
"Titan is also the only place besides\n"
"Earth known to have liquids in the\n"
"form of rivers, lakes and seas on its surface.";

const std::string iapetusText =
"IAPETUS\nDiscovered: Oct 25, 1671\nMythologiacl greek giant\n"
"Iapetus has been called the yin and yang\n"
"of the Saturn moons because its leading\n"
"hemisphere has a reflectivity as dark as coal\n"
"and its trailing hemisphere is much brighter.";

const std::string titaniaText =
"TITANIA\nDiscovered: Jan 11, 1787\nLiterally fairy queen\n"
"Titania is Uranus' largest moon.\n"
"Images taken by Voyager 2 almost 200\n"
"years after Titania's discovery revealed\n"
"signs that the moon was geologically active.";

std::map<int, std::string> mapTextsById;

void initTexts() 
{
	mapTextsById.insert(std::pair<int, std::string>(1, sunText));
	mapTextsById.insert(std::pair<int, std::string>(2, mercuryText));
	mapTextsById.insert(std::pair<int, std::string>(3, venusText));
	mapTextsById.insert(std::pair<int, std::string>(4, earthText));
	mapTextsById.insert(std::pair<int, std::string>(5, marsText));
	mapTextsById.insert(std::pair<int, std::string>(6, jupiterText));
	mapTextsById.insert(std::pair<int, std::string>(7, saturnText));
	mapTextsById.insert(std::pair<int, std::string>(8, uranusText));
	mapTextsById.insert(std::pair<int, std::string>(9, neptuneText));

	//sateliti
	mapTextsById.insert(std::pair<int, std::string>(41, moonText));
	mapTextsById.insert(std::pair<int, std::string>(51, phobosText));
	mapTextsById.insert(std::pair<int, std::string>(52, deimosText));
	mapTextsById.insert(std::pair<int, std::string>(61, ioText));
	mapTextsById.insert(std::pair<int, std::string>(62, callistoText));
	mapTextsById.insert(std::pair<int, std::string>(63, europaText));
	mapTextsById.insert(std::pair<int, std::string>(71, titanText));
	mapTextsById.insert(std::pair<int, std::string>(72, iapetusText));
	mapTextsById.insert(std::pair<int, std::string>(81, titaniaText));
}