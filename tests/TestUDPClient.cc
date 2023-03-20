/*
 * (C) Copyright 2023- ECMWF.
 *
 * This software is licensed under the terms of the Apache Licence version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 * In applying this licence, ECMWF does not waive the privileges and immunities
 * granted to it by virtue of its status as an intergovernmental organisation
 * nor does it submit to any jurisdiction.
 */

#include "eckit/testing/Test.h"

#include "eckit/testing/Test.h"

#include "ecflow/light/ClientAPI.h"

namespace ecfl = ecflow::light;

namespace ecflow::light::testing {

struct MockUDPDispatcher {
    static void dispatch_request(const ConfigurationOptions& cfg, const std::string& request) {
        MockUDPDispatcher::cfg     = cfg;
        MockUDPDispatcher::request = request;
    }

    static ConfigurationOptions cfg;
    static std::string request;
};

ConfigurationOptions MockUDPDispatcher::cfg;
std::string MockUDPDispatcher::request;

CASE("test_udp_client__uses_provided_configuration_to_build_request") {
    ConfigurationOptions cfg;
    cfg.host = "custom_hostname";
    cfg.port = "custom_port";
    EnvironmentOptions env;
    env.task_rid      = "custom_rid";
    env.task_name     = "/path/to/task";
    env.task_password = "custom_password";
    env.task_try_no   = "2";

    {
        ecfl::BaseUDPClientAPI<MockUDPDispatcher> client(cfg);
        client.child_update_meter(env, "meter_name", 42);

        EXPECT(MockUDPDispatcher::cfg.host == cfg.host);
        EXPECT(MockUDPDispatcher::cfg.port == cfg.port);

        EXPECT(MockUDPDispatcher::request.find(R"("task_rid":"custom_rid")") != std::string::npos);
        EXPECT(MockUDPDispatcher::request.find(R"("task_password":"custom_password")") != std::string::npos);
        EXPECT(MockUDPDispatcher::request.find(R"("task_try_no":2)") != std::string::npos);

        EXPECT(MockUDPDispatcher::request.find(R"("path":"/path/to/task")") != std::string::npos);
        EXPECT(MockUDPDispatcher::request.find(R"("name":"meter_name")") != std::string::npos);
        EXPECT(MockUDPDispatcher::request.find(R"("value":"42")") != std::string::npos);
    }
    {
        ecfl::BaseUDPClientAPI<MockUDPDispatcher> client(cfg);
        client.child_update_label(env, "label_name", "label_text");

        EXPECT(MockUDPDispatcher::cfg.host == cfg.host);
        EXPECT(MockUDPDispatcher::cfg.port == cfg.port);

        EXPECT(MockUDPDispatcher::request.find(R"("task_rid":"custom_rid")") != std::string::npos);
        EXPECT(MockUDPDispatcher::request.find(R"("path":"/path/to/task")") != std::string::npos);
        EXPECT(MockUDPDispatcher::request.find(R"("task_password":"custom_password")") != std::string::npos);
        EXPECT(MockUDPDispatcher::request.find(R"("task_try_no":2)") != std::string::npos);

        EXPECT(MockUDPDispatcher::request.find(R"("path":"/path/to/task")") != std::string::npos);
        EXPECT(MockUDPDispatcher::request.find(R"("name":"label_name")") != std::string::npos);
        EXPECT(MockUDPDispatcher::request.find(R"("value":"label_text")") != std::string::npos);
    }
    {
        ecfl::BaseUDPClientAPI<MockUDPDispatcher> client(cfg);
        client.child_update_event(env, "event_name", true);

        EXPECT(MockUDPDispatcher::cfg.host == cfg.host);
        EXPECT(MockUDPDispatcher::cfg.port == cfg.port);

        EXPECT(MockUDPDispatcher::request.find(R"("task_rid":"custom_rid")") != std::string::npos);
        EXPECT(MockUDPDispatcher::request.find(R"("task_password":"custom_password")") != std::string::npos);
        EXPECT(MockUDPDispatcher::request.find(R"("task_try_no":2)") != std::string::npos);

        EXPECT(MockUDPDispatcher::request.find(R"("path":"/path/to/task")") != std::string::npos);
        EXPECT(MockUDPDispatcher::request.find(R"("name":"event_name")") != std::string::npos);
        EXPECT(MockUDPDispatcher::request.find(R"("value":"1")") != std::string::npos);
    }
}

}  // namespace ecflow::light::testing

int main(int argc, char** argv) {
    return eckit::testing::run_tests(argc, argv);
}
