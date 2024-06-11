#include "bntest.h"

#include <binaryninjaapi.h>
#include <binaryninjacore.h>
#include <fmt/format.h>
#include <gtest/gtest.h>

namespace bntest {

namespace bn = BinaryNinja;

class BinaryNinjaTestListener : public ::testing::EmptyTestEventListener {
  void OnTestProgramStart(const ::testing::UnitTest& unit_test) override {
    number_of_tests_to_run = unit_test.test_to_run_count();
  }

  void OnTestStart(const testing::TestInfo& test_info) override {
    current_test_number++;
    bn::LogInfo("  Running %llu: %s.%s", current_test_number,
                test_info.test_suite_name(), test_info.name());
  }

  void OnTestEnd(const testing::TestInfo& test_info) override {
    if (test_info.result()->Passed())
      bn::LogInfo("%llu/%llu Test #%llu: %s.%s. Passed.", current_test_number,
                  number_of_tests_to_run, current_test_number,
                  test_info.test_suite_name(), test_info.name());
    else
      bn::LogInfo("%llu/%llu Test #%llu: %s.%s. Failed.", current_test_number,
                  number_of_tests_to_run, current_test_number,
                  test_info.test_suite_name(), test_info.name());
  }

  void OnTestPartResult(
      const ::testing::TestPartResult& test_part_result) override {
    if (test_part_result.failed()) {
      bn::LogError("%s", test_part_result.summary());
    } else {
      bn::LogInfo("%s", test_part_result.summary());
    }
  }

  void OnTestProgramEnd(const ::testing::UnitTest& unit_test) override {
    uint64_t success_ratio =
        static_cast<double>(unit_test.successful_test_count()) /
        static_cast<double>(unit_test.total_test_count()) * 100;
    bn::LogInfo("%llu%% tests passed, %d tests failed out of %d", success_ratio,
                unit_test.failed_test_count(), unit_test.total_test_count());
    bn::LogInfo("Total Test time (real) = %f sec",
                static_cast<double>(unit_test.elapsed_time()) / 1000);
  }

 private:
  uint64_t current_test_number = 0;

  uint64_t number_of_tests_to_run = 0;
};

void InitTests() {
  ::testing::InitGoogleTest();
  ::testing::UnitTest::GetInstance()->listeners().Append(
      new BinaryNinjaTestListener);
}

void RunTests() { RUN_ALL_TESTS(); }

}  // namespace bntest