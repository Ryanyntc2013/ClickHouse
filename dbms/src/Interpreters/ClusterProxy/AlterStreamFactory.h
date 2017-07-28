#pragma once

#include <Interpreters/ClusterProxy/IStreamFactory.h>

namespace DB
{

namespace ClusterProxy
{

class AlterStreamFactory final : public IStreamFactory
{
public:
    AlterStreamFactory() = default;

    virtual BlockInputStreamPtr create(
            const Cluster::ShardInfo & shard_info,
            const String & query, const ASTPtr & query_ast, const Context & context,
            const ThrottlerPtr & throttler) override;
};

}

}
