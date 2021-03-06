//
//  workers.h
//  Workflow
//
//  Created by Кирилл on 20.11.17.
//  Copyright © 2017 Кирилл. All rights reserved.
//

#ifndef WORKERS_H_
#define WORKERS_H_

#include <string>

#include "worker.h"

/**
 * Набор реализаций обработчиков блоков.
 */
namespace workers {

/**
 * Фабрика обработчиков блоков.
 * Подбирает подходящий обработчик исходя из его имени и кол-ва аргументов.
 *
 * @return Обработчик, или если не найден nullptr.
 */
const wkfw::Worker* constructWorker(const size_t ident,
                                    const std::string& name,
                                    const std::vector<std::string>& args);

/**
 * Считывание текстового файла в память, целиком.
 *
 * None -> Text
 */
class ReadFile : public wkfw::Worker {
 public:
  ReadFile(const size_t ident, const std::string& filename)
      : wkfw::Worker(ident,
                     wkfw::WorkerResult::ResultType::TEXT,
                     wkfw::WorkerResult::ResultType::NONE),
        filename(filename) {}

  const wkfw::WorkerResult execute(const wkfw::WorkerResult& previous) const
      throw(wkfw::WorkerExecuteException) override;

 private:
  const std::string filename;
};

/**
 * Запись текста в файл.
 *
 * Text -> None
 */
class WriteFile : public wkfw::Worker {
 public:
  WriteFile(const size_t ident, const std::string& filename)
      : wkfw::Worker(ident,
                     wkfw::WorkerResult::ResultType::NONE,
                     wkfw::WorkerResult::ResultType::TEXT),
        filename(filename) {}

  virtual const wkfw::WorkerResult execute(const wkfw::WorkerResult& previous)
      const throw(wkfw::WorkerExecuteException) override;

 protected:
  WriteFile(const size_t ident,
            const std::string& filename,
            wkfw::WorkerResult::ResultType returnType)
      : wkfw::Worker(ident, returnType, wkfw::WorkerResult::ResultType::TEXT),
        filename(filename) {}

 private:
  const std::string filename;
};

/**
 * Выбор из входного текста строк, разделенных символами переноса строки,
 * содержащих заданное слово.
 *
 * Text -> Text
 */
class Grep : public wkfw::Worker {
 public:
  Grep(const size_t ident, const std::string& pattern)
      : Worker(ident,
               wkfw::WorkerResult::ResultType::TEXT,
               wkfw::WorkerResult::ResultType::TEXT),
        pattern(pattern) {}

  const wkfw::WorkerResult execute(const wkfw::WorkerResult& previous) const
      throw(wkfw::WorkerExecuteException) override;

 private:
  const std::string pattern;
};

/**
 * Лексикогорафическая сортировка входного набора строк.
 *
 * Text -> Text
 */
class Sort : public wkfw::Worker {
 public:
  Sort(const size_t ident)
      : wkfw::Worker(ident,
                     wkfw::WorkerResult::ResultType::TEXT,
                     wkfw::WorkerResult::ResultType::TEXT) {}

  const wkfw::WorkerResult execute(const wkfw::WorkerResult& previous) const
      throw(wkfw::WorkerExecuteException) override;
};

/**
 * Замена слова <паттерн> на слово <замена>.
 *
 * Text -> Text
 */
class Replace : public wkfw::Worker {
 public:
  Replace(const size_t ident,
          const std::string& pattern,
          const std::string& substitution)
      : wkfw::Worker(ident,
                     wkfw::WorkerResult::ResultType::TEXT,
                     wkfw::WorkerResult::ResultType::TEXT),
        pattern(pattern),
        substitution(substitution) {}

  const wkfw::WorkerResult execute(const wkfw::WorkerResult& previous) const
      throw(wkfw::WorkerExecuteException) override;

 private:
  const std::string pattern;
  const std::string substitution;
};

/**
 * Сохранение пришедшего текста в указанном файле и передача дальше.
 *
 * Text -> Text
 */
class Dump : public WriteFile {
 public:
  Dump(const size_t ident, const std::string& filename)
      : WriteFile(ident, filename, wkfw::WorkerResult::ResultType::TEXT) {}

  const wkfw::WorkerResult execute(const wkfw::WorkerResult& previous) const
      throw(wkfw::WorkerExecuteException) override;

 private:
  const std::string filename;
};

}  // namespace workers

#endif /* WORKERS_H_ */
