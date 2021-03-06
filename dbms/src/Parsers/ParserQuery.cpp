#include <Parsers/ParserQuery.h>
#include <Parsers/ParserQueryWithOutput.h>
#include <Parsers/ParserCreateQuery.h>
#include <Parsers/ParserInsertQuery.h>
#include <Parsers/ParserDropQuery.h>
#include <Parsers/ParserRenameQuery.h>
#include <Parsers/ParserOptimizeQuery.h>
#include <Parsers/ParserUseQuery.h>
#include <Parsers/ParserSetQuery.h>
#include <Parsers/ParserAlterQuery.h>


namespace DB
{


bool ParserQuery::parseImpl(Pos & pos, ASTPtr & node, Expected & expected)
{
    ParserQueryWithOutput query_with_output_p;
    ParserInsertQuery insert_p(end);
    ParserCreateQuery create_p;
    ParserRenameQuery rename_p;
    ParserDropQuery drop_p;
    ParserAlterQuery alter_p;
    ParserUseQuery use_p;
    ParserSetQuery set_p;
    ParserOptimizeQuery optimize_p;

    bool res = query_with_output_p.parse(pos, node, expected)
        || insert_p.parse(pos, node, expected)
        || create_p.parse(pos, node, expected)
        || rename_p.parse(pos, node, expected)
        || drop_p.parse(pos, node, expected)
        || alter_p.parse(pos, node, expected)
        || use_p.parse(pos, node, expected)
        || set_p.parse(pos, node, expected)
        || optimize_p.parse(pos, node, expected);

    return res;
}

}
