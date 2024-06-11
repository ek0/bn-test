#include <binaryninjaapi.h>
#include <binaryninjacore.h>
#include <bntest.h>
#include <gtest/gtest.h>

namespace details {

TEST(BinaryNinjaTest, BasicSuccessTest) { EXPECT_EQ(2 + 2, 4); }

TEST(BinaryNinjaTest, BasicFailedTest) { EXPECT_TRUE(false); }

TEST(OtherTestSuite, TimerTest) {
  // Wait a few millisec to see proper values on the elapsed time info.
  Sleep(1234);
  EXPECT_TRUE(true);
}

}  // namespace details

extern "C" {

namespace bn = BinaryNinja;

void RunTests(bn::BinaryView* view) { bntest::RunTests(); }

BINARYNINJAPLUGIN uint32_t CorePluginABIVersion() {
  return BN_CURRENT_CORE_ABI_VERSION;
}

BINARYNINJAPLUGIN void CorePluginDependencies() {}

BINARYNINJAPLUGIN bool CorePluginInit() {
  bntest::InitTests();
  bn::PluginCommand::Register("[bn-test] Run Demo", "Run bntest demo suite.",
                              RunTests);
  return true;
}

}