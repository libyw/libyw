# libyw

> **A next-generation C23 standard utility library, built from scratch without standard `libc` dependencies.**

`libyw` is a modern, freestanding-first C23 standard utility library designed to replace legacy 1970s `libc` abstractions. It offers type-safe, explicit, and high-performance primitives for modern systems programming, embedded devices, and bare-metal environments.

---

## ✨ Key Features & Design Philosophy

* **Zero Standard `libc` Dependency**
  * Completely removes dependencies on legacy headers like `<stdlib.h>`, `<stdio.h>`, or `<unistd.h>`.
  * Memory allocation and system interfaces directly invoke Linux kernel system calls (via inline assembly `mmap` / `munmap`).
* **Native C23 & Type Safety**
  * Built around modern C23 features including `nullptr`, `[[nodiscard]]` attributes, and `_Generic` macros.
  * Provides generic, type-safe API overloads (`yw_sin`, `yw_sqrt`, `yw_sv`, etc.) without sacrificing performance.
* **Explicit Allocator Control**
  * Eliminates hidden global dynamic memory allocation.
  * Features a explicit `yw_allocator` interface supporting custom memory arenas, pool allocators, and freestanding configurations.
* **Modern Error Handling**
  * Abandons implicit global state like `errno`.
  * Uses explicit return statuses (`yw_result`) and tagged unions (`yw_option_ptr`) enforced with `[[nodiscard]]`.
* **SIMD & Modern Math Engine**
  * Includes a standalone, freestanding-compatible math library with built-in SIMD (`gnu::vector_size`) support.

---

## 📁 Project Structure

```text
.
├── include/
│   └── libyw/          # Public C23 Headers
│       ├── core/       # Fundamental types (types.h), Allocator interface (allocator.h)
│       ├── container/  # Dynamic containers (vec.h, etc.)
│       ├── math.h      # Generic, standalone math library
│       └── string.h    # Safe string_view & memory operations
├── src/                # Implementation Files
│   ├── core/           # Core allocators & infrastructure
│   ├── container/      # Container implementations
│   ├── math/           # Trigonometric & SIMD math routines
│   ├── string/         # String view and safe memory functions
│   └── sys/            # Direct OS system call interface (sys_linux.h)
└── tests/              # Integrated CTest Unit Tests
```

## 🛠️ Building & Testing

### Prerequisites

- **C Compiler**: GCC 13+ or Clang 16+ (Full C23 support required)

- **CMake**: Version 3.25 or higher

- **Build Tool**: Ninja or Make

### Quick Start

```bash
# 1. Clone the repository
git clone [https://github.com/KoHaRxnP/libyw.git](https://github.com/KoHaRxnP/libyw.git)
cd libyw

# 2. Configure build files
cmake -B build -G Ninja

# 3. Build the static library & tests
cmake --build build

# 4. Run unit tests via CTest
ctest --test-dir build --output-on-failure
```

## 💻 Usage Examples

### 1. Type-Safe Math with C23 `_Generic`

```c
#include "libyw/math.h"

void example_math(void) {
    // Automatically selects float or double implementation based on argument type
    float  f_res = yw_sin(1.57079f); 
    double d_res = yw_sin(3.1415926535);

    // Compound result structs eliminate output pointers and side effects
    yw_sincosf_result sc = yw_sincos(0.0f);
    // Access sc.sin and sc.cos safely
}
```

### 2. Explicit Allocator & `yw_vec` (Dynamic Array)

```c
#include "libyw/container/vec.h"

void example_vec(void) {
    yw_vec v;
    // Explicitly pass system-call-backed allocator
    yw_vec_init(&v, sizeof(int), yw_allocator_system());

    int value = 42;
    yw_vec_push_back(&v, &value);

    yw_vec_free(&v);
}
```

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.