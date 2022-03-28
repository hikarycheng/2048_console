#ifndef GAME_STEM_HPP
#define GAME_STEM_HPP

#include "gameLogics.hpp"
#include <iostream>
#include <unordered_map>

namespace GameStem {

	enum class Operations {left='a', right='d', up='w', down='s', restart='r', exit='e' };

	class Game {
	public:
		Game(int boardSize = 4, int maxNum = 2048) 
			:boardSize_(boardSize), maxNum_(maxNum), logics_(boardSize_, maxNum_) {

			currScore_ = 0;
			maxScore_ = 0;
		}

		void newGame() {
			logics_.reset();
			gameOvered_ = false;
			currScore_ = 0;
			logics_.generateTile();
		}

		void oneStep(Operations operation) {
			bool flag = false;
			switch (operation)
			{
			case Operations::left:
				flag = logics_.move(GameLogics::Directions::left);
				break;
			case Operations::right:
				flag = logics_.move(GameLogics::Directions::right);
				break;
			case Operations::up:
				flag = logics_.move(GameLogics::Directions::up);
				break;
			case Operations::down:
				flag = logics_.move(GameLogics::Directions::down);
				break;
			}

			if (flag) {
				++currScore_;
				maxScore_ = std::max(currScore_, maxScore_);
			}

			if (logics_.isGameOver()) {
				currScore_ = 0;
				gameOvered_ = true;
				return;
			}
			if (flag)
				logics_.generateTile();
		}

		void play() {
			this->newGame();
			this->display();
			char operation;

			while (true) {
				while (gameOvered_) {
					std::cout << "Game over, please press R to restart a new game\n";
					std::cin >> operation;
					if (operation == static_cast<char>(Operations::restart)) {
						newGame();
						this->display();
						break;
					}
				}

				std::cin >> operation;
				if (operation == static_cast<char>(Operations::exit)) {
					break;
				}
				if (operation == static_cast<char>(Operations::restart)) {
					this->newGame();
				}
				else if (operation == static_cast<char>(Operations::down)
						|| operation == static_cast<char>(Operations::right)
						|| operation == static_cast<char>(Operations::left)
						|| operation == static_cast<char>(Operations::up)) {

					oneStep(static_cast<Operations>(operation));
				}
				this->display();
			}
		}

		void display() const {
			system("cls");
			std::cout << "higest score is: " << maxScore_
					<< ", current score is: "<< currScore_ << '\n';
			std::cout << '\n';
			std::cout << "Press A to move left, D to move right, W to move up, S to move down\n";
			std::cout << "To restart a new game, please press R\n";
			std::cout << "To exit the game, please press E\n";
			std::cout << '\n';
			std::cout << '\n';
			std::cout << '\n';

			auto const& board = logics_.getBoard();

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
			//std::cout << "=========================================\n";
			std::cout << '\n';
			std::cout << "Move: ";

		}

	private:
		int boardSize_, maxNum_;
		int maxScore_;
		GameLogics::GameLogic logics_;
		int currScore_;
		bool gameOvered_ = false;
	};
}

#endif // !GAME_STEM_HPP
