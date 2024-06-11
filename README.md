# Binary Ninja Test Framework

`bn-test` is a `googletest` harness to enable the usage of `googletest` in a Binary Ninja plugin. It implements a custom event listener to redirect `googletest` results to Bianry Ninja's log console.

## Usage

You need to initialize the test suite by calling `bntest::InitTests` during the plugin initialization. It is then recommended to run your test using a `PluginCommand`. The following code will initialize the test suite and add a contextual menu entry to run all tests.

```cpp
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
```

## Building

Using CMake from the command line:

```
$ mkdir build && cd build
$ cmake -G "Ninja" -DHEADLESS=1 ..
$ cmake --build .
```

For Visual Studio, you can emulate CMake behavior by adding the required options to your `CMakeSettings.json`.

## Integrating in a CMake project

If your goal is to use `bntest` in an existing project, you can just use the library through `FetchContent`:

```cmake
include(FetchContent)
FetchContent_Declare(bntest
	GIT_REPOSITORY https://github.com/ek0/bn-test.git
	GIT_TAG main
	SYSTEM
)
FetchContent_MakeAvailable(bntest)

# ...

target_link_libraries(myproject PRIVATE bntest)
```

## Considerations

- We currently can't control test behavior through standard `googletest` command line arguments.
- Most Binary Ninja objects inherit from `CoreRefCountObject`, which require to be held in a `Ref`. This will lead to conflicts on Windows if used within `googletest` macros when attempting to compare `Ref` values. To work around this, it is required to leverage the `Ref::GetPtr` function.
```cpp
    auto arch = bn::Architecture::GetByName("x86_64");
    // ASSERT_NE(arch, nullptr);       <- Compilation error
    ASSERT_NE(arch.GetPtr(), nullptr);
```
- While tests can be written without depending on a specific `BinaryView` or `Function`, you still need to have a binary view opened to run the plugin.