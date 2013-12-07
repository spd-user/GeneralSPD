/**
 * Regular.hpp
 *
 * @date 2013/10/29
 * @author katsumata
 *
 */
#ifndef REGULAR_HPP_
#define REGULAR_HPP_

#include <memory>
#include <string>

#include "Network.hpp"

namespace spd {
namespace topology {

/**
 * 次数2以上のレギュラーグラフを表すクラス
 */
class Regular: public spd::topology::Network {
public:

	/**
	 * コンストラクタ
	 */
	Regular();

	/**
	 * すべてのプレイヤの接続を作成する。
	 *
	 * メモリ容量が足りない場合、プログラムを終了する。
	 * @param[in] players すべてのプレイヤ
	 * @param[in] param パラメタ
	 */
	void connectPlayers(const spd::core::AllPlayer& players,
			const spd::param::Parameter& param);

	/**
	 * トポロジのプロパティ(接続次数)を設定する。
	 *
	 * もし複数のプロパティが渡されても、一つ目しか使用しない<br>
	 * また、無い場合や 2 未満が与えられた場合は変更しない。
	 * @param[in] properties プロパティ
	 */
	void setProp(std::vector<std::string> properties);

	/**
	 * 空間構図構造名の出力
	 * @return 空間構図構造名(Regular)
	 */
	std::string toString() const {
		std::string result;

		result = "Regular:" + std::to_string(this->degree);
		return result;
	}

private:

	/**
	 * エッジを表すデータクラス
	 */
	class Edge {
	public:
		Edge(int node1, int node2) : n1(node1), n2(node2) {}

		bool operator==(Edge e) {
			return ((e.getNode1() == n1) && (e.getNode2() == n2))
					|| ((e.getNode1() == n2) && (e.getNode2() == n1));
		}
		bool operator!=(Edge e) {
			return !(((e.getNode1() == n1) && (e.getNode2() == n2))
					|| ((e.getNode1() == n2) && (e.getNode2() == n1)));
		}
		int getNode1() const {
			return n1;
		}
		int getNode2() const {
			return n2;
		}
	private:
		int n1;
		int n2;

	};

	/**
	 * デフォルトの接続次数
	 */
	static const int DEFAULT_DEGREE = 2;

	/**
	 * 次数
	 */
	int degree;

	/**
	 * 接続に使用したメモリ
	 */
	unsigned long int usedMemory;


	/**
	 * クリーク挿入法
	 */
	void cliqueInsertion(const spd::core::AllPlayer& players,
			const spd::param::Parameter& param, int k,
			std::vector<std::vector<int>>& nodes,
			std::vector<Edge>& edges);

	/**
	 * クリークの作成し、そのエッジリストを返す
	 */
	std::vector<Edge> createClique(std::vector<int>& nodes,
			const spd::core::AllPlayer& players);

	/**
	 * node の属するクリークIDを取得する
	 */
	int getCliqueID(std::vector<std::vector<int>>& allNodes,
			int node) const;


	/**
	 * クリーク番号の再管理を行う
	 * 渡されたエッジのうち、一つ目のエッジを含むクリークにまとめる
	 */
	void remanageCliqueID(std::vector<std::vector<int>>& allNodes,
			const std::vector<int>& nodes) const;


	/**
	 * 頂点挿入法
	 */
	void vertexInsertion(const spd::core::AllPlayer& players,
			const spd::param::Parameter& param,
			std::vector<std::vector<int>>& nodes,
			std::vector<Edge>& edges);

	/**
	 * 選択したエッジの上に、新しい頂点プレイヤを追加
	 */
	void addVertexOn(Edge edge, int vertex,
			std::vector<Edge>& edges,
			const spd::core::AllPlayer& players);

	/**
	 * n本の隣接しないエッジを選ぶ
	 */
	std::vector<Edge> selectDisjointEdge(int n,
			std::vector<Edge> allEdges,
			const spd::param::Parameter& param);

	/**
	 * エッジリストに隣接するエッジか調べる
	 * @retval true 隣接する場合
	 */
	bool adjustEdge(std::vector<Edge>& edgeList, Edge& edge);

	/**
	 * startNodeから始める長さpathのエッジを選ぶ
	 */
	std::vector<Edge> selectPath(const spd::core::AllPlayer& players,
			const spd::param::Parameter& param,
			std::vector<Edge>& allEdges);

	/**
	 * 一つのグラフにする
	 */
	void composeToOne(const spd::core::AllPlayer& players,
			std::vector<std::vector<int>>& nodes);

	/**
	 * 一つのグラフかどうか
	 */
	bool isOneGraph(std::vector<std::vector<int>>& nodes, int playerNum);


};

} /* namespace topology */
} /* namespace spd */

#endif /* REGULAR_HPP_ */
