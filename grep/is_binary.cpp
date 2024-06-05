#include "is_binary.h"
#include <string_view>

bool is_elf_header(const char *buffer) {
  static constexpr std::string_view elf_magic = "\x7f"
                                                "ELF";
  return (strncmp(buffer, elf_magic.data(), elf_magic.size()) == 0);
}

bool is_archive_header(const char *buffer) {
  static constexpr std::string_view archive_magic = "!<arch>";
  return (strncmp(buffer, archive_magic.data(), archive_magic.size()) == 0);
}

bool is_jpeg(const char *buffer) {
  static constexpr std::string_view jpg_magic = "\xFF\xD8\xFF";
  return (strncmp(buffer, jpg_magic.data(), jpg_magic.size()) == 0);
}

bool is_png(const char *buffer) {
  static constexpr std::string_view png_magic = "\x89PNG\r\n\x1A\n";
  return (strncmp(buffer, png_magic.data(), png_magic.size()) == 0);
}

bool is_zip(const char *buffer) {
  static constexpr std::string_view zip_magic = "PK\x03\x04";
  return (strncmp(buffer, zip_magic.data(), zip_magic.size()) == 0);
}

bool is_gzip(const char *buffer) {
  static constexpr std::string_view gzip_magic = "\x1F\x8B";
  return (strncmp(buffer, gzip_magic.data(), gzip_magic.size()) == 0);
}

bool is_pdf(const char *buffer) {
  // 25 50 44 46 2D
  static constexpr std::string_view pdf_magic = "\x25\x50\x44\x46\x2D";
  return (strncmp(buffer, pdf_magic.data(), pdf_magic.size()) == 0);
}


bool is_binary(const char* buffer) {
    return is_elf_header(buffer) || is_archive_header(buffer)
        || is_jpeg(buffer)
        || is_png(buffer)
        || is_zip(buffer)
        || is_gzip(buffer)       
        || is_pdf(buffer);
}