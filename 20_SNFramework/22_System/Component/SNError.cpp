#include "SNError.h"


SNErrorCode SNError::ErrorCode = SNErrorNon;


Void SNError::ErrorLogOut()
{
    // まだログシステムがないので、最低限の出力だけ
    if (SNError::ErrorCode != SNErrorNon)
    {
        // ログ出力
    }

	return;
}
