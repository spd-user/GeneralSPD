/**
 * GEXFOutput.cpp
 *
 * @date 2013/01/24
 * @author katsumata
 */

#include "GEXFOutput.hpp"

#include <iomanip>
#include <memory>
#include <cmath>
#include <utility>
#include <stdexcept>

#include <boost/any.hpp>

#include "../core/Space.hpp"
#include "../core/Player.hpp"
#include "../core/Strategy.hpp"
#include "../core/Converter.hpp"

#include "../param/Parameter.hpp"
#include "../param/NeighborhoodParameter.hpp"
#include "../param/RandomParameter.hpp"
#include "../param/RuntimeParameter.hpp"
#include "../param/OutputParameter.hpp"

#include "../topology/Topology.hpp"

#include "FileSystemOperation.hpp"

namespace spd {
namespace output {

/*
 * 空間の情報を出力する
 * @param[in, out] space 空間
 */
std::pair<std::string, bool> GEXFOutput::output(spd::core::Space& space) {

	auto& players = space.getPlayers();

	auto& param = space.getParameter();
	auto& runtimeParam = param.getRuntimeParameter();

	//(ディレクトリ)/gexf/spd_gexf_(3桁のsim)_(5桁のステップ数).gexf
	std::ostringstream simCount, step;
	setZeroPadding(simCount, 3, space.getSimCount());

	setZeroPadding(step, 5, space.getStep());

	std::string filename (param.getOutputParameter()->getDirectory() +
			PREFIX + simCount.str() + "_" + step.str() + SUFFIX);

	std::ofstream outputfile (filename);

	// メタ情報の表示
	outputfile << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
			"<gexf xmlns=\"http://www.gexf.net/1.2draft\" version=\"1.2\">\n"
			"\t<meta>\n\t\t<keywords>topology=" <<
			param.getNeighborhoodParameter()->getTopology()->toString(); // 空間構造

	for (auto strategy : param.getStrategyList()) {
		outputfile << "</keywords>\n\t\t<keywords>strategy=" << // 戦略
				strategy.first->getShortStrategy();
	}
	outputfile << "</keywords>\n\t\t<keywords>payoff-R=" << runtimeParam->getPayoffR() << // payoff
			"</keywords>\n\t\t<keywords>payoff-S=" << runtimeParam->getPayoffS() <<
			"</keywords>\n\t\t<keywords>payoff-T=" << runtimeParam->getPayoffT() <<
			"</keywords>\n\t\t<keywords>payoff-P=" << runtimeParam->getPayoffP() <<
			"</keywords>\n\t\t<keywords>strategy-update-cycle=" << runtimeParam->getStrategyUpdateCycle() << // 戦略周期
			"</keywords>\n\t\t<keywords>self-interaction=" << runtimeParam->isSelfInteraction() << // 自己対戦
			"</keywords>\n\t\t<keywords>step=" << space.getStep() << // ステップ
			"</keywords>\n\t\t<keywords>generated-rand=" <<
			param.getRandomParameter()->getGenerated() << "</keywords>\n\t</meta>\n"; // 乱数の生成回数

	// グラフの出力
	outputfile << "\t<graph mode=\"static\" defaultedgetype=\"undirected\">\n" <<
			"\t\t<attributes class=\"node\" mode=\"static\">\n";
	// ノードの属性出力
	writeNodeAttr(outputfile, 0, "pre-action", "string");
	writeNodeAttr(outputfile, 1, "action", "string");
	writeNodeAttr(outputfile, 2, "pre-score", "double");
	writeNodeAttr(outputfile, 3, "score", "double");
	writeNodeAttr(outputfile, 4, "pre-strategy", "string");
	writeNodeAttr(outputfile, 5, "strategy", "string");

	int attrId = 6;
	auto& properties = players.front()->getProperties();
	for (auto prop : properties) {
		writeNodeAttr(outputfile, attrId, prop.getName(), prop.valueType());
		++attrId;
	}
	outputfile << "\t\t</attributes>\n\t\t<nodes>\n";

	// ノードの出力
	// id は綺麗にしたい
	std::ostringstream id;
	int playerIdWidth = std::ceil(std::log10(players.size()));

	for (auto player : players) {

		setZeroPadding(id, playerIdWidth, player->getId());
		std::string playerId (id.str());

		outputfile << "\t\t\t<node id=\"" << playerId <<"\">\n\t\t\t\t<attvalues>\n";

		std::string preAction = "";
		preAction += spd::core::converter::actionToChar(player->getPreAction());

		std::string action = "";
		action += spd::core::converter::actionToChar(player->getAction());

		writeNodeValue(outputfile, 0, preAction);
		writeNodeValue(outputfile, 1, action);

		writeNodeValue(outputfile, 2, std::to_string(player->getPreScore()));
		writeNodeValue(outputfile, 3, std::to_string(player->getScore()));

		writeNodeValue(outputfile, 4, player->getPreStrategy()->getShortStrategy());
		writeNodeValue(outputfile, 5, player->getStrategy()->getShortStrategy());


		attrId = 6;

		auto& properties = player->getProperties();
		for (auto prop : properties) {
			std::string value (prop.valueToString());
			// ここで outputType enum の数が2桁になると、処理をすこし丁寧にする必要あり
			value += std::to_string(static_cast<int>(prop.getType()));
			writeNodeValue(outputfile, attrId, value);
			++attrId;
		}

		outputfile << "\t\t\t\t</attvalues>\n\t\t\t</node>\n";
	}

	outputfile << "\t\t</nodes>\n\t\t<edges>\n";

	// エッジの出力
	int edgeId = 0;
	for (auto player : players) {

		auto playerIdNum = player->getId();
		setZeroPadding(id, playerIdWidth, playerIdNum);
		std::string playerId = id.str();

		auto& opponents = player->getLinkedPlayers();
		for (int i = 0, size = opponents->size(); i < size; ++i) {

			// 相手のidが下位のだったらパスする
			auto opponentIdNum = opponents->at(i).lock()->getId();
			if (playerIdNum > opponentIdNum) {
				continue;
			}

			setZeroPadding(id, playerIdWidth, opponentIdNum);
			std::string opponentId = id.str();

			writeEdge(outputfile, edgeId, playerId, opponentId);
			++edgeId;
		}
	}
	outputfile << "\t\t</edges>\n\t</graph>\n</gexf>" << std::endl;


	return std::pair<std::string, bool> {filename, true};
}

/*
 * configファイルの作成
 */
void GEXFOutput::init(spd::core::Space& space, spd::param::Parameter& param) {

	std::string filename (param.getOutputParameter()->getDirectory() + PREFIX + "config.conf");

	// ディレクトリの作成
	FileSystemOperation fso;
	if (!fso.createDirectory(filename) ) {
		throw std::runtime_error("Could not create a directory for Gexf Output.");
	}

	std::ofstream confFile (filename);
	if (confFile.fail()) {
		throw std::runtime_error("Could not file open for gexf config file.");
	}

	param.showParameter(confFile);
}

inline void GEXFOutput::writeNodeAttr(std::ofstream& outputfile, int id, std::string name, std::string type) {
	outputfile << "\t\t\t<attribute id=\""<< id << "\" title=\"" << name <<"\" type=\"" << type << "\"/>\n";
}

inline void GEXFOutput::writeNodeValue(std::ofstream& outputfile, int attrId, std::string value) {

	outputfile << "\t\t\t\t\t<attvalue for=\"" << attrId << "\" value=\"" << value << "\"/>\n";
}

inline void GEXFOutput::writeEdge(
		std::ofstream& outputfile, int edgeId, std::string source, std::string target) {

	outputfile << "\t\t\t<edge id=\"" << edgeId << "\" source=\"" << source << "\" target=\"" << target << "\" />\n";
}

inline void GEXFOutput::setZeroPadding(std::ostringstream& oss, int width, int val) {
	oss.str("");
	oss.clear(std::stringstream::goodbit);
	oss.setf(std::ios::right);
	oss.fill('0');
	oss.width(width);
	oss << val;
}

} /* namespace output */
} /* namespace spd */
