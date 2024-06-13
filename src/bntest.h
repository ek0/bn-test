#ifndef BNTEST_BNTEST_H
#define BNTEST_BNTEST_H

namespace bntest {

// Initialize the test suite. Should be called during your plugin
// initialization.
void InitTests();

// Run all registered tests.
// Returns the result of RUN_ALL_TESTS macro.
int RunTests();

}  // namespace bntest

#endif