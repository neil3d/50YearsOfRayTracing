#include "MyAssetManager.h"

#include <algorithm>
#include <filesystem>
#include <iostream>

#include "ImageRGBA.h"
#include "WavefrontOBJ.h"

namespace RTKit2 {

std::string MyAssetManager::makeFullPath(const std::string& szPath) const {
  for (const auto& szBasePath : mSearchPath) {
    std::filesystem::path fullPath(szBasePath);
    fullPath.append(szPath);

    if (std::filesystem::exists(fullPath)) {
      return fullPath.string();
    }
  }  // end of for

  std::cerr << "File not found: " << std::filesystem::current_path() << ", "
            << szPath << std::endl;
  return std::string();
}

MyAssetObject::Ptr MyAssetManager::add(const std::string& szKey,
                                       const std::string& szPath) {
  std::filesystem::path path(szPath);
  auto ext = path.extension().string();
  std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

  if (ext == ".png") return load<ImageRGBA>(szKey, szPath);
  if (ext == ".obj") return load<WavefrontOBJ>(szKey, szPath);

  throw MyException(std::string("File extension not supported: ") + szPath);

  return MyAssetObject::Ptr();
}

}  // namespace RTKit2