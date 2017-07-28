#include <Interpreters/ClusterProxy/ShardWithLocalReplicaBlockInputStream.h>
#include <Interpreters/InterpreterSelectQuery.h>
#include <DataStreams/MaterializingBlockInputStream.h>

namespace DB
{
namespace ClusterProxy
{

void ShardWithLocalReplicaBlockInputStream::readPrefix()
{
    bool use_local_replica = true;

    // StoragePtr table = connection_context.tryGetTable(table_name.database, table_name.table);
    // if (!table)
    //     use_local_replica = false;
    // else if
    // {

    // }


    std::unique_ptr<IProfilingBlockInputStream> stream;

    if (use_local_replica)
    {
        InterpreterSelectQuery interpreter{query_ast, context, to_stage};
        BlockInputStreamPtr executed = interpreter.execute().in;

        /// Materialization is needed, since from remote servers the constants come materialized.
        /// If you do not do this, different types (Const and non-Const) columns will be produced in different threads,
        /// And this is not allowed, since all code is based on the assumption that in the block stream all types are the same.
        auto stream = std::make_unique<MaterializingBlockInputStream>(executed);
    }
    // else
    //     stream = std::make_unique(RemoteBlockInputStream>(
    //                                       std::move(connections), query,


    std::lock_guard<std::mutex> lock(cancel_mutex);

    if (isCancelled())
        return;

    impl = std::move(stream);

    if (progress_callback)
        impl->setProgressCallback(progress_callback);
    if (process_list_elem)
        impl->setProcessListElement(process_list_elem);

    std::stringstream log_str;
    log_str << "Executing locally. Query pipeline:\n";
    impl->dumpTree(log_str);
    LOG_DEBUG(log, log_str.str());
}

void ShardWithLocalReplicaBlockInputStream::cancel()
{
    std::lock_guard<std::mutex> lock(cancel_mutex);

    IProfilingBlockInputStream::cancel();
    if (impl)
        impl->cancel();
}

Block ShardWithLocalReplicaBlockInputStream::readImpl()
{
    Block res;
    if (isCancelled())
        return res;
    return impl->read();
}

}
}
