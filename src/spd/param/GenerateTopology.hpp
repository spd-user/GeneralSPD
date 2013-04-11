/**
 * GenerateTopology.hpp
 *
 * @date 2012/12/31
 * @author katsumata
 */

#ifndef GENERATETOPOLOGY_H_
#define GENERATETOPOLOGY_H_

#include <map>
#include <string>
#include <memory>

namespace spd {

namespace topology {
	class Topology;
}
namespace param {

/**
 * 空間構造の生成を行うクラス
 */
class GenerateTopology {
public:

	/**
	 * コンストラクタ
	 */
	GenerateTopology();

	/**
	 * 文字列から空間構造を作成する
	 * @param[in] topology 空間構造名
	 * @return 空間構造
	 * @throw invalid_argument 文字列が空の場合または、空間構造名に対応する構造を作成できない場合
	 */
	std::shared_ptr<spd::topology::Topology> generate(const std::string& topology) const;

	/**
	 * 構造名と構造の対応を取得
	 * @return 構造名と構造の対応
	 */
	const std::map<std::string, std::shared_ptr<spd::topology::Topology>>& getTopologyMap() const;

private:

	/**
	 * 空間構造名と空間構造を結びつけているmap
	 */
	std::map<std::string, std::shared_ptr<spd::topology::Topology>> topologyMap;
};

} /* namespace param */
} /* namespace spd */
#endif /* GENERATETOPOLOGY_H_ */
