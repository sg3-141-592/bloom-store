#include "SqliteDataSource.h"

SqliteDataSource::SqliteDataSource(std::shared_ptr<Config> config) {
  _config = config;
}

void SqliteDataSource::start(std::shared_ptr<TSQueue<GenericRecord>> queue) {
  _thread = std::thread([this, queue]() {
    std::cout << "Starting loading messages from database\n";

    int exit = sqlite3_open("data.db", &_db);
    if (exit) {
      throw std::runtime_error("Failed to open database: " +
                               std::string(sqlite3_errmsg(_db)));
    }

    sqlite3_stmt *stmt;
    const char *query = "SELECT id, name, type, country, genres FROM artists;";

    if (sqlite3_prepare_v2(_db, query, -1, &stmt, nullptr) != SQLITE_OK) {
      throw std::runtime_error("Failed to prepare statement: " +
                               std::string(sqlite3_errmsg(_db)));
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
      const unsigned char *id = sqlite3_column_text(stmt, 0);
      const unsigned char *nameText = sqlite3_column_text(stmt, 1);
      const unsigned char *typeText = sqlite3_column_text(stmt, 2);
      const unsigned char *countryText = sqlite3_column_text(stmt, 3);
      const unsigned char *genresText = sqlite3_column_text(stmt, 4);

      std::cout
          << "ID: " << (id ? reinterpret_cast<const char *>(id) : "NULL")
          << ", Name: "
          << (nameText ? reinterpret_cast<const char *>(nameText) : "NULL")
          << ", Type: "
          << (typeText ? reinterpret_cast<const char *>(typeText) : "NULL")
          << ", Country: "
          << (countryText ? reinterpret_cast<const char *>(countryText)
                          : "NULL")
          << ", Genres: "
          << (genresText ? reinterpret_cast<const char *>(genresText) : "NULL")
          << std::endl;
    }

    sqlite3_finalize(stmt);

    sqlite3_close(_db);

    _completed.store(true);

    std::cout << "Finished loading messages from database\n";
  });
}

void SqliteDataSource::stop() {
  _completed.store(true);
  if (_thread.joinable()) {
    _thread.join();
  }
}

SqliteDataSource::~SqliteDataSource() { sqlite3_close(_db); }