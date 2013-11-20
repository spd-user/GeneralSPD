/**
 * SpdRule.hpp
 *
 * @date 2012/09/08
 * @author katsumata
 */

#ifndef SPDRULE_H_
#define SPDRULE_H_

#include <string>
#include <vector>
#include <thread>
#include <functional>

#include "Rule.hpp"
#include "../param/Parameter.hpp"

namespace spd {
namespace rule {

class SpdRule : public IToString {
public:

	/**
	 * コンストラクタ
	 * @param[in] name ルール名
	 */
	SpdRule(std::string name);

	/**
	 * ルール特有の初期化を実行
	 * @param[in] allPlayers 全てのプレイヤ
	 * @param[in] param パラメタ
	 */
	void init(
		const AllPlayer& allPlayers,
		const spd::param::Parameter& param) const;

	/**
	 * 前処理ルールを順番に実行
	 * @param[in] allPlayers 全てのプレイヤ
	 * @param[in] param パラメタ
	 * @param[in] step 実行ステップ
	 */
	void runRulesBeforeOutput(
		const AllPlayer& allPlayers,
		const spd::param::Parameter& param,
		int step) const {

		int core = param.getCore();
		std::vector<std::thread> thr(core);

		// 順番に処理
		for (auto& rule : rulesBeforeOutput) {

			// 1コアが担当するプレイヤ数
			int playerNum = allPlayers.size();
			int breadth = playerNum / core;

			for (int i = 0, size = thr.size(); i < size; ++i) {
				thr[i] = std::thread(
						[&, i]{

					int from = breadth * i;
					int to = (i + 1 < core) ? breadth * (i + 1) : playerNum;
					for (int id = from; id < to; ++id) {
						rule->runRule(allPlayers[id], allPlayers, param, step);
					}
				}
				);
			}

			for (std::thread& t : thr) {
				t.join();
			}
		}
	}

	/**
	 * 後処理ルールを順番に実行
	 * @param[in] allPlayers 全てのプレイヤ
	 * @param[in] param パラメタ
	 * @param[in] step 実行ステップ
	 */
	void runRulesAfterOutput(
		const AllPlayer& allPlayers,
		const spd::param::Parameter& param,
		int step) const {

		int core = param.getCore();
		std::vector<std::thread> thr(core);

		// 順番に処理
		for (auto& rule : rulesAfterOutput) {

			// 1コアが担当するプレイヤ数
			int playerNum = allPlayers.size();
			int breadth = playerNum / core;

			for (int i = 0, size = thr.size(); i < size; ++i) {
				thr[i] = std::thread(
						[&, i]{

					int from = breadth * i;
					int to = (i + 1 < core) ? breadth * (i + 1) : playerNum;
					for (int id = from; id < to; ++id) {
						rule->runRule(allPlayers[id], allPlayers, param, step);
					}
				}
				);
			}

			for (std::thread& t : thr) {
				t.join();
			}
		}
	}

	/**
	 * 前処理ルールを追加
	 */
	void addRuleBeforeOutput(const std::shared_ptr<Rule>& rule) {
		this->rulesBeforeOutput.push_back(rule);
	}

	/**
	 * 前処理ルールの取得
	 * @return 前処理ルール
	 */
	const std::vector<std::shared_ptr<Rule>>& getRulesBeforeOutput() const {
		return rulesBeforeOutput;
	}

	/**
	 * 後処理ルールを追加
	 */
	void addRuleAfterOutput(const std::shared_ptr<Rule>& rule) {
		this->rulesAfterOutput.push_back(rule);
	}

	/**
	 * 後処理ルールの取得
	 * @return 後処理ルール
	 */
	const std::vector<std::shared_ptr<Rule>>& getRulesAfterOutput() const {
		return rulesAfterOutput;
	}

	/**
	 * ルール名の出力
	 */
	const std::string getName() const {
		return this->name;
	}

	/**
	 * ルール内容の出力
	 * @return ルール内容
	 */
	std::string toString() const;

private:

	/**
	 * ルール名
	 */
	std::string name;

	/**
	 * 前処理ルール
	 */
	std::vector<std::shared_ptr<Rule>> rulesBeforeOutput;

	/**
	 * 後処理ルール
	 */
	std::vector<std::shared_ptr<Rule>> rulesAfterOutput;
};

} /* namespace core */
} /* namespace spd */

#endif /* SPDRULE_H_ */
