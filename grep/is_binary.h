#pragma once

#include <cstring>

bool is_elf_header(const char* buffer);

bool is_archive_header(const char* buffer);

bool is_jpeg(const char* buffer);

bool is_png(const char* buffer);

bool is_zip(const char* buffer);

bool is_gzip(const char* buffer);

bool is_tar(const char* buffer, const std::size_t& bytes_read);

bool is_pdf(const char* buffer);

bool is_binary(const char* buffer);