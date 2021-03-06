#ifndef DB_POOL_H
#define DB_POOL_H

#include "soci/soci.h"
#include <boost/thread.hpp>

using namespace std;


/**
 * the connection pool manager is responsible for details such
 * as which database to use (SQLite for development, PostgreSQL on the
 * production server), how many concurrent connections to use, etc.
 */
class DBPool {
public:
  static DBPool* TestPool(); /**< pool for unit tests */
  static DBPool* ProdPool(); /**< pool for normal code */

  /**
   * get global connection pool to construct thread local connection
   *
   * SOCI is not threadsafe per-se, but thread safe connections can be achieved
   * by constructing them on demand. To save on overhead, a global connection
   * pool is provided.
   *
   * actually getting a session is as simple as: `soci::session sql(db_pool->getPool());`
   *
   * for details see: http://soci.sourceforge.net/doc/3.2/multithreading.html
   * @return the global connection pool
   */
  soci::connection_pool& getPool();
private:
  DBPool(string db_name, int size);
  size_t pool_size;
  soci::connection_pool pool;
};

#endif
