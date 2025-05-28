#pragma once

// Macro untuk ekspor fungsi saat jadi DLL (biar bisa dipakai pengguna)
#ifdef _WIN32
#  ifdef ZKETCH_EXPORTS
#    define ZKETCH_API __declspec(dllexport)
#  else
#    define ZKETCH_API __declspec(dllimport)
#  endif
#else
#  define ZKETCH_API
#endif

namespace zketch {
    ZKETCH_API void init();      // Inisialisasi manual (optional)
    ZKETCH_API void shutdown();  // Bersihkan kalau perlu
}
