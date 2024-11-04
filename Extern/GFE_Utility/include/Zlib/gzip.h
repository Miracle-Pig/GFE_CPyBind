#ifndef GZIP_H
#define GZIP_H

#include <string>

#ifdef _WIN32
namespace GFE::Zip {

	//! 压缩文件到输出路径 gzip压缩
	int CompressFile(const std::string& in_path, const std::string& out_path);
	//! 解压gzip文件到输出路径， 非gzip文件不变
	int DecompressFile(const std::string& in_path, const std::string& out_path);
	//int GZstream(std::ostream& is, std::ostream& os);


}
#endif

#endif // GZIP_H
