#include <iostream>
#include <string>
#include "rapidjson/document.h"
#include "data_fetch.h"

/*
    Functions to process API data
*/

int process_json(std::string apiUrl, std::string username, std::string password) {
    // Fetch data from the API
    std::string responseData = fetchDataFromAPI(apiUrl, username, password);

    // Parse the JSON string
    rapidjson::Document doc;
    doc.Parse(responseData.c_str());

    // Check if parsing was successful
    if (!doc.HasParseError() && doc.IsObject()) {
        // Check response contains appropriate data
        if (doc.MemberCount() >= 2) {
            auto lastMember = doc.MemberEnd() - 1;
            const rapidjson::Value &itemArray = lastMember->value;
            // Check item array format
            if (itemArray.IsArray()) {
                std::string url;
                int count = 1;
                // Iterate over object instances
                for (rapidjson::SizeType i = 0; i < itemArray.Size(); ++i) {
                    const rapidjson::Value& item = itemArray[i];
                    // Access object fields
                    for (auto& member : item.GetObject()) {
                        if (member.value.IsString()) {
                            const char* value = member.value.GetString();
                            // Data field name can be accessed with member.name.GetString()

                            // Parse and print out informaton on each event
                            if (std::strcmp(member.name.GetString(), "name") == 0) {
                                std::cout << "\n " << std::endl;
                                std::cout << count << ". " << value << std::endl;
                                count++;
                                std::cout << "-------------------------" << std::endl;
                                // Print out information about the event
                            } else if (std::strcmp(member.name.GetString(), "datetime_start") == 0) {
                                std::cout << "Starts: " << value << std::endl;
                            } else if (std::strcmp(member.name.GetString(), "datetime_end") == 0) {
                                std::cout << "Ends: " << value << std::endl;
                            } else if (std::strcmp(member.name.GetString(), "location_summary") == 0) {
                                std::cout << "Location: " << value << std::endl;
                                if(url != "") {
                                    std::cout << "URL: " << url << std::endl;
                                }
                            } else if (std::strcmp(member.name.GetString(), "description") == 0) {
                                std::cout << "Description: " << value << std::endl;
                                std::cout << "-------------------------" << std::endl;
                            } else if (std::strcmp(member.name.GetString(), "url") == 0) {
                                url = value;
                            }
                        }
                    }
                }
            } else {
                std::cerr << "Object itemArray is not an array" << std::endl;
                return 1;
            }
        } else {
            std::cerr << "Value not found or empty" << std::endl;
            return 1;
        }
    } else {
        std::cerr << "Failed to parse JSON data" << std::endl;
        return 1;
    }

    return 0;
}