#include <iostream>

#include <fstream>
#include <iterator>

#include "Client.h"

// #include <nlohmann/json.hpp>
// using json = nlohmann::json;

#include <json/json.h>


void help()
{
    std::cout
        << "client -a broker_address -s service_name -i [input.json|-] [-o output]"
        << std::endl;
}

std::string json_requet(const std::string& request_body)
{

   Json::Value root;
   root["protocole"] = "SVC 1.0";
   root["service"] = "service 1";
   root["command"] = "PUT";
   root["body"] = request_body;

   // convert to string
   Json::StreamWriterBuilder builder;
   builder["indentation"] = "";
   std::string s = Json::writeString(builder, root);
   return s;
}

int main(int argc, char *const argv[])
{  
   std::string address("tcp://127.0.0.1:5001");
   std::string serviceName("echo");
   std::string iname;
   std::string oname;

   if(address.empty() || serviceName.empty()) {
      help();
      return EXIT_FAILURE;
   }

   try {
      Client::PayloadSeq payloadSeq;

      payloadSeq.emplace_back(json_requet("Hi !"));

      Client client;

      const auto reply = client.exec(address, serviceName, payloadSeq);

      ENSURE(!reply.empty(), RuntimeError);

      auto begin = std::begin(reply);

      // ENSURE(MDP::Broker::Signature::statusSucess == *begin, RuntimeError);
      if (*begin== MDP::Broker::Signature::statusSucess) {
         // ok 
      } else if (MDP::Broker::Signature::statusFailure == *begin) {
         std::string failed = ("Failure " + *(++begin));
         TRACE(failed);
         throw RuntimeError(failed.c_str());
      }

      ++begin;

      if(std::end(reply) == begin) goto exit;

      if(oname.empty()) {
         std::cout << '[';
         for(;begin != std::end(reply);) {
               std::cout << *begin;
               ++begin;
               if(begin != std::end(reply)) std::cout << ',';
         }
         std::cout << ']';
      } else {
         std::ofstream ofile{oname};
         ofile << '[';
         for(;begin != std::end(reply);) {
               ofile << *begin;
               if(begin != std::end(reply)) ofile << ',';
         }
         ofile << ']';
      }
   } catch(const std::exception &e) {
      TRACE(TraceLevel::Error, e.what());
      return EXIT_FAILURE;
   } catch(...) {
      TRACE(TraceLevel::Error, "unsupported exception");
      return EXIT_FAILURE;
   }
exit:
   return EXIT_SUCCESS;
}
