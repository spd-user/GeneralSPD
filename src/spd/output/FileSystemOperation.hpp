/**
 * FileSystemOperation.hpp
 *
 * @date 2013/02/09
 * @author katsumata
 */

#ifndef FILESYSTEMOPERATION_HPP_
#define FILESYSTEMOPERATION_HPP_

#include <string>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string/predicate.hpp> // iends_with

namespace spd {
namespace output {

/**
 * ファイルシステムの操作を持つクラス
 */
class FileSystemOperation {

public:

	/**
	 * ファイル名からそのファイルのあるディレクトリを作成する
	 *
	 * @par
	 * ファイル名 /hoge/foo/bar/piyo の場合 /hoge/foo/bar ディレクトリを作成
	 * @param[in] fileName ファイル名
	 * @return ディレクトリがあるかどうか
	 * @retval true ディレクトリが存在または，作成に成功した場合
	 * @retval false 同名のファイルが存在または、作成に失敗した場合
	 */
	bool createDirectory(const std::string& fileName) {
		namespace fs = boost::filesystem;

		const fs::path file (fileName);
		const fs::path dir (file.parent_path());

		if (fs::is_directory(dir)) {
			// 既にあるディレクトリである
			return true;
		} else if (fs::is_regular_file(dir)) {
			// ファイルならfalse
			return false;
		}

		// 作成する
		return (fs::create_directories(dir));
	}

	/**
	 * ファイルまたは、ディレクトリの削除を行う
	 * @param[in] fileName 削除するファイルまたはディレクトリ
	 * @return 削除に成功したかどうか
	 * @retval true 削除に成功または、元からファイルがない場合
	 * @retval false 削除に失敗
	 */
	bool removeFile(const std::string& fileName) {
		namespace fs = boost::filesystem;
		const fs::path file (fileName);

		if (!fs::exists(file)) {
			return true;
		}
		boost::system::error_code error;
		const bool result = fs::remove(file, error);

		return (result && !error);
	}

	/**
	 * ファイルが指定した拡張子のファイルかどうか判定
	 * @param[in] fileName ファイル名
	 * @param[in] type 拡張子
	 * @return 指定した拡張子のファイルかどうか
	 * @retval true 指定した拡張子のファイルの場合
	 * @retval false 指定した拡張子のファイルでない場合
	 * @throw boost::filesystem::filesystem_error ファイルがないまたは、ディレクトリの場合
	 */
	bool isFileType(const std::string& fileName, const std::string& type) {
		namespace fs = boost::filesystem;
		const fs::path file (fileName);
		if (!fs::is_regular_file(file)) {
			std::string err (fileName + " is not a regular file.");
			boost::system::error_code ec;
			throw boost::filesystem::filesystem_error(err, ec);
		}

		return boost::algorithm::iends_with(fileName, type);
	}
};

} /* namespace output */
} /* namespace spd */
#endif /* FILESYSTEMOPERATION_HPP_ */
