// #include <string>
// #include <unordered_map>
// #include <functional>
// #include <server/TCPServer.cpp>

// class CommandHandler {

// public:

//     CommandHandler() {

//         // register_function( "exit", );

//     }

//     using command_function = std::function < void( const TCPServer& server, const std::vector<std::string>& args )>;

//     void handleCommand( const std::string& commandText ) {

//     }

// private:
//     std::unordered_map < std::string, command_function> commands_;

//     std::vector<std::string> parse_paramms( const std::string& command_str ) {

//     }

//     void register_function( const std::string& command_name, const command_function& command_function ) {
//         commands_[command_name] = command_function;
//     }


// };