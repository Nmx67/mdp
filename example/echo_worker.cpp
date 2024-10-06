#include <iostream>
#include <string>
#include <sstream>

#include "Worker.h"

void help()
{
    std::cout
        << "worker -a broker_address -s service_name"
        << std::endl;
}

int main(int argc, char *const argv[])
{
   std::string address("tcp://127.0.0.1:5001");
   std::string serviceName("echo");
   TRACE(TraceLevel::Info, "Staring worker : service = echo");

   try {
      Worker worker;
      worker.exec(address, serviceName,
         [](zmqpp::message message) {
             /* echo */
             return message;
         }
     );
   } catch(const std::exception &e) {
      std::cerr << "std exception " << e.what() << std::endl;
      return EXIT_FAILURE;
   } catch(...) {
      std::cerr << "unsupported exception" << std::endl;
      return EXIT_FAILURE;
   }

   return EXIT_SUCCESS;
}
