class Jsoncpp < Formula
  desc "Library for interacting with JSON"
  homepage "https://github.com/open-source-parsers/jsoncpp"
  url "https://github.com/open-source-parsers/jsoncpp/archive/refs/tags/1.9.6.tar.gz"
  sha256 "f93b6dd7ce796b13d02c108bc9f79812245a82e577581c4c9aabe57075c90ea2"
  license "MIT"
  head "https://github.com/open-source-parsers/jsoncpp.git", branch: "master"

  livecheck do
    url :stable
    strategy :github_latest
  end

  # NOTE: Do not change this to use CMake, because the CMake build is deprecated.
  # See: https://github.com/open-source-parsers/jsoncpp/wiki/Building#building-and-testing-with-cmake
  #      https://github.com/Homebrew/homebrew-core/pull/103386
  depends_on "meson" => :build
  depends_on "ninja" => :build
  depends_on "cmake" => :test

  # remove check_required_components for meson build
  # upstream pr ref, https://github.com/open-source-parsers/jsoncpp/pull/1570
  patch do
    url "https://github.com/open-source-parsers/jsoncpp/commit/3d47db0edcfa5cb5a6237c43efbe443221a32702.patch?full_index=1"
    sha256 "1d042632c3272e6946ac9ac1a7cb3b1f0b2a61f901bd20001bed53fc6892d0e0"
  end

  def install
    system "meson", "setup", "build", *std_meson_args
    system "meson", "compile", "-C", "build", "--verbose"
    system "meson", "install", "-C", "build"
  end

  test do
    (testpath/"CMakeLists.txt").write <<~EOS
      cmake_minimum_required(VERSION 3.10)
      project(TestJsonCpp)

      set(CMAKE_CXX_STANDARD 11)
      find_package(jsoncpp REQUIRED)

      add_executable(test test.cpp)
      target_link_libraries(test jsoncpp_lib)
    EOS

    (testpath/"test.cpp").write <<~EOS
      #include <json/json.h>
      int main() {
          Json::Value root;
          Json::CharReaderBuilder builder;
          std::string errs;
          std::istringstream stream1;
          stream1.str("[1, 2, 3]");
          return Json::parseFromStream(builder, stream1, &root, &errs) ? 0: 1;
      }
    EOS

    system "cmake", "-S", ".", "-B", "build"
    system "cmake", "--build", "build"
    system "./build/test"
  end
end
