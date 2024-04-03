#ifndef GAME_LOGICS_HPP
#define GAME_LOGICS_HPP

#include<vector>
#include<queue>
#include<iostream>
#include <random>

namespace GameLogics {
	using boardTy = std::vector<std::vector<int>>;
	using tileTy = std::pair<int, int>;
	
	enum class Directions {left, right, up, down};

	class GameLogic {
	public:
		GameLogic(int=4, int=2048);
		// GameLogic();
		bool move(const Directions);
		bool generateTile() noexcept;
		bool isGameOver() const noexcept;
		boardTy const& getBoard() const noexcept;
		void reset() noexcept;

		void display() const noexcept;

		GameLogic& operator=(GameLogic const&) noexcept;
		GameLogic& operator=(GameLogic&&) noexcept;

	private:
		bool moveHorizontal_(const Directions) noexcept;
		bool moveVertical_(const Directions) noexcept;
		bool scanHorizontal_(std::size_t) noexcept;
		bool scanVertical_(std::size_t) noexcept;
		bool mergeHorizontal_(std::size_t) noexcept;
		bool mergeVertical_(std::size_t) noexcept;
		int generateRandomInt_() noexcept;
		double generateCanonical_() noexcept;
		inline tileTy& searchEmptyTile_(tileTy&);
		void emplaceTile_(tileTy&, int) noexcept;
		inline bool isOccupied_(tileTy&) const noexcept;
		inline bool isOccupied_(tileTy&&) const noexcept;

		int boardSize_;
		int upperBound_;
		boardTy board_;
		std::deque<int> unmergePieces_;
		int maxNumPieces_;
		int currNumPieces_ = 0;
		const int nonOccupied_ = 0;
		 //Will be used to obtain a seed for the random number engine
		std::ranlux48  genEngine_; 
		std::uniform_int_distribution<> distrib_;		
	};

	
}


#endif // !GAME_LOGICS_HPP
