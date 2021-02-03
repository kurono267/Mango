//
// Created by kurono267 on 03.02.2021.
//

#include <gtest/gtest.h>
#include <api/ShaderCode.hpp>
#include <api/Utils.hpp>

using namespace mango;

class ShaderCodeTest : public ::testing::Test {
	public:
		ShaderCodeTest() { /* init protected members here */ }
		~ShaderCodeTest() { /* free protected members here */ }
		void SetUp() override { /* called before every test */ }
		void TearDown() override { /* called after every test */ }
	protected:
};

TEST_F(ShaderCodeTest, GetMutable) {
	ShaderCode shaderCode("tests/ShaderCodeTest/input.glsl",ShaderStage::Fragment);
	std::cout << "Mutable code begin:" << std::endl;
	std::cout << shaderCode.getMutable() << std::endl;
	std::cout << "Mutable code end" << std::endl;
	std::string mutableReference = readFile("tests/ShaderCodeTest/mutable.glsl");
	mutableReference = mutableReference.substr(0,mutableReference.size()-1);

	ASSERT_EQ(mutableReference,shaderCode.getMutable());
}

TEST_F(ShaderCodeTest, ModifiedMutable) {
	ShaderCode shaderCode("tests/ShaderCodeTest/input.glsl",ShaderStage::Fragment);
	shaderCode.setMutable("    vec4 tex = vec4(1.f,0.f,0.f,1.f);\n"
						  "    float a = 1.0f;\n"
		"    outColor = vec4(color.rgb*tex.rgb,a);\n");

	std::cout << "Modified code " << std::endl;
	std::cout << shaderCode.getCode();

	std::string modifiedReference = readFile("tests/ShaderCodeTest/modified.glsl");
	ASSERT_EQ(modifiedReference,shaderCode.getCode());
}

int main(int argc, char** argv) {
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}