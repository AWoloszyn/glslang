// Copyright 2015 The Shaderc Authors. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

// Some of the tests here check code paths that are not checked by
// integration tests.
// Generally, these would be conditions not generated by the Glslang
// compiler.  It's easier to write these unit tests than to inject
// a dependency on a fake compiler.
#include "shader_stage.h"

#include <sstream>

#include <gtest/gtest.h>

using shaderc_util::string_piece;

namespace {

TEST(StageDeducer, ValidStages) {
  std::stringstream error_stream;
  EXPECT_EQ(EShLangVertex, glslc::StageDeducer("a.vert")(&error_stream, "tag"));
  EXPECT_EQ("", error_stream.str());

  EXPECT_EQ(EShLangFragment,
            glslc::StageDeducer("a.frag")(&error_stream, "tag"));
  EXPECT_EQ("", error_stream.str());

  EXPECT_EQ(EShLangGeometry,
            glslc::StageDeducer("a.geom")(&error_stream, "tag"));
  EXPECT_EQ("", error_stream.str());

  EXPECT_EQ(EShLangTessControl,
            glslc::StageDeducer("a.tesc")(&error_stream, "tag"));
  EXPECT_EQ("", error_stream.str());

  EXPECT_EQ(EShLangTessEvaluation,
            glslc::StageDeducer("a.tese")(&error_stream, "tag"));
  EXPECT_EQ("", error_stream.str());

  EXPECT_EQ(EShLangCompute,
            glslc::StageDeducer("a.comp")(&error_stream, "tag"));
  EXPECT_EQ("", error_stream.str());
}

TEST(StageDeducer, InvalidStages) {
  std::stringstream error_stream;

  EXPECT_EQ(EShLangCount, glslc::StageDeducer("a.glsl")(&error_stream, "tag"));
  EXPECT_EQ(
      "glslc: error: 'a.glsl': .glsl file encountered but no"
      " -fshader-stage specified ahead\n",
      error_stream.str());
  error_stream.str("");

  EXPECT_EQ(EShLangCount, glslc::StageDeducer("-")(&error_stream, "tag"));
  EXPECT_EQ(
      "glslc: error: '-': -fshader-stage required when input is from"
      " standard input \"-\"\n",
      error_stream.str());
  error_stream.str("");

  EXPECT_EQ(EShLangCount, glslc::StageDeducer("a.foo")(&error_stream, "tag"));
  EXPECT_EQ(
      "glslc: error: 'a.foo': file not recognized: File format not"
      " recognized\n",
      error_stream.str());
  error_stream.str("");

  EXPECT_EQ(EShLangCount,
            glslc::StageDeducer("no-file-extension")(&error_stream, "tag"));
  EXPECT_EQ(
      "glslc: error: 'no-file-extension': file not recognized: File format not"
      " recognized\n",
      error_stream.str());
  error_stream.str("");
}

}  // anonymous namespace