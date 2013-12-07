/**
 * Regular.cpp
 *
 * @date 2013/10/29
 * @author katsumata
 */
#include "Regular.hpp"

#include <stdexcept>
#include <iostream>
#include <random>
#include <algorithm>

#include "../../core/OriginalType.hpp"
#include "../../core/Player.hpp"
#include "../../core/Space.hpp"

#include "../../param/Parameter.hpp"
#include "../../param/InitParameter.hpp"
#include "../../param/NeighborhoodParameter.hpp"
#include "../../param/RandomParameter.hpp"

namespace spd {
namespace topology {

using spd::core::Player;


/*
 * コンストラクタ
 */
Regular::Regular() : degree(DEFAULT_DEGREE), usedMemory(0) {};



/*
 * すべてのプレイヤの接続を作成する。
 *
 * クリーク挿入法、頂点挿入法を用いる
 *
 * メモリ容量が足りない場合、プログラムを終了する。
 * @note 松田らの正則グラフを生成するアルゴリズムを参考に、2つを用いた。
 * @param[in] players すべてのプレイヤ
 * @param[in] param パラメタ
 */
void Regular::connectPlayers(const spd::core::AllPlayer& players,
		const spd::param::Parameter& param) {


	// プレイヤ数
	int allPlayerNum = players.size();

	// 指定した次数で作成できるかどうか
	// プレイヤ数と次数の積は偶数である必要がある
	if (((allPlayerNum * this->degree) % 2 == 1)
			|| (this->degree >= allPlayerNum)) {
		std::cerr << "degree: " << this->degree << std::endl;
		throw std::runtime_error("Could not create regular graph for this degree. (node * degree) must be even number.");
	}


	// 前回使ったメモリを戻しておく
	// 初回時は 0
	auto memory = param.getInitialParameter()->getMemory() + usedMemory;
	param.getInitialParameter()->setMemory(memory);

	auto iniParam = param.getInitialParameter();

	// 使用可能メモリ
	long long int availableMemory = iniParam->getMemory();

	// 生成するエッジ数
	unsigned long long generateEdge = this->degree * players.size() / 2;
	// メモリの確認(双方向分)
	unsigned long int requiredMemory = sizeof(std::weak_ptr<Player>) * generateEdge * 2;


	availableMemory -= requiredMemory;

	if (availableMemory < 0) {

		std::cerr << "Degree : " << degree << "\ngenerateEdge : " <<  generateEdge <<
				"\nrequired memory : " << requiredMemory << std::endl;

		std::cerr << "\nThis program could not construct a spatial structure due to insufficient memory.\n"
				<< "Please input a sufficient available memory size or run on other machines.\n\n"
				<< "To simulate this setting, add more than "
				<< std::abs(availableMemory) << " byte(s) of memory.\n";
		std::exit(EXIT_FAILURE);
	}
	iniParam->setMemory(availableMemory);
	// 使用メモリの記憶
	this->usedMemory = requiredMemory;


	auto randParam = param.getRandomParameter();
	auto& engine = randParam->getEngine();

	unsigned long long genRnd = 0;


	// 初期ネットワークとする、次数 d のクリークの数
	// 半分ぐらいは初期のクリークでできるようにする
	// また、kは 1 以上
	int k = allPlayerNum / (2 * (this->degree + 1));
	if (k == 0) {
		++k;
	}


	// ノードとクリークの管理
	// 対応するクリークごとにどのプレイヤが属するのかをみる
	// 0が未配置プレイヤであり、1 以降がクリークID
	std::vector<std::vector<int>> nodes;
	nodes.reserve(k + 1);
	for (int i = 0; i < k + 1; ++i) {
		nodes.push_back(std::vector<int>());
	}
	nodes.at(0).reserve(allPlayerNum);
	for (int i = 0; i < allPlayerNum; ++i) {
		nodes[0].push_back(i);
	}

	// 全ての枝
	std::vector<Edge> edges;
	edges.reserve(degree * allPlayerNum / 2);


	// 初期クリークの作成
	for (int i = 0; i < k; ++i) {
		// クリークに入れるもの
		std::vector<int> initClique;
		initClique.reserve(this->degree + 1);
		for (int n = 0; n < this->degree + 1; ++n) {
			std::uniform_int_distribution<> dist(0, nodes[0].size() - 1);
			int node = dist(engine);
			++genRnd;
			initClique.push_back(nodes[0].at(node));

			// ここで入れたものが、どのグラフかを管理する
			nodes.at(i + 1).push_back(nodes[0].at(node));

			// 未配置から削除
			nodes[0].erase(std::begin(nodes[0]) + node);
		}

		auto cliqueEdges = createClique(initClique, players);
		edges.insert(std::end(edges), std::begin(cliqueEdges), std::end(cliqueEdges));
	}


	// クリーク挿入
	cliqueInsertion(players, param, k, nodes, edges);


	// 頂点挿入
	vertexInsertion(players, param, nodes, edges);

	// 一つのグラフにする
	composeToOne(players, nodes);


	randParam->addGenerated(genRnd);
}


/*
 * 端同士つなげて一つのグラフにする
 */
void Regular::composeToOne(const spd::core::AllPlayer& players,
		std::vector<std::vector<int>>& nodes) {

	// 未配置があったらエラー
	if (nodes.at(0).size() != 0) {
		throw std::runtime_error("Unassigned players are exist.");
	}

	// vector<vector>の空でない番号を取得
	// 流れは、前からは0, 後ろからはそれ以外を指定する
	auto findUnemptyGroup = [](std::vector<std::vector<int>>& v, int flow){
		if (flow == 0) {
			for (int i = 0, vecMax = v.size(); i < vecMax; ++i) {
				if (! v[i].empty()) {
					return i;
				}
			}
		} else {
			for (int i = v.size() - 1; i >= 0; --i) {
				if (! v[i].empty()) {
					return i;
				}
			}
		}
		throw std::runtime_error("all vectors are empty");
	};

	while(! isOneGraph(nodes, players.size())) {
		int firstPos = findUnemptyGroup(nodes, 0);
		int lastPos = findUnemptyGroup(nodes, 1);

		if (firstPos == lastPos) {
			// どっちも同じはおかしい
			throw std::runtime_error("Graph is uncomplete graph.");
		}

		// グループ
		auto group1 = nodes[firstPos];
		auto group2 = nodes[lastPos];

		// ノード
		int g1e1 = group1[0];
		int g1e2 = players[group1[0]]->getLinkedPlayers()->front().lock()->getId();
		int g2e1 = group2[0];
		int g2e2 = players[group2[0]]->getLinkedPlayers()->front().lock()->getId();

		// 接続を削除
		players[g1e1]->deleteLinkTo(players[g1e2]);
		players[g1e2]->deleteLinkTo(players[g1e1]);
		players[g2e1]->deleteLinkTo(players[g2e2]);
		players[g2e2]->deleteLinkTo(players[g2e1]);

		// 交差接続
		players[g1e1]->linkTo(players[g2e1]);
		players[g2e1]->linkTo(players[g1e1]);
		players[g1e2]->linkTo(players[g2e2]);
		players[g2e2]->linkTo(players[g1e2]);

		nodes[firstPos].insert(std::end(nodes[firstPos]), std::begin(nodes[lastPos]), std::end(nodes[lastPos]));
		nodes[lastPos].clear();
	}
}




bool Regular::isOneGraph(std::vector<std::vector<int>>& nodes, int playerNum) {

	// 未配置があったらエラー
	if (nodes.at(0).size() != 0) {
		throw std::runtime_error("Unassigned players are exist.");
	}

	for (auto v : nodes) {
		if (!((v.size() == 0) || (v.size() == static_cast<unsigned int>(playerNum)))) {
			return false;
		}
	}
	return true;
}


/*
 * 頂点挿入法
 */
void Regular::vertexInsertion(const spd::core::AllPlayer& players,
		const spd::param::Parameter& param,
		std::vector<std::vector<int>>& allNodes,
		std::vector<Edge>& allEdges) {


	while (allNodes[0].size() != 0) {


		if (this->degree % 2 == 0) {

			// 未配置プレイヤを取得
			auto node = allNodes[0].back();
			allNodes[0].pop_back();

			// 次数が偶数
			auto disjointEdges = selectDisjointEdge(degree / 2, allEdges, param);

			// 同じグラフ上になることになる頂点
			std::vector<int> sameGraphVertex;

			for (auto delEdge : disjointEdges) {

				addVertexOn(delEdge, node, allEdges, players);

				sameGraphVertex.push_back(delEdge.getNode1());
			}
			remanageCliqueID(allNodes, sameGraphVertex);
			// 一つ目のノードがあるところへ挿入
			allNodes.at(getCliqueID(allNodes, disjointEdges[0].getNode1())).push_back(node);


		} else {
			// 次数が奇数

			// 長さ degree を構成するノード
			auto pathNode = selectPath(players, param, allEdges);

			for (int i = 0; i <= 1; ++i) {
				// 未配置プレイヤを取得
				auto node = allNodes[0].back();
				allNodes[0].pop_back();

				std::vector<Regular::Edge> path;
				path.reserve(degree - 1);
				for (int j = i; j < degree - 1 + i; ++j) {
					path.push_back(pathNode[j]);
				}
				for (auto edge : path) {
					addVertexOn(edge, node, allEdges, players);
				}
				allNodes.at(path[0].getNode1()).push_back(node);
			}
		}
	}
}

std::vector<Regular::Edge> Regular::selectPath(const spd::core::AllPlayer& players,
		const spd::param::Parameter& param,
		std::vector<Edge>& allEdges) {

	unsigned long long genRnd = 0;

	auto randParam = param.getRandomParameter();
	auto& engine = randParam->getEngine();
	// 次数までの分布
	std::uniform_int_distribution<> dist(0, this->degree - 1);

	std::uniform_int_distribution<> edgeDist(0, allEdges.size() - 1);
	// 適当にエッジを選んで、その一つからスタートする
	int startNode = allEdges[edgeDist(engine)].getNode1();
	++genRnd;


	std::vector<int> selectNodes;
	selectNodes.reserve(this->degree + 1);

	int node = startNode;
	selectNodes.push_back(node);

	while (selectNodes.size() != static_cast<unsigned int>(this->degree + 1)) {

		auto playerLink = players[node]->getLinkedPlayers();

		if (playerLink->size() != static_cast<unsigned int>(this->degree)) {
			throw std::runtime_error("Already not regular graph.");
		}

		int adNodeID = ((playerLink->at(dist(engine))).lock())->getId();
		++genRnd;

		// 今までにない場合に追加
		if (std::find(std::begin(selectNodes),
				std::end(selectNodes),
				adNodeID) == std::end(selectNodes)) {
			selectNodes.push_back(adNodeID);
			node = adNodeID;
		}
	}

	std::vector<Regular::Edge> result;
	result.reserve(this->degree);

	int firstNode = startNode;
	int secondNode = selectNodes[1];
	for (int i = 1; i < this->degree; ++i) {
		Regular::Edge edge(firstNode, secondNode);
		result.push_back(edge);

		firstNode = secondNode;
		secondNode = selectNodes.at(i + 1);
	}

	randParam->addGenerated(genRnd);

	return result;
}


/*
 * n本の隣接しないエッジを選ぶ
 */
std::vector<Regular::Edge> Regular::selectDisjointEdge(int n,
		std::vector<Regular::Edge> allEdges,
		const spd::param::Parameter& param) {

	auto randParam = param.getRandomParameter();
	auto& engine = randParam->getEngine();

	unsigned long long genRnd = 0;

	std::vector<Edge> result;
	result.reserve(n);

	while(result.size() < static_cast<unsigned int>(n)) {
		std::uniform_int_distribution<> dist(0, allEdges.size() - 1);

		int selectEdgeID = dist(engine);
		++genRnd;
		auto selectEdge = allEdges[selectEdgeID];

		// 隣接していない
		if(!adjustEdge(result, selectEdge)) {

			allEdges.erase(std::begin(allEdges) + selectEdgeID);
			result.push_back(selectEdge);
		}
	}

	randParam->addGenerated(genRnd);
	return result;
}

/*
 * 隣接したエッジを選んだかどうか
 */
bool Regular::adjustEdge(std::vector<Edge>& edgeList, Edge& edge) {

	// 調べる対象がない場合は、隣接にはならない
	if (edgeList.size() == 0) {
		return false;
	}

	auto n1 = edge.getNode1();
	auto n2 = edge.getNode2();

	for (auto e : edgeList) {

		if ((e.getNode1() == n1) || (e.getNode1() == n2)
			|| (e.getNode2() == n1) || (e.getNode2() == n2)) {

			return true;
			}
	}
	return false;
}


/*
 * クリーク挿入方法
 */
void Regular::cliqueInsertion(const spd::core::AllPlayer& players,
		const spd::param::Parameter& param, int k,
		std::vector<std::vector<int>>& allNodes,
		std::vector<Edge>& allEdges) {


	// プレイヤ数
	int allPlayerNum = players.size();

	auto randParam = param.getRandomParameter();
	auto& engine = randParam->getEngine();

	unsigned long long genRnd = 0;

	// クリーク挿入回数
	int l = (allPlayerNum - k * (degree + 1)) / (degree - 1);

	// クリーク挿入
	for (int i = 0; i < l; ++i) {

		// 挿入するクリーク
		std::vector<int> insCliqueNode;
		insCliqueNode.reserve(this->degree - 1);
		for (int n = 0; n < this->degree - 1; ++n) {
			std::uniform_int_distribution<> dist(0, allNodes[0].size() - 1);
			int node = dist(engine);
			++genRnd;
			insCliqueNode.push_back(allNodes[0].at(node));

			// 未配置から削除
			allNodes[0].erase(std::begin(allNodes[0]) + node);
		}
		auto insCliqueEdges = createClique(insCliqueNode, players);

		// 挿入する先のエッジを選択
		std::vector<Edge> deleteEdges;
		while (deleteEdges.size() < static_cast<unsigned int>(this->degree - 1)) {
			std::uniform_int_distribution<> edgeDist(0, allEdges.size() -1);
			int edgeID = edgeDist(engine);
			++genRnd;

			auto edge = allEdges.at(edgeID);
			if (std::find(std::begin(deleteEdges), std::end(deleteEdges), edge) == std::end(deleteEdges)) {
				deleteEdges.push_back(allEdges.at(edgeID));
			}
		}

		// クリークの挿入
		for (int v = 0; v < this->degree - 1; ++v) {
			addVertexOn(deleteEdges[v], insCliqueNode[v], allEdges, players);
		}

		// クリーク番号の整理
		std::vector<int> sameGraphVertex;
		for (auto edge : deleteEdges) {
			sameGraphVertex.push_back(edge.getNode1());
		}
		remanageCliqueID(allNodes, sameGraphVertex);

		// 出来たのを新しく管理変数へ代入
		auto& composedGraph = allNodes[getCliqueID(allNodes, deleteEdges[0].getNode1())];
		composedGraph.insert(std::begin(composedGraph), std::begin(insCliqueNode), std::end(insCliqueNode));

		allEdges.insert(std::begin(allEdges), std::begin(insCliqueEdges), std::end(insCliqueEdges));
	}

	randParam->addGenerated(genRnd);
}



/*
 * 選択したエッジの上に、新しいプレイヤを追加
 */
void Regular::addVertexOn(Edge edge, int vertex,
		std::vector<Edge>& allEdges,
		const spd::core::AllPlayer& players) {

	auto node1 = edge.getNode1();
	auto node2 = edge.getNode2();

	// エッジの削除
	// プレイヤ間
	auto delLink1 = players.at(node1)->deleteLinkTo(players.at(node2));
	auto delLink2 = players.at(node2)->deleteLinkTo(players.at(node1));
	if (!(delLink1 && delLink2)) {
		std::cerr << "node1: " << node1 << ", node2: " << node2 << std::endl;
		throw std::runtime_error("Missed a delete edge in Regular::addVertexOn");
	}
	// 管理リスト
	auto edgePosition = std::find(std::begin(allEdges), std::end(allEdges), edge);
	if (edgePosition != std::end(allEdges)) {
		allEdges.erase(edgePosition);
	} else {
		// ないのはおかしい
		throw std::runtime_error("Could not find a delete edge in all edges.");
	}


	// エッジの追加
	// プレイヤ間
	if (!(players.at(node1)->linkTo(players.at(vertex))
			&& players.at(vertex)->linkTo(players.at(node1))
			&& players.at(node2)->linkTo(players.at(vertex))
			&& players.at(vertex)->linkTo(players.at(node2)))) {
		std::cerr << "new vertex: " << vertex << std::endl;
		throw std::runtime_error("Missed a insert new vertex.");
	}
	// 管理リスト
	allEdges.push_back(Edge(node1, vertex));
	allEdges.push_back(Edge(node2, vertex));
}


/*
 * 指定したプレイヤ間でクリークを作る
 */
std::vector<Regular::Edge> Regular::createClique(std::vector<int>& nodes,
		const spd::core::AllPlayer& players) {

	std::vector<Regular::Edge> result;

	if (nodes.size() == 0) {
		throw std::runtime_error("Could not create NULL clique.");
	}

	for (int i = 0, iMax = nodes.size() - 1; i < iMax; ++i) {
		for (int j = i + 1, jMax = nodes.size(); j < jMax; ++j) {
			// どっちかのリンクに失敗
			auto link1 = players.at(nodes.at(i))->linkTo(players.at(nodes.at(j)));
			auto link2 = players.at(nodes.at(j))->linkTo(players.at(nodes.at(i)));
			if (!(link1 && link2)) {
				std::cerr << "src: " << i << ", dest: " << j << ", result: s->d("
						<< link1 << "), d->s(" << link2 << ")" << std::endl;
				throw std::runtime_error("Clique generate err.");
			}
			result.push_back(Edge(nodes[i], nodes[j]));
		}
	}

	if (result.size() != (nodes.size() * (nodes.size() - 1) / 2)) {
		throw std::runtime_error("Missed a create clique.");
	}

	return result;
}


/*
 * クリーク番号の再管理を行う
 */
void Regular::remanageCliqueID(std::vector<std::vector<int>>& allNodes,
		const std::vector<int>& nodes) const {

	// 渡されたノードを含むネットワークが同じ、クリークIDになるように再管理する
	// つまり、渡されたノード達は直接(または、間接)接続されている

	if (nodes.empty()) {
		return;
	}

	int firstNodeCliqueID = getCliqueID(allNodes, nodes[0]);

	// 必ず属しているはずなので 0 以下にはならない
	if (firstNodeCliqueID < 1) {
		throw std::runtime_error("first node clique ID err.");
	}


	// 1つめのエッジを持つクリークに含まれるようにする
	for (int node : nodes) {

		// エッジのノードは同じクリークIDを持っている
		int cliqueID = getCliqueID(allNodes, node);
		if (cliqueID < 1) {
			// クリークに属していない
			throw std::runtime_error("clique ID err.");
		}

		if ((cliqueID != firstNodeCliqueID) && (! allNodes.at(cliqueID).empty())) {

			allNodes.at(firstNodeCliqueID).insert(std::end(allNodes.at(firstNodeCliqueID)),
					std::begin(allNodes.at(cliqueID)),
					std::end(allNodes.at(cliqueID)));

			allNodes.at(cliqueID).clear();
		}
	}
}

/*
 * どのクリークに属するかしらべる
 */
int Regular::getCliqueID(std::vector<std::vector<int>>& allNodes,
		int node) const {

	for (int i = allNodes.size() - 1; i >= 0; --i) {
		// クリーク上にノードがあった場合
		if (std::find(
				std::begin(allNodes.at(i)),
				std::end(allNodes.at(i)),
				node
		) != std::end(allNodes.at(i))) {

			return i;
		}
	}

	return -1;
}



/*
 * トポロジのプロパティ(接続次数)を設定する
 *
 * もし複数のプロパティが渡されても、一つ目しか使用しない<br>
 * また、無い場合は変更しないまま
 * @param[in] properties プロパティ
 */
void Regular::setProp(std::vector<std::string> properties) {
	if (properties.size() < 1) {
		return;
	}

	int dg = std::stoi(properties[0]);
	if (dg < 2) {
		return;
	}

	this->degree = dg;
}

} /* namespace topology */
} /* namespace spd */
