/**
 * Space.cpp
 *
 * @date 2012/09/08
 * @author katsumata
 */
 
 // ///////////////////////////
#include <unistd.h>
// ///////////////////////////

#include "Space.hpp"

#include <iostream>
#include <iomanip>
#include <ratio> // 時間
#include <random>
#include <stdexcept>
#include <thread>
#include <functional>

#include "NeighborhoodType.hpp"

#include "Player.hpp"
#include "maker/PlayerMaker.hpp"

#include "../rule/SpdRule.hpp"

#include "../param/Parameter.hpp"
#include "../param/InitParameter.hpp"
#include "../param/OutputParameter.hpp"
#include "../param/RandomParameter.hpp"

#include "../topology/Topology.hpp"

#include "../output/Output.hpp"
#include "../output/Compressor.hpp"
#include "../output/FileSystemOperation.hpp"

using namespace std::chrono;

namespace spd {
namespace core {

/*
 * パラメタとルールを設定して盤面を作成
 */
Space::Space(spd::param::Parameter& param) :
		parameter(param), step(0), sim(0) , skipBeforeRules(false),
		startTime(std::chrono::system_clock::now()) {

	// 乱数の除去
	param.getRandomParameter()->getEngine().discard(param.getRandomParameter()->getDiscardNum());

	this->spdRule = param.getInitialParameter()->getSpdRule();

	// プレイヤの作成
	std::cout << "making players." << std::endl;
	this->players = std::move(param.getPlayerMaker()->makePlayers());

	// パラメタの保存
	param.store();
}

/*
 * 1シミュレーションの実行
 */
bool Space::run() {
	if (sim != 0) {
		// トポロジーの初期化
		this->parameter.getPlayerMaker()->resetTopology(this->players);
	}
	step = 0;

	// プレイヤの初期化
	parameter.restore();
	this->parameter.getPlayerMaker()->initPlayer(this->players, *this);

	// 出力の初期化
	for (auto output : parameter.getOutputParameter()->getOutputs()) {
		std::get<0>(output)->init(*this, parameter);
	}


#ifdef DEBUG
	for (int i = 0; i < players.size(); ++i) {
		std::cout << "player: " << players[i]->getId() << ", degree: " << players[i]->getLinkedPlayers()->size() << "\n";
	}
	std::cout << std::endl;
#endif




	// 開始前の出力
	auto outputResults = output();
	// 圧縮
	std::vector<std::thread> thr;
	for (auto& outputResult : outputResults) {
		thr.push_back(std::thread(
				[&]{
			if (outputResult.second) {
				std::string outputFile (outputResult.first + spd::output::compressor::COMPRESS_TYPE);
				if (spd::output::compressor::compress(outputResult.first, outputFile)) {
					// 圧縮できたら元のファイルを削除
					spd::output::FileSystemOperation fso;
					fso.removeFile(outputResult.first);
				}
			}
		}
		));
	}
	for (std::thread& t : thr) {
		t.join();
	}

	auto endStep = parameter.getInitialParameter()->getEndStep();
	while(step < endStep) {
		execStep();
	}
	sim++;

	if (sim == this->parameter.getInitialParameter()->getSimCount()) {
		auto usedTime = duration_cast<seconds>(system_clock::now() - startTime);

		std::cout << "Used " << duration_cast<hours>(usedTime).count() << ":" <<
				duration_cast<minutes>(usedTime).count() % 60 << ":" <<
				usedTime.count() %  60 << "    \n";

		return true;
	}
	return false;
}

/*
 * 出力
 */
inline Space::OutputResultType Space::output() {
	auto& outputs = parameter.getOutputParameter()->getOutputs();

	int outputsNum = outputs.size();

	// 出力のスレッド化
	std::vector<std::thread> thr(outputsNum);

	// 出力結果を圧縮するかどうか
	OutputResultType outputResults (outputsNum);

	for (int i = 0; i < outputsNum; ++i) {
		auto& output = outputs.at(i);
		thr[i] = std::thread(
					[&, i]{
				if ((step == 0) ||
					// 開始ステップ以上、終了ステップ未満(整数の場合)かつ、間隔のステップの場合
					((std::get<1>(output) <= step) &&
						((std::get<2>(output) < 0) || (std::get<2>(output) > step)) &&
						((step - std::get<1>(output)) % std::get<3>(output) == 0))) {
				// 出力
				outputResults.at(i) = std::get<0>(output)->output(*this);
			}
		}
		);
	}
	for (std::thread& t : thr) {
		t.join();
	}

	return outputResults;
}

/*
 * 1ステップ実行
 */
inline void Space::execStep() {

	OutputResultType outputResults;

	if (!skipBeforeRules) {
		// 表示前処理
		this->spdRule->runRulesBeforeOutput(players, parameter, step);

		// ステップを進める
		++step;

		// 出力
		outputResults = output();
		
		// 進捗の表示
		printProgress();

	}

	// 圧縮開始
	std::vector<std::thread> thr;
	for (auto& outputResult : outputResults) {
		thr.push_back(std::thread(
				[&]{
			if (outputResult.second) {
				std::string outputFile (outputResult.first + spd::output::compressor::COMPRESS_TYPE);
				if (spd::output::compressor::compress(outputResult.first, outputFile)) {
					// 圧縮できたら元のファイルを削除
					spd::output::FileSystemOperation fso;
					fso.removeFile(outputResult.first);
				}
			}
		}
		));
	}
	
	skipBeforeRules = false;

	// 表示後処理
	this->spdRule->runRulesAfterOutput(players, parameter, step);

	// 圧縮はここまでに終わればいい
	for (std::thread& t : thr) {
		t.join();
	}
	
}

/*
 * 進捗の表示
 */
inline void Space::printProgress() const {

	// 終了ステップ
	auto endStep = this->parameter.getInitialParameter()->getEndStep();

	// シミュレーション回数
	auto simCount = this->parameter.getInitialParameter()->getSimCount();

	// 進捗度
	double progress = 100.0 * step / endStep;

	std::cout << std::setw(5) << std::setfill('0') << step << " [";

	int remain = 20 - static_cast<int>(progress / 5);
	for (int j = 0; j < (20 - remain); ++j) {
		std::cout << "=";
	}
	if (remain != 0) {
		std::cout << ">";
		for (int i = 1; i < remain; ++i) {
			std::cout << "-";
		}
	}

	// 1ステップあたりの時間(マイクロ秒)
	auto oneStepTime = static_cast<microseconds>(
			duration_cast<microseconds>(system_clock::now() - startTime).count() /
			(sim * endStep + step));

	// 推定完了時間
	auto estimatedTimeToComplete =
			duration_cast<seconds>(
				static_cast<microseconds>(
					// 残りステップ で時間を予測
					((simCount - sim -1) * endStep + (endStep - step)) * oneStepTime
				)
			);

	// 改行は行わずに復帰する
	std::cout << "] " << std::setprecision(2) << std::setiosflags(std::ios::fixed)
			<< progress << "% (" << sim + 1 << "/" << simCount << ") @ ETC " <<
			duration_cast<hours>(estimatedTimeToComplete).count() << ":" <<
			duration_cast<minutes>(estimatedTimeToComplete).count() % 60 << ":" <<
			duration_cast<seconds>(estimatedTimeToComplete).count() % 60 << " (" <<
			duration_cast<minutes>(oneStepTime).count() << "m" <<
			duration_cast<seconds>(oneStepTime).count() % 60 << "s/step)    \r" << std::flush;

}


} /* namespace core */
} /* namespace spd */
