#include <binaryninjaapi.h>
#include <binaryninjacore.h>
#include <bntest.h>
#include <gtest/gtest.h>
#include <mediumlevelilinstruction.h>

namespace details {

namespace bn = BinaryNinja;

class MediumLevelILTest : public ::testing::Test {
 public:
  void SetUp() override {
    arch_ = bn::Architecture::GetByName("x86_64");
    ASSERT_NE(arch_.GetPtr(), nullptr);

    // CoreRefCountObject require to be held in a bn::Ref (some kind of shared
    // pointer) or they will lead to undefined behavior when getting deleted.
    // You can't use a std::unique_ptr to hold this object either.
    mlil_ = new bn::MediumLevelILFunction{arch_};
    mlil_->AddInstruction(mlil_->Return({}));
    ASSERT_NE(mlil_.GetPtr(), nullptr);
  }

  void TearDown() override {}

  bn::Ref<bn::Architecture> arch_ = nullptr;

  bn::Ref<bn::MediumLevelILFunction> mlil_ = nullptr;
};

TEST_F(MediumLevelILTest, BasicTest) {
  auto count = mlil_->GetInstructionCount();
  EXPECT_EQ(count, 1);
  auto i = mlil_->GetInstruction(0);
  EXPECT_EQ(i.operation, MLIL_RET);
}

}  // namespace details