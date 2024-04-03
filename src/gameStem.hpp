#ifndef GAME_HPP
#define GAME_HPP

#include "gameLogics.hpp"
#include <iostream>
#include <unordered_map>

namespace Game {
	enum class Operations { left = 'a', right = 'd', up = 'w', down = 's', restart = 'r', exit = 'e' };

	namespace {
		static int currScore = 0, maxScore = 0;
		static GameLogics::GameLogic logics;
		static bool gameOvered = false;
	}

	void newGame() {
		logics.reset();
		gameOvered = false;
		currScore = 0;
		logics.generateTile();
		logics.generateTile();
	}

	template<typename ty>
	ty getMaxscore(ty curMaxScore, ty curScore) {
		return std::max(curMaxScore, curScore);
	}

	void oneStep(Operations operation) {
		bool flag = false;
		switch (operation)
		{
		case Operations::left:
			flag = logics.move(GameLogics::Directions::left);
			break;
		case Operations::right:
			flag = logics.move(GameLogics::Directions::right);
			break;
		case Operations::up:
			flag = logics.move(GameLogics::Directions::up);
			break;
		case Operations::down:
			flag = logics.move(GameLogics::Directions::down);
			break;
		case Operations::exit:
		case Operations::restart:
			break;
		}

		if (flag) {
			++currScore;
			maxScore = getMaxscore(maxScore, currScore);
		}

		if (logics.isGameOver()) {
			gameOvered = true;
			return;
		}
		if (flag)
			logics.generateTile();
	}

	void display() {
		system("clear");
		std::cout << "higest score is: " << maxScore
			<< ", current score is: " << currScore << '\n';
		std::cout << '\n';
		std::cout << "Press A to move left, D to move right, W to move up, S to move down\n";
		std::cout << "To restart a new game, please press R\n";
		std::cout << "To exit the game, please press E\n";
		std::cout << '\n';
		std::cout << '\n';
		std::cout << '\n';

		auto const& board = logics.getBoard();

		for (auto const& row : board) {
			for (auto const& tile : row) {
				if (tile == 0) {
					std::cout << ".\t";
				}
				else {
					std::cout << tile << '\t';
				}
			}

			std::cout << '\n';
		}

		std::cout << '\n';
		std::cout << "Move: ";

	}

	void game(int boardSize = 4, int maxNum = 2048) {
		logics = GameLogics::GameLogic(boardSize, maxNum);
		newGame();
		display();
		char operation;

		while (true) {
			while (gameOvered) {
				std::cout << "Game over, please press R to restart a new game\n";
				std::cin >> operation;
				if (operation == static_cast<char>(Operations::restart)) {
					newGame();
					display();
					break;
				}
			}

			std::cin >> operation;
			if (operation == static_cast<char>(Operations::exit)) {
				break;
			}
			if (operation == static_cast<char>(Operations::restart)) {
				maxScore = getMaxscore(maxScore, currScore);
				newGame();
			}
			else if (operation == static_cast<char>(Operations::down)
				|| operation == static_cast<char>(Operations::right)
				|| operation == static_cast<char>(Operations::left)
				|| operation == static_cast<char>(Operations::up)) {

				oneStep(static_cast<Operations>(operation));
			}
			display();
		}
	}
}

#endif // !GAME_HPP
