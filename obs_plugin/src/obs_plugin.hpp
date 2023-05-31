#pragma once

#define _WEBSOCKETPP_CPP11_RANDOM_DEVICE_
#define ASIO_STANDALONE
#define _WEBSOCKETPP_CPP11_TYPE_TRAITS_

#pragma warning(push)
#pragma warning(disable : 4267)
#include <nlohmann/json.hpp>
#include <websocketpp/client.hpp>
#include <websocketpp/config/asio_no_tls_client.hpp>
#pragma warning(pop)
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <functional>
#include <map>
#include <mutex>
#include <queue>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

namespace logi
{
    namespace applets
    {
        namespace obs_plugin
        {
            using ws_client = websocketpp::client<websocketpp::config::asio_client>;
            using action_parameters = std::vector<nlohmann::json>;
            using action_invoke_parameters = std::map<std::string, std::string>;

            using sources = std::vector<std::string>;
            using mixers = std::vector<std::string>;

            struct scene_info
            {
                mixers mixers;
                sources sources;
            };

            using scenes = std::map<std::string, scene_info>;
            using collection_to_scenes = std::unordered_map<std::string, scenes>;

            struct new_state_info
            {
                std::string name;
                bool new_state;
                bool is_toggle;
                new_state_info() : name(std::string()), new_state(false), is_toggle(false)
                {
                }
            };

            bool connect();
            void disconnect();
            bool is_connected();

            void _run_forever();
            ws_client::connection_ptr _create_connection();

            void websocket_open_handler(const websocketpp::connection_hdl &connection_handle);
            void websocket_message_handler(const websocketpp::connection_hdl &connection_handle,
                                           const ws_client::message_ptr &response);
            void websocket_close_handler(const websocketpp::connection_hdl &connection_handle);
            void websocket_fail_handler(const websocketpp::connection_hdl &connection_handle);

            bool register_integration();

            bool initialize_actions();
            void uninitialize_actions();
            nlohmann::json register_action(const std::string &action_id,
                                           const std::string &action_name,
                                           const action_parameters &arguments = action_parameters());
            bool register_actions_broadcast();
            void register_regular_actions();
            void register_parameter_actions();

            // Action handlers
            void action_stream_start(const action_invoke_parameters &parameters);
            void action_stream_stop(const action_invoke_parameters &parameters);
            void action_stream_toggle(const action_invoke_parameters &parameters);
            void action_recording_start(const action_invoke_parameters &parameters);
            void action_recording_stop(const action_invoke_parameters &parameters);
            void action_recording_toggle(const action_invoke_parameters &parameters);
            void action_buffer_start(const action_invoke_parameters &parameters);
            void action_buffer_stop(const action_invoke_parameters &parameters);
            void action_buffer_toggle(const action_invoke_parameters &parameters);
            void action_buffer_save(const action_invoke_parameters &parameters);
            void action_desktop_mute(const action_invoke_parameters &parameters);
            void action_desktop_unmute(const action_invoke_parameters &parameters);
            void action_desktop_mute_toggle(const action_invoke_parameters &parameters);
            void action_mic_mute(const action_invoke_parameters &parameters);
            void action_mic_unmute(const action_invoke_parameters &parameters);
            void action_mic_mute_toggle(const action_invoke_parameters &parameters);
            void action_collection_activate(const action_invoke_parameters &parameters);
            void action_scene_activate(const action_invoke_parameters &parameters);
            void action_source_activate(const action_invoke_parameters &parameters);
            void action_source_deactivate(const action_invoke_parameters &parameters);
            void action_source_toggle(const action_invoke_parameters &parameters);
            void action_mixer_mute(const action_invoke_parameters &parameters);
            void action_mixer_unmute(const action_invoke_parameters &parameters);
            void action_mixer_mute_toggle(const action_invoke_parameters &parameters);


            // Action helpers
            void helper_desktop_mute(bool new_state, bool is_toggle);
            void helper_mic_mute(bool new_state, bool is_toggle);
            void helper_source_activate(const std::string &scene_name,
                                        const std::string &source_name,
                                        bool new_state,
                                        bool is_toggle);
            void helper_mixer_mute(const std::string &scene_name,
                                   const std::string &mixer_name,
                                   bool new_state,
                                   bool is_toggle);
            action_parameters helper_get_available_collections();
            action_parameters helper_get_available_scenes();
            action_parameters helper_get_available_sources();
            action_parameters helper_get_available_mixers();
            bool helper_populate_collections();

            // This function is to be used on messages that have to be delieved immediately
            bool send_message(nlohmann::json message);

            void start_loop();
            void stop_loop();
            void loop_function();

            std::atomic<bool> m_shutting_down;
            std::atomic<bool> m_collection_locked;

            std::mutex m_lock;
            ws_client m_websocket;
            bool m_websocket_open;
            websocketpp::connection_hdl m_connection_handle;
            const std::string m_subprotocol = "json";
            const websocketpp::frame::opcode::value m_subprotocol_opcode = websocketpp::frame::opcode::text;
            std::unique_ptr<std::thread> m_websocket_thread;
            uint16_t m_current_port = 9010;

            uint32_t m_current_message_id = 1;

            std::mutex m_compressor_ready_mutex;
            std::condition_variable m_compressor_ready_cv;
            static const long m_update_interval = 1000 / 1;  // 1 fps

            std::mutex m_thread_lock;

            std::thread m_loop_thread;
            bool m_loop_thread_running = false;

            std::chrono::time_point<std::chrono::steady_clock> m_start_time;
            bool m_studio_mode = false;
            int32_t m_total_streamed_bytes = 0;
            int32_t m_total_streamed_frames = 0;

            std::string registered_regular_actions;
            std::string registered_parametarized_actions;

            std::string m_integration_guid;
            std::string m_integration_instance;

            std::mutex m_initialization_mutex;
            std::condition_variable m_initialization_cv;

            // clang-format off
            const std::string s_integration_name            = "OBS";
            const std::string s_integration_author          = "Logitech G";
            const std::string s_integration_description     = "A Logitech G plugin for Open Broadcaster Software, exposing additional actions to G HUB.";
            const std::string s_integration_identifier      = "logi_obs_plugin";
            const std::string s_integration_icon_1          = "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";
            const std::string s_integration_icon_2          = "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";
            // clang-format on

            collection_to_scenes m_obs_collections;  // [Collection Name] = {[Scene Name] = {Source Name}}

            // clang-format off
            namespace actions
            {
                const std::string s_stream_start                = "obs_stream_start";
                const std::string s_stream_stop                 = "obs_stream_stop";
                const std::string s_stream_toggle               = "obs_stream_toggle";
                const std::string s_recording_start             = "obs_recording_start";
                const std::string s_recording_stop              = "obs_recording_stop";
                const std::string s_recording_toggle            = "obs_recording_toggle";
                const std::string s_buffer_start                = "obs_buffer_start";
                const std::string s_buffer_stop                 = "obs_buffer_stop";
                const std::string s_buffer_toggle               = "obs_buffer_toggle";
                const std::string s_buffer_save                 = "obs_buffer_save";
                const std::string s_desktop_mute                = "obs_desktop_mute";
                const std::string s_desktop_unmute              = "obs_desktop_unmute";
                const std::string s_desktop_mute_toggle         = "obs_desktop_mute_toggle";
                const std::string s_mic_mute                    = "obs_mic_mute";
                const std::string s_mic_unmute                  = "obs_mic_unmute";
                const std::string s_mic_mute_toggle             = "obs_mic_mute_toggle";
                const std::string s_collection_activate         = "obs_collection_activate";
                const std::string s_scenes_activate             = "obs_scene_activate";
                const std::string s_source_activate             = "obs_source_activate";
                const std::string s_source_deactivate           = "obs_source_deactivate";
                const std::string s_source_toggle               = "obs_source_toggle";
                const std::string s_mixer_mute                  = "obs_mixer_mute";
                const std::string s_mixer_unmute                = "obs_mixer_unmute";
                const std::string s_mixer_mute_toggle           = "obs_mixer_mute_toggle";

                namespace messages
                {
                    const std::string s_collection_assignment   = "APPLET_OBS_COLLECTION_SELECTION";
                    const std::string s_collection_error        = "APPLET_OBS_COLLECTION_INVALID";
                    const std::string s_scene_assignment        = "APPLET_OBS_SCENE_SELECTION";
                    const std::string s_scene_error             = "APPLET_OBS_SCENE_INVALID";
                    const std::string s_source_assignment       = "APPLET_OBS_SOURCE_SELECTION";
                    const std::string s_source_error            = "APPLET_OBS_SOURCE_INVALID";
                    const std::string s_mixer_assignment        = "APPLET_OBS_MIXER_SELECTION";
                    const std::string s_mixer_error             = "APPLET_OBS_MIXER_INVALID";
                }

                namespace parameters
                {
                    const std::string s_collection_name         = "collection_name";
                    const std::string s_scene_name              = "scene_name";
                    const std::string s_source_name             = "source_name";
                    const std::string s_mixer_name              = "mixer_name";
                }
            }
            // clang-format on
        }
    }
}
