#include "_tgvoip.h"
#include <sstream>

PYBIND11_MODULE(_tgvoip, m) {
    py::register_exception_translator([](std::exception_ptr p) {
        try {
            if (p) std::rethrow_exception(p);
        } catch (const py::not_implemented_error &e) {
            PyErr_SetString(PyExc_NotImplementedError, "");
        }
    });

    py::enum_<NetType>(m, "NetType")
            .value("UNKNOWN", NetType::NET_UNKNOWN)
            .value("GPRS", NetType::GPRS)
            .value("EDGE", NetType::EDGE)
            .value("NET_3G", NetType::NET_3G)
            .value("HSPA", NetType::HSPA)
            .value("LTE", NetType::LTE)
            .value("WIFI", NetType::WIFI)
            .value("ETHERNET", NetType::ETHERNET)
            .value("OTHER_HIGH_SPEED", NetType::OTHER_HIGH_SPEED)
            .value("OTHER_LOW_SPEED", NetType::OTHER_LOW_SPEED)
            .value("DIALUP", NetType::DIALUP)
            .value("OTHER_MOBILE", NetType::OTHER_MOBILE);

    py::enum_<CallState>(m, "CallState")
            .value("WAIT_INIT", CallState::WAIT_INIT)
            .value("WAIT_INIT_ACK", CallState::WAIT_INIT_ACK)
            .value("ESTABLISHED", CallState::ESTABLISHED)
            .value("FAILED", CallState::FAILED)
            .value("RECONNECTING", CallState::RECONNECTING);

    py::enum_<DataSaving>(m, "DataSaving")
            .value("NEVER", DataSaving::NEVER)
            .value("MOBILE", DataSaving::MOBILE)
            .value("ALWAYS", DataSaving::ALWAYS);

    py::class_<Stats>(m, "Stats")
            .def_readonly("bytes_sent_wifi", &Stats::bytes_sent_wifi)
            .def_readonly("bytes_sent_mobile", &Stats::bytes_sent_mobile)
            .def_readonly("bytes_recvd_wifi", &Stats::bytes_recvd_wifi)
            .def_readonly("bytes_recvd_mobile", &Stats::bytes_recvd_mobile)
            .def("__repr__", [](const Stats &s) {
                std::ostringstream repr;
                repr << "<_tgvoip.Stats ";
                repr << "bytes_sent_wifi=" << s.bytes_sent_wifi << " ";
                repr << "bytes_sent_mobile=" << s.bytes_sent_mobile << " ";
                repr << "bytes_recvd_wifi=" << s.bytes_recvd_wifi << " ";
                repr << "bytes_recvd_mobile=" << s.bytes_recvd_mobile << ">";
                return repr.str();
            });

    py::class_<Endpoint>(m, "Endpoint")
            .def(py::init<long, const std::string &, const std::string &, int, const std::string &>())
            .def_readwrite("_id", &Endpoint::id)
            .def_readwrite("ip", &Endpoint::ip)
            .def_readwrite("ipv6", &Endpoint::ipv6)
            .def_readwrite("port", &Endpoint::port)
            .def_readwrite("peer_tag", &Endpoint::peer_tag)
            .def("__repr__", [](const Endpoint &e) {
                std::ostringstream repr;
                repr << "<_tgvoip.Endpoint ";
                repr << "_id=" << e.id << " ";
                repr << "ip=\"" << e.ip << "\" ";
                repr << "ipv6=\"" << e.ipv6 << "\" ";
                repr << "port=" << e.port << ">";
                // repr << "peer_tag=" << e.peer_tag << ">";
                return repr.str();
            });

    py::class_<VoIPController, PyVoIPController>(m, "VoIPController")
            .def(py::init<>())
            .def(py::init<const std::string &>())
            .def("start", &VoIPController::start)
            .def("connect", &VoIPController::connect)
            .def("set_proxy", &VoIPController::set_proxy)
            .def("set_encryption_key", &VoIPController::set_encryption_key)
            .def("set_remote_endpoints", &VoIPController::set_remote_endpoints)
            .def("get_debug_string", &VoIPController::get_debug_string)
            .def("set_network_type", &VoIPController::set_network_type)
            .def("set_mic_mute", &VoIPController::set_mic_mute)
            .def("set_config", &VoIPController::set_config)
            .def("debug_ctl", &VoIPController::debug_ctl)
            .def("get_preferred_relay_id", &VoIPController::get_preferred_relay_id)
            .def("get_last_error", &VoIPController::get_last_error)
            .def("get_stats", &VoIPController::get_stats)
            .def("get_debug_log", &VoIPController::get_debug_log)
            .def("set_audio_output_gain_control_enabled", &VoIPController::set_audio_output_gain_control_enabled)
            .def("set_echo_cancellation_strength", &VoIPController::set_echo_cancellation_strength)
            .def("get_peer_capabilities", &VoIPController::get_peer_capabilities)
            .def("need_rate", &VoIPController::need_rate)
            .def("handle_state_change", &VoIPController::handle_state_change)
            .def("handle_signal_bars_change", &VoIPController::handle_signal_bars_change)
            .def_readonly("persistent_state_file", &VoIPController::persistent_state_file)
            .def_property_readonly_static("LIBTGVOIP_VERSION", &VoIPController::get_version)
            .def_property_readonly_static("CONNECTION_MAX_LAYER", &VoIPController::connection_max_layer);

    py::class_<VoIPServerConfig>(m, "VoIPServerConfig")
            .def_static("set_config", &VoIPServerConfig::set_config);
}
