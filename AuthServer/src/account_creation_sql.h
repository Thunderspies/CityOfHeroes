#ifndef ACCOUNT_CREATION_SQL_H
#define ACCOUNT_CREATION_SQL_H

// Targets Assets/DBSchemas/cohauth.sql. Parameters are the validated lowercase
// account and exactly 128 ASCII hash bytes. Lock account before auth so concurrent
// registrations cannot reuse a UID or claim a differently cased account name.
// Keep transaction and session settings unchanged when returning to the pool.
static const char ACCOUNT_CREATION_SQL[] =
    "DECLARE @name varchar(14) = ?, @hash binary(128) = ?, @uid int, @result int = 8; "
    "BEGIN TRY "
    "BEGIN TRANSACTION; "
    "DECLARE @accountExists int, @authExists int; "
    "SELECT @accountExists = COUNT(*) FROM dbo.user_account WITH (TABLOCKX,HOLDLOCK) "
    "WHERE account COLLATE Latin1_General_CI_AS = @name; "
    "SELECT @authExists = COUNT(*) FROM dbo.user_auth WITH (TABLOCKX,HOLDLOCK) "
    "WHERE account COLLATE Latin1_General_CI_AS = @name; "
    "IF @accountExists > 0 OR @authExists > 0 SET @result = 4; "
    "ELSE BEGIN "
    // The table remains exclusively locked until commit. Conversion overflow is
    // caught below; the PHP-compatible empty-table starting UID is ten.
    "SELECT @uid = CONVERT(int,COALESCE(MAX(CONVERT(bigint,uid)) + 1,10)) FROM dbo.user_account; "
    "INSERT dbo.user_account "
    "(uid,forum_id,account,pay_stat,login_flag,warn_flag,block_flag,block_flag2,subscription_flag,queue_level) "
    "VALUES (@uid,@uid,@name,1014,0,0,0,0,0,1); "
    "INSERT dbo.user_auth (account,password,hash_type,salt) VALUES (@name,@hash,1,0); "
    "INSERT dbo.user_data (uid,user_data,user_game_data,user_data_new,user_game_data_new) "
    "VALUES (@uid,0x0,0x0,0x0,0x0); "
    "INSERT dbo.user_server_group (uid,server_group_id) VALUES (@uid,1); "
    "SET @result = 0; END; "
    "COMMIT TRANSACTION; "
    "END TRY BEGIN CATCH "
    "IF @@TRANCOUNT > 0 ROLLBACK TRANSACTION; "
    "SET @result = 8; "
    "END CATCH; "
    "SELECT @result AS cox_account_creation_result;";

#endif
