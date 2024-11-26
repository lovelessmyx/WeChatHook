using System;
using System.Collections.Generic;
using System.IO.Pipes;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
namespace InjectApp
{
    /// <summary>
    /// 命名管道服务器工具类
    /// </summary>
    public class NamedPipesHelper
    {
        NamedPipeServerStream pipeServer;
        public NamedPipesHelper()
        {
            pipeServer = new NamedPipeServerStream("HookServerPipe", PipeDirection.In);
            pipeServer.WaitForConnection();
        }


        public string Read()
        {
            StreamReader reader = new StreamReader(pipeServer);
            string clientResponse = reader.ReadLine();
            return clientResponse;
        }

        public void Send(string str)
        {
            StreamWriter writer = new StreamWriter(pipeServer);
            writer.Write(str);
        }

        
    }
}
