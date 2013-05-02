/**
 * Strategy.cpp
 *
 * @date 2012/08/25
 * @author: katsumata
 */

#include "Strategy.hpp"

#include <sstream>
#include <stdexcept>
#include <cstring>
#include "Converter.hpp"

namespace spd {
namespace core {

/*
 * 長さ0, 戦略文字列がすべて空の戦略を作成
 */
Strategy::Strategy() : id(-1) {
}

/*
 * 指定された戦略文字列から、該当する戦略長さだけの戦略を作成
 */
Strategy::Strategy(const char* strategy, int minLen, int maxLen) : id(-1) {

	this->longStrategy = toLongStrategy(strategy, minLen, maxLen);

	this->shortStrategy = toShortStrategy(this->longStrategy);
}
/*
 * 文字配列を冗長な列挙型戦略列に変換
 * @param[in] strategy 戦略の文字配列
 * @param[in] length 戦略の長さ
 * @return 冗長な列挙型戦略列
 * @throw std::invalid_argument 先頭が数字の戦略の場合
 */
std::vector<Action> Strategy::toLongStrategy(const char* cStrategy, int iMinLen, int iMaxLen) {

	if ((iMinLen < 0) || (iMaxLen < 0)) {
		throw std::invalid_argument("The length minimum or maximum length of strategy is a minus.");
	}
	unsigned int minLen = static_cast<int>(iMinLen);
	unsigned int maxLen = static_cast<int>(iMaxLen);

	std::vector<Action> longStrategy;

	if (isdigit(cStrategy[0])) {
		std::string errStrategy (cStrategy);
		throw std::invalid_argument("The initial of strategy " + errStrategy + " is digit.");
	} else {
		longStrategy.push_back(spd::core::converter::charToActoin(cStrategy[0]));
	}

	//  繰り返し回数
	int repeatNum = 0;
	char preAction = cStrategy[0];

	// 長さが十分なら終了
	if (maxLen != 0 && longStrategy.size() >= maxLen) {
		return longStrategy;
	}

	// 文字配列をすべて走査してその分の冗長戦略文字列を作成
	for (int i = 1, len = strlen(cStrategy); i < len; i++) {

		char action = cStrategy[i];

		if (isdigit(action)) {
			// 数字の場合
			repeatNum = repeatNum * 10 + (action - '0');
		} else {
			if (1 < repeatNum) {
				// 繰り返し
				for (int j = 0; j < (repeatNum -1); ++j) {
					longStrategy.push_back(converter::charToActoin(preAction));

					// 戦略の長さが十分なら終了
					if (maxLen != 0 && longStrategy.size() >= maxLen) {
						return longStrategy;
					}
				}
			}

			longStrategy.push_back(converter::charToActoin(action));

			// 戦略の長さが十分なら終了
			if (maxLen != 0 && longStrategy.size() >= maxLen) {
				return longStrategy;
			}

			// 行動を記憶し、繰り返し回数をリセット
			preAction = action;
			repeatNum = 0;
		}
	}

	// 最後が数字の場合残りを出力
	if (repeatNum != 0) {
		for (int j = 0; j < (repeatNum - 1); j++) {
			longStrategy.push_back(converter::charToActoin(preAction));

			// 戦略の長さが十分なら終了
			if (maxLen != 0 && longStrategy.size() >= maxLen) {
				return longStrategy;
			}
		}
	}

	if (longStrategy.size() < minLen) {
		std::string errStrategy = cStrategy;
		throw std::invalid_argument("A strategy [" + errStrategy + "] is short. "
				"Please set a strategy that lenght is at least " + std::to_string(minLen) + ".");
	}

	return longStrategy;
}

/*
 * 冗長な列挙型戦略列を、略記文字列に変換
 * @param[in] longStrategy 冗長な列挙型戦略列
 * @return 略記文字列
 */
std::string Strategy::toShortStrategy(const std::vector<Action>& longStrategy) {
	Action firstAction = longStrategy.front();
	char preAction = converter::actionToChar(firstAction);

	// 略記戦略文字列
	shortStrategy += preAction;

	int repeatNum = 1;

	for (int i = 1, len = longStrategy.size(); i < len; i++) {

		char action = converter::actionToChar(longStrategy.at(i));

		if (action == preAction) {
			repeatNum += 1;
		} else {
			// 前の行動と異なる場合
			if (repeatNum > 1) {
				// 一回以上だったら繰り返しを出力
				shortStrategy += std::to_string(repeatNum);
				repeatNum = 1;
			}
			preAction = action;
			shortStrategy += preAction;
		}
	}

	// もし繰り返しが残っているなら追加
	if (repeatNum != 1) {
		shortStrategy += std::to_string(repeatNum);
	}

	return shortStrategy;
}

/*
 * 戦略が All 戦略かどうかを調べる
 * @return All戦略ならtrue, 違うならfalse
 */
bool Strategy::isAll(Action targetAction) const {
	for (auto action : longStrategy) {
		if (action != targetAction) {
			return false;
		}
	}
	return true;
}


} /* namespace core */
} /* namespace spd */
