using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace InjectApp
{
    public enum EnumRetState
    {
        OK = 0,
        ERROR = 1,
        CAN_NOT_OPEN_PROCESS =2,
        VIRTUAL_ALLOC_ERROR = 3,
        MEMORY_FREE_ERRPR = 4,
        HANDLE_CLOSE_ERROR = 5,
        NONE_DLL_UNLOAD = 6,
    }

}
