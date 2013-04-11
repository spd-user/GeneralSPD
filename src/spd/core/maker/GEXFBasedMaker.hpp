/**
 * GEXFBasedMaker.hpp
 *
 * @date 2013/01/28
 * @author katsumata
 */

#ifndef GEXFBASEDMAKER_H_
#define GEXFBASEDMAKER_H_

#include <string>
#include <boost/property_tree/xml_parser.hpp>
#include "PlayerMaker.hpp"

namespace spd {
namespace param {
class Parameter;
}
namespace core {
class Space;

/**
 * GEXF v1.2 からプレイヤを作成するクラス
 */
class GEXFBasedMaker: public spd::core::PlayerMaker {

public :

	/**
	 * GEXFファイルを指定して初期化
	 * @param[in, out] param パラメタ
	 * @param[in] filename GEXFファイル
	 */
	GEXFBasedMaker(spd::param::Parameter& param, std::string filename);

	/**
	 * プレイヤを作成し、接続と近傍設定を行う
	 * @return 全てのプレイヤ
	 */
	AllPlayer makePlayers();

	/**
	 * プレイヤの初期化を行う
	 * @param[in] allPlayer すべてのプレイヤ
	 * @param[in, out] space 盤面
	 * @throw std::runtime_error 戦略の設定が出来ない場合
	 */
	void initPlayer(const AllPlayer& allPlayer, spd::core::Space& space);

	/**
	 * 構造と近傍の再初期化を行う
	 * @param[in, out] allPlayer すべてのプレイヤ
	 */
	void resetTopology(AllPlayer& allPlayer);

private:

	// パラメタ
	spd::param::Parameter& param;

	// ファイル名
	std::string gexfFile;

	// コネクション設定前の使用可能メモリ
	long int memoryForConnection;

	std::string::size_type getKeywordPos(const boost::property_tree::ptree::value_type& meta, std::string propType);

	// load connections and set neighbors
	void loadConnectionFromGEXF(AllPlayer& allPlayer);

	/**
	 * プレイヤに、ノードの要素がattrId の値を設定する
	 */
	void setPlayerVal(
			const std::shared_ptr<Player>& player,
			int attrId,
			const std::vector<std::pair<std::string, std::string>>& prop,
			std::string value);

	/**
	 * 同じ戦略のものを戦略リストからとってくる
	 */
	int getStrategyNum(std::string strategy);


};

} /* namespace core */
} /* namespace spd */
#endif /* GEXFBASEDMAKER_H_ */
