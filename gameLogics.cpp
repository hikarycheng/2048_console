#include "gameLogics.hpp"

#include<cassert>
#include<utility>

namespace GameLogics {
	GameLogic::GameLogic(int size=4, int upperBound=2048)
		:boardSize_(size), upperBound_(upperBound), maxNumPieces_(size* size) {
		assert(size > 2); // "Board size should be greater than 0."
		this->board_ = boardTy(boardSize_, std::vector<int>(boardSize_, 0));
		std::random_device rd;
		genEngine_ = std::ranlux48(rd());
		distrib_ = std::uniform_int_distribution<> (0, maxNumPieces_-1);
	};

	GameLogic::GameLogic() :GameLogic(4, 2048) {};

	GameLogic& GameLogic::operator=(GameLogic const& other) noexcept {
		if (this == &other) {
			return *this;
		}

		this->boardSize_ = other.boardSize_;
		this->board_ = other.board_;
		this->currNumPieces_ = other.currNumPieces_;
		this->upperBound_ = other.upperBound_;
		this->maxNumPieces_ = other.maxNumPieces_;

		return *this;
	}

	GameLogic& GameLogic::operator=(GameLogic&& other) noexcept {
		if (this == &other) {
			return *this;
		}

		this->reset();

		std::swap(this->boardSize_, other.boardSize_);
		std::swap(this->board_, other.board_);
		std::swap(this->currNumPieces_, other.currNumPieces_);
		std::swap(this->upperBound_, other.upperBound_);
		std::swap(this->maxNumPieces_, other.maxNumPieces_);

		return *this;
	}

	void GameLogic::reset() noexcept {
		std::for_each(board_.begin(), board_.end(),
			[](auto& sub) {
				std::fill(sub.begin(), sub.end(), 0);
			});
		currNumPieces_ = 0;
	}

	bool GameLogic::move(const Directions direction) {
		this->currNumPieces_ = 0;
		bool moveFlag = false;
		switch (direction)
		{
		case Directions::left:
			moveFlag = this->moveHorizontal_(direction);
			break;
		case Directions::right:
			moveFlag = this->moveHorizontal_(direction);
			break;
		case Directions::up:
			moveFlag = this->moveVertical_(direction);
			break;
		case Directions::down:
			moveFlag = this->moveVertical_(direction);
			break;
		}

		return moveFlag;
	}

	bool GameLogic::moveVertical_(const Directions direction) noexcept {
		bool topDown = direction == Directions::up ? true : false;
		bool moveFlag = false;
		if (!topDown)
			std::reverse(this->board_.begin(), this->board_.end());
		for (auto col = 0; col < this->boardSize_; ++col) {
			moveFlag = this->scanVertical_(col) || moveFlag;
			moveFlag = this->mergeVertical_(col) || moveFlag;
		}

		if (!topDown)
			std::reverse(this->board_.begin(), this->board_.end());

		return moveFlag;
	}

	bool GameLogic::scanVertical_(std::size_t col) noexcept {
		bool moveFlag = false;
		for (auto row = 0; row < this->boardSize_; ++row) {
			if (!this->isOccupied_(std::make_pair(row, col)))
				continue;

			this->unmergePieces_.push_back(board_[row][col]);
			moveFlag = moveFlag || row >= unmergePieces_.size();
			board_[row][col] = 0;
		}

		return moveFlag;
	}

	bool GameLogic::mergeVertical_(std::size_t col) noexcept {
		if (this->unmergePieces_.empty())
			return false;

		int currPiece = unmergePieces_.front();
		this->unmergePieces_.pop_front();
		int idx = 0;
		bool moveFlag = false;
		while (!this->unmergePieces_.empty()) {
			if (currPiece != this->unmergePieces_.front() || currPiece == this->upperBound_) {
				board_[idx++][col] = currPiece;
			}
			else {
				moveFlag = true;
				board_[idx++][col] = currPiece * 2;
				this->unmergePieces_.pop_front();
				if (this->unmergePieces_.empty()) {
					++currNumPieces_;
					return moveFlag;
				}
			}

			++currNumPieces_;
			currPiece = this->unmergePieces_.front();
			this->unmergePieces_.pop_front();
		}

		board_[idx][col] = currPiece;
		++currNumPieces_;
		return moveFlag;
	}

	bool GameLogic::moveHorizontal_(const Directions direction) noexcept {
		bool leftToRight = direction == Directions::left ? true : false;
		bool moveFlag = false;
		for (auto row = 0; row < boardSize_;++row) {
			auto& rowVals = board_[row];
			if (!leftToRight)
				std::reverse(rowVals.begin(), rowVals.end());

			moveFlag = this->scanHorizontal_(row) || moveFlag;
			moveFlag = this->mergeHorizontal_(row) || moveFlag;
			if (!leftToRight)
				std::reverse(rowVals.begin(), rowVals.end());
		}

		return moveFlag;
	}

	bool GameLogic::scanHorizontal_(std::size_t row) noexcept {
		auto& rowVals = board_[row];
		bool moveFlag = false;
		for (auto col = 0; col < boardSize_;++col) {
			if (!this->isOccupied_(std::make_pair(row, col)))
				continue;

			moveFlag = moveFlag || (col > unmergePieces_.size());
			this->unmergePieces_.push_back(rowVals[col]);
			rowVals[col] = 0;
		}

		return moveFlag;
	}

	bool GameLogic::mergeHorizontal_(std::size_t row) noexcept {
		if (this->unmergePieces_.empty())
			return false;

		int currPiece = unmergePieces_.front();
		this->unmergePieces_.pop_front();
		int idx = 0;
		auto& rowVals = board_[row];
		bool moveFlag = false;
		while (!this->unmergePieces_.empty()) {
			if (currPiece != this->unmergePieces_.front()||currPiece== this->upperBound_) {
				rowVals[idx++] = currPiece;
			}
			else {
				moveFlag = true;
				rowVals[idx++] = currPiece * 2;
				this->unmergePieces_.pop_front();
				if (this->unmergePieces_.empty()) {
					++currNumPieces_;
					return moveFlag;
				}
			}

			++currNumPieces_;
			currPiece = this->unmergePieces_.front();
			this->unmergePieces_.pop_front();
		}

		rowVals[idx] = currPiece;
		++currNumPieces_;

		return moveFlag;
	}

	inline bool GameLogic::isOccupied_(tileTy& tile) const noexcept {
		return board_[tile.first][tile.second] != nonOccupied_;
	}

	inline bool GameLogic::isOccupied_(tileTy&& tile) const noexcept {
		return board_[tile.first][tile.second] != nonOccupied_;
	}

	bool GameLogic::generateTile() noexcept {
		if (this->isGameOver()) {
			return false;
		}

		int randint = this->generateRandomInt_();
		auto tile = std::make_pair(randint / boardSize_, randint % boardSize_);
		int generatedNum = this->generateCanonical_() < 0.5 ? 2 : 4;
		if (this->isOccupied_(tile)) {
			tile = this->searchEmptyTile_(tile);
		}

		this->emplaceTile_(tile, generatedNum);
		++this->currNumPieces_;
		return true;
	}

	void GameLogic::emplaceTile_(tileTy& tile, int value) noexcept {
		board_[tile.first][tile.second] = value;
	}

	inline tileTy& GameLogic::searchEmptyTile_(tileTy& tile) {
		if (this->currNumPieces_ < this->maxNumPieces_ / 2) {
			while (this->isOccupied_(tile)) {
				int randint = this->generateRandomInt_();
				tile.first = randint / boardSize_, tile.second = randint % boardSize_;
			}
		}
		else {
			std::vector<std::pair<int, int>> emptyGrids;
			for (auto i = 0; i < boardSize_; ++i) {
				for (auto j = 0; j < boardSize_; ++j) {
					auto newTile = std::make_pair(i, j);
					if (!this->isOccupied_(newTile))
						emptyGrids.emplace_back(std::move(newTile));
				}
			}

			std::uniform_int_distribution<> randInt(0, emptyGrids.size() - 1);
			auto& chosen = emptyGrids[randInt(this->genEngine_)];
			tile.first = chosen.first, tile.second = chosen.second;
		}

		return tile;
	}

	int GameLogic::generateRandomInt_() noexcept {
		return this->distrib_(this->genEngine_);
	}

	double GameLogic::generateCanonical_() noexcept {
		return std::generate_canonical<double, 10>(genEngine_);
	}

	bool GameLogic::isGameOver() const noexcept {
		return this->currNumPieces_ >= this->maxNumPieces_;
	}

	boardTy const& GameLogic::getBoard() const noexcept {
		return board_;
	}

	void GameLogic::display() const noexcept {
		for (auto const& row : board_) {
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
		std::cout << "=========================================\n";
		std::for_each(unmergePieces_.begin(), unmergePieces_.end(), 
			[](auto& e) {std::cout << e << ' '; });
		std::cout << '\n';
		std::cout << "=========================================\n";
	}
}