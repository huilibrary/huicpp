

#include "hconfigbase.h"

namespace HUICPP {

HRET HConfigBase::ReloadConfig (HCSTRR strConfigFileName) {

    Clear();

    return LoadConfig(strConfigFileName);

}


}