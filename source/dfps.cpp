/*
 * Copyright (C) 2021-2022 Matt Yang
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <iostream>
#include <memory>


#include "utils/misc.h"
#include "dfps.h"
#include "modules/cgroup_listener.h"
#include "modules/dynamic_fps.h"
#include "modules/input_listener.h"
#include "modules/offscreen_monitor.h"
#include "modules/topapp_monitor.h"
#include "utils/sched_ctrl.h"
#include "utils/misc_android.h"
#include <spdlog/spdlog.h>

Dfps::Dfps() {}

Dfps::~Dfps() {}


void Dfps::Start(void) {
    SetSelfSchedHint();

    unique_ptr<FILE, decltype(&pclose)> pipe(popen("dmesg -w", "r"), pclose);
    if (!pipe)
        throw runtime_error("popen failed");

    array<char, 4096> buffer;
    while (fgets(buffer.data(), buffer.size(), pipe.get())){
        if(strstr(buffer.data(), "KERNELORDER")){
            cout << "DMESG-W: " << buffer.data() << endl;
            if(strstr(buffer.data(), "SMARTHZH")){
                SysPeakRefreshRate(120, true);
            }
            if(strstr(buffer.data(), "SMARTHZL")){
                SysPeakRefreshRate(60, true);
            }
        }
    }

    SPDLOG_INFO("Dfps is running");
}

void Dfps::SetSelfSchedHint(void) {
    // heavyworker prio 120
    SchedCtrlSetStaticPrio(0, 120, false);
    HeavyWorker::GetInstance();
    // others prio 98
    SchedCtrlSetStaticPrio(0, 98, false);
}
