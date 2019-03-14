// Golf_Giomi_Kainoa.cpp
//Author: Kainoa Giomi

/* Here I have created a two-player golf game, which pits the two players against each other
 * in a competition to win the most holes in the game. The course is 5 holes long, with varying
 * pars and yard lengths. To win a hole, a player must score less than the other player does.
 * In the end, the player who wins the most holes wins the game. The game can also result in a tie.
 * There are 6 clubs total in the game, with varying ranges of distance that they can hit. The
 * distance each player hits with a club in a single turn is a random value between the minimum and
 * maximum distance values for the chosen club. There are also wind and bunkers throughout the course.
 * The bunkers can only be hit out of by a single club, the “Wedge”, and the wind affects the distances
 * hit. The chances of landing in a bunker increase as the distance hit increases. The “Old Faithful”
 * club has a massive range for distance hit (0 to 500 yards to be specific), which can either really
 * help or hurt the player using it. The green of the hole is within 20 yards of the hole on both sides
 * of it, where the “Putter” should be used. The putter can be used off the green as well, where it will
 * hit 20 yards every time. */

#include <iostream>
#include <ctime> //for time()
#include <string>
#define PAR_TOTAL 22 //total strokes for getting a Par, or being even

using namespace std;

struct Holes{ //struct for each hole's par number and yardage
	string par;
	int yards;
};
//array with the 5 holes of the courses
Holes h[5] = {{"Par 4", 304},
			{"Par 5", 545},
			{"Par 3", 193},
			{"Par 4", 350},
			{"Par 6", 717}
		};

struct Clubs{ //struct for the usable clubs for each player and the min and max each club can hit
	string club;
	int minyard;
	int maxyard;
};
//array with 5 clubs
Clubs c[5] = {{"Driver", 190, 260},
			{"Big Iron", 145, 210},
			{"Small Iron", 85, 155},
			{"Wedge", 30, 100},
			{"Old Faithful", 0, 500}
		};

void bunker(int &dist, int &bunker); //function that determines whether player has hit into a bunker
int input_validation(int bunkflag, int &clubpick); //function for input validation
void playerlocation(int dist, int &bunkflag); //function which reads the situation where the player is at the start of the turn
void putter(int &dist, int &hole); //function for the 6th club, the Putter
void scorecard(int score1, int score2); //function that prints each player's score for the game

int main(int argc, const char * argv[]){
	if(argc != 1){ //argc if statement taken from Lab #7, adjusted to fit C++
		cout << "Usage: " << argv[0] << endl;
		return 1;
	}
	srand(time(NULL));
	int wind, yardsaway1, yardsaway2 /*yards away from */, clubdist /*distance hit*/,
		clubchoice, bunk1 = 0, bunk2 = 0 /* bunker flag for players*/, temp1, temp2 /*number of strokes for current hole per player*/,
		tot1 = 0, tot2 = 0 /*total strokes per player*/, count1 = 0, count2 = 0 /*hole wins per player*/;

	//Instructions for the players
	cout << "Welcome to the golf course!\n" << endl;
	cout << "There are a total of 5 holes on this course, and the player that wins the most holes, wins the game." << endl;
	cout << "Each player is provided with the following six clubs, with the ranges of distance each club can hit:\n" << endl;
	for(int i = 0; i < 5; i++){
		cout << i+1 << ". " << c[i].club << ", which ranges from " << c[i].minyard << " to " << c[i].maxyard << " yards." << endl;
	}
	cout << "6. Putter, which is to be used on the green, which is when the ball is within 20 yards of the hole.\n" << endl;
	cout << "To choose a club to use, simply pick the number of the club you would like to use.\n" << endl;
	cout << "Pro tips:" << endl;
	cout << "1. Using the Old Faithful is risky, but it can give you the perfect edge you need to win a hole. " << endl;
	cout << "2. The Putter can be used off the green, where it will hit the ball 20 yards every time." << endl;
	cout << "3. There is also wind and bunkers throughout the course. The bunkers can only be hit out of by the Wedge, "
			"\n   and the wind affects the distance hit by each club." << endl;
	cout << "4. The further the ball is hit, the more likely the ball will end up in a bunker." << endl;
	cout << "\nGood luck and play ball!\n" << endl;

	for(int i = 0; i < 5; i++){ //loops 5 times, each loop represents one hole
		//Hole and its respective yard length is printed
		cout << "Hole " << i+1 << " is a " << h[i].par << " that is " << h[i].yards << " yards long." << endl;
		//each player starts at the yard length for the hole they are on
		yardsaway1 = h[i].yards;
		yardsaway2 = h[i].yards;
		/*wind is anywhere from 1 to 20 mph, going either with or against the ball, which
		  affects the distance each player hits with every club, other than the putter*/
		wind = rand() % 40 + 1;
		if(wind > 0 && wind <= 20) // 1-20 means 1-20 mph winds against the ball
			cout << "The wind is going " << wind << " mph against the ball." << endl;
		else if(wind > 20 && wind <= 40) // 21-40 means 1-20 mph winds against the ball
			cout << "The wind is going " << wind-20 << " mph with the ball." << endl;
		temp1 = 0; //Player 1's number of strokes for the hole is set to 0
		temp2 = 0; //Player 2's number of strokes for the hole is set to 0
		do {
			if(yardsaway1 != 0){
				cout << "\nPlayer 1 is ";
				playerlocation(yardsaway1, bunk1); //prints where Player 1 is on the course
				input_validation(bunk1, clubchoice); //Player 1 prompted to choose a club, with input validation
				if(clubchoice == 6){ //if Player 1 picks the Putter
					putter(yardsaway1, temp1); //Putter function
				}
				else{
					//int clubdist is a random value between Player 1's chosen club's minimum and maximum for each swing taken
					clubdist = rand() % (c[clubchoice-1].maxyard-c[clubchoice-1].minyard) + c[clubchoice-1].minyard + 1;
					if(wind > 0 && wind <= 20) //if wind is going against the ball, 1% of Player 1's clubdist times the specified wind is subtracted from the original clubdist
						clubdist -= (clubdist/100)*wind;
					else if(wind > 20 && wind <= 40) //if wind is going with the ball, 1% of Player 1's clubdist times the specified wind is added to the original clubdist
						clubdist += (clubdist/100)*(wind-20);
					/*distance hit by Player 1 is taken away from Player 1's yards away from the hole.
					  The absolute value is there so if Player 1 hits over the hole, their distance away from the hole is still positive*/
					yardsaway1 = abs(yardsaway1-clubdist);
					bunker(clubdist, bunk1); //decides if Player 1 has hit into a bunker
					temp1++; //Player 1's number of strokes for the hole is incremented by 1
				}
			}
			else if(yardsaway1 == 0) //states Player 1 is waiting if they have already finished the hole
				cout << "\nPlayer 1 is waiting..." << endl;

			if(yardsaway2 != 0){
				cout << "\nPlayer 2 is ";
				playerlocation(yardsaway2, bunk2); //prints where Player 2 is on the course
				input_validation(bunk2, clubchoice); //Player 2 prompted to choose a club, with input validation
				if(clubchoice == 6) //if Player 2 picks the Putter
					putter(yardsaway2, temp2); //Putter function
				else{
					//int clubdist is a random value between Player 2's chosen club's minimum and maximum for each swing taken
					clubdist = rand() % (c[clubchoice-1].maxyard-c[clubchoice-1].minyard) + c[clubchoice-1].minyard + 1;
					if(wind > 0 && wind <= 20) //if wind is going against the ball, 1% of Player 2's clubdist times the specified wind is subtracted from the original clubdist
						clubdist -= (clubdist/100)*wind;
					else if(wind > 20 && wind <= 40) //if wind is going with the ball, 1% of Player 2's clubdist times the specified wind is added to the original clubdist
						clubdist += (clubdist/100)*(wind-20);
					/*distance hit by Player 2 is taken away from Player 1's yards away from the hole.
					  The absolute value is there so if Player 2 hits over the hole, their distance away from the hole is still positive*/
					yardsaway2 = abs(yardsaway2-clubdist);
					bunker(clubdist, bunk2); //decides if Player 2 has hit into a bunker
					temp2++; //Player 2's number of strokes for the hole is incremented by 1
				}
			}
			else if(yardsaway2 == 0) //states Player 2 is waiting if they have already finished the hole
				cout << "\nPlayer 2 is waiting..." << endl;
		}while(yardsaway1 != 0 || yardsaway2 != 0); //breaks when both players are done with the hole
		//"stroke" if hole-in-one, "strokes" if not
		string stroke1, stroke2;
		stroke1 = (temp1 == 1) ? "stroke" : "strokes";
		stroke2 = (temp2 == 1) ? "stroke" : "strokes";
		//player scores for the hole are printed
		cout << "\nPlayer 1 took " << temp1 << " " << stroke1 <<" to finish the hole." << endl;
		cout << "Player 2 took " << temp2 << " " << stroke2 << " to finish the hole.\n" << endl;
		//player scores for the hole are added to their total score for the game
		tot1 += temp1;
		tot2 += temp2;
		//if Player 1 takes less strokes to finish the hole than Player 2, Player 1 wins the round and their hole win total is incremented by 1
		if(temp1 < temp2)
			count1++;
		//if Player 2 takes less strokes to finish the hole than Player 1, Player 2 wins the round and their hole win total is incremented by 1
		else if(temp2 < temp1)
			count2++;
	}
	//the Player with the most hole wins takes the game and the holes won for each player is printed
	if(count1 > count2){
		cout << "Player 1 wins " << count1 << " rounds to " << count2 << "!\n" << endl;
		scorecard(tot1, tot2); //score board for the game is printed
	}
	else if(count2 > count1){
		cout << "Player 2 wins " << count1 << " rounds to " << count2 << "!\n" << endl;
		scorecard(tot1, tot2); //score board for the game is printed
	}
	else{
		//If both players win the same amount of holes, the game ends in a tie
		cout << "It's a Tie!\n" << endl;
		scorecard(tot1, tot2); //score board for the game is printed
	}
	return 0;
}

void bunker(int &dist, int &bunker){
	int chance = rand() % 1000 + 1;
	//if the distance the player hit is above or the same as the random value from 1 to 1000, then the player is in the bunker
	if(chance <= dist)
		bunker = 1; //Player is in the bunker
	else
		bunker = 0; //Player is not in the bunker
}

int input_validation(int bunkflag, int &clubpick){
	if(bunkflag == 1){
		cout << "Player must use the Wedge to get out: "; //input validation when the player is in the bunker
		while(!(cin >> clubpick) || clubpick != 4){
			cout << "The sand doesn't approve! You must use the wedge (must choose 4): ";
			cin.clear();
		while(cin.get() != '\n')
			continue;
		}
		return clubpick;
	}
	else{
		cout << "Which club would the player like to use? "; //input validation when the player is not in the bunker
		while(!(cin >> clubpick) || clubpick < 1 || clubpick > 6){
			cout << "Not a club! You must use a club from the choices below: " << endl;;
			for(int i = 0; i < 5; i++){ //clubs with respective yard ranges are printed in case the player forgets the available clubs
				cout << i+1 << ". " << c[i].club << endl;
				cout << "   Yard Range: " << c[i].minyard << " to " << c[i].maxyard << " yards"<< endl;
			}
			cout << "6. Putter" << endl;
			cout << "Which club would the player like to use? ";
			cin.clear();
			while(cin.get() != '\n')
				continue;
		}
		return clubpick;
	}
}

void playerlocation(int dist, int &bunkflag){
	if(dist > 20){ //player not on the green
		if(bunkflag == 0)
			//not in bunker
			cout << dist << " yards away from the hole." << endl;
		else
			//in bunker
			cout << "in a bunker, " << dist << " yards away from the hole." << endl;
	}
	else{ //player on the green
		bunkflag = 0; //makes it so player cannot be in the bunker when on the green
		cout << "on the green, " << dist << " yards away from the hole" << endl;
	}
}

void putter(int &dist, int &hole){
	//within 10 yards of the hole
	if(dist <= 10){
		hole++; //player's number of strokes for the hole is incremented by 1
		dist = 0; //putt is made, hole finished
	}
	//10-20 yards away from the hole
	else if(dist > 10 && dist <= 20){
		hole++; //player's number of strokes for the hole is incremented by 1
		int hardputt = rand() % 2 + 1;
		//50% chance to make putt
		if(hardputt == 2)
			//putt is made, hole finished
			dist = 0;
		else
			//ball is in a random place, 1-10 yards away from the hole
			dist = rand() % 10 + 1;
	}
	//20 or more yards away from the hole
	else if(dist > 20){
		hole++; //player's number of strokes for the hole is incremented by 1
		dist -= 20; //putts 20 yards towards the hole
	}
}

void scorecard(int total1, int total2){
	int score1 = total1 - PAR_TOTAL; //Player 1's plus/minus for the game is printed
	int score2 = total2 - PAR_TOTAL; //Player 2's plus/minus for the game is printed
	//Plus/minus is how much each player scored over or under par
	//score board for the game is printed out
	cout << "          Score Totals   Par Totals   Plus/Minus" << endl;
	cout << "Player 1       " << total1 << "            " << PAR_TOTAL << "           " << score1 << endl;
	cout << "Player 2       " << total2 << "            " << PAR_TOTAL << "           " << score2 << endl;
}
