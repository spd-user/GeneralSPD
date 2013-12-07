/**
 * GenerateTopology.cpp
 *
 * @date 2012/12/31
 * @author katsumata
 */

#include <stdexcept>
#include <algorithm>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp> // is_any_of
#include "GenerateTopology.hpp"
#include "../topology/AllTopology.hpp"

namespace spd {
namespace param {

using namespace std;

/*
 * コンストラクタ
 */
GenerateTopology::GenerateTopology() {

	// ムーア
	shared_ptr<spd::topology::Topology> moore = make_shared<spd::topology::Moore>();
	auto mooreName = moore->toString();
	std::transform(mooreName.begin(), mooreName.end(), mooreName.begin(), ::tolower);

	// ノイマン
	shared_ptr<spd::topology::Topology> neumann = make_shared<spd::topology::Neumann>();
	auto neumannName = neumann->toString();
	std::transform(neumannName.begin(), neumannName.end(), neumannName.begin(), ::tolower);

	// 六角
	shared_ptr<spd::topology::Topology> hex = make_shared<spd::topology::Hexagon>();
	auto hexName = hex->toString();
	std::transform(hexName.begin(), hexName.end(), hexName.begin(), ::tolower);

	// ランダムネットワーク
	shared_ptr<spd::topology::Topology> random = make_shared<spd::topology::Random>();
	auto randomName = "random";

	// レギュラー
	shared_ptr<spd::topology::Topology> regular = make_shared<spd::topology::Regular>();
	auto regularName = "regular";

	// 立方体
	shared_ptr<spd::topology::Topology> cube = make_shared<spd::topology::Cube>();
	auto cubeName = "cube";



	map<string, shared_ptr<spd::topology::Topology>> m {
		{mooreName, 	moore},
		{neumannName, neumann},
		{hexName, 		hex},
		{randomName, 	random},
		{regularName, regular},
		{cubeName, 	cube}
	};

	this->topologyMap = m;

}

/*
 * 文字列から空間構造を作成する
 */
shared_ptr<spd::topology::Topology> GenerateTopology::generate(const string& topology) const {

	if (topology.empty()) {
		throw std::invalid_argument("Topology strings could not to be empty string.");
	}

	std::vector<std::string> topoStrings;
	// コロンで分割
	boost::algorithm::split(topoStrings, topology, boost::is_any_of(":"));

	// 構造名
	std::string topologyName = topoStrings.at(0);
	std::transform(topologyName.begin(), topologyName.end(), topologyName.begin(), ::tolower);
	auto itr = this->topologyMap.find(topologyName);
	if (itr != this->topologyMap.end()) {

		auto& result = itr->second;

		// プロパティを取得し、設定
		std::vector<std::string> properties(topoStrings.size() - 1);
		for (int i = 1, max = topoStrings.size(); i < max; ++i) {
			properties[i - 1] = topoStrings[i];
		}
		result->setProp(properties);

		return result;
	}



	std::string err = "";
	for (auto m : this->topologyMap) {
		err += " " + m.first;
	}
	throw std::invalid_argument("Could not generate topology for " + topology + ".\n"
			"Settable rule(s): [" + err + " ]");
}

/*
 * 構造名と構造のマップを取得
 * @return 構造名と構造のマップ
 */
const map<string, shared_ptr<spd::topology::Topology>>& GenerateTopology::getTopologyMap() const {
	return this->topologyMap;
}

} /* namespace param */
} /* namespace spd */
