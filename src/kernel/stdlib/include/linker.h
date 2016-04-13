#pragma once
extern "C" uintptr_t kernel_lma;
extern "C" uintptr_t kernel_vma;
extern "C" uintptr_t k_start;
extern "C" uintptr_t k_end;
extern "C" uintptr_t k_data_end;
#define KERNEL_LMA  reinterpret_cast<uintptr_t>(&kernel_lma)
#define KERNEL_VMA  reinterpret_cast<uintptr_t>(&kernel_vma)
#define K_START     reinterpret_cast<uintptr_t>(&k_start)
#define K_END       reinterpret_cast<uintptr_t>(&k_end)
#define K_DATA_END  reinterpret_cast<uintptr_t>(&k_data_end)
