/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "mozilla/ProcInfo.h"

#include <kernel/OS.h>

namespace mozilla {

int GetCycleTimeFrequencyMHz() { return 0; }

nsresult GetCpuTimeSinceProcessStartInMs(uint64_t* aResult) {
  team_usage_info usage;
  if (B_OK != get_team_usage_info(B_CURRENT_TEAM, B_TEAM_USAGE_SELF, &usage)) {
    return NS_ERROR_FAILURE;
  }
  const bigtime_t microseconds = usage.user_time + usage.kernel_time;
  *aResult = microseconds / 1000;
  return NS_OK;
}

nsresult GetGpuTimeSinceProcessStartInMs(uint64_t* aResult) {
  return NS_ERROR_NOT_IMPLEMENTED;
}

ProcInfoPromise::ResolveOrRejectValue GetProcInfoSync(
    nsTArray<ProcInfoRequest>&& aRequests) {
  ProcInfoPromise::ResolveOrRejectValue result;

  HashMap<base::ProcessId, ProcInfo> gathered;
  if (!gathered.reserve(aRequests.Length())) {
    result.SetReject(NS_ERROR_OUT_OF_MEMORY);
    return result;
  }
  for (const auto& request : aRequests) {
    ProcInfo info;

    team_usage_info usage;
    if (B_OK != get_team_usage_info(request.pid, B_TEAM_USAGE_SELF, &usage)) {
      continue;
    }
    const bigtime_t microseconds = usage.user_time + usage.kernel_time;
    const uint64_t  nanoseconds  = microseconds * 1000;
    info.cpuTime = nanoseconds;
    
    info.memory = 0;
    ssize_t cookie_area = 0;
    area_info area;
    while (B_OK == get_next_area_info(request.pid, &cookie_area, &area)) {
      info.memory += area.ram_size;
    }
    
    info.pid     = request.pid;
    info.childId = request.childId;
    info.type    = request.processType;
    info.origin  = request.origin;
    info.windows = std::move(request.windowInfo);
    info.utilityActors = std::move(request.utilityInfo);
 
    int32 cookie_thread = 0;
    thread_info thread;
    while (B_OK == get_next_thread_info(request.pid, &cookie_thread, &thread)) {
      const bigtime_t microseconds = thread.user_time + thread.kernel_time;
      const uint64_t  nanoseconds  = microseconds * 1000;
      
      ThreadInfo threadInfo;
      threadInfo.tid = thread.thread;
      threadInfo.cpuTime = nanoseconds;
      info.threads.AppendElement(threadInfo);
    }
    
    if (!gathered.put(request.pid, std::move(info))) {
      result.SetReject(NS_ERROR_OUT_OF_MEMORY);
      return result;
    }
  }

  // ... and we're done!
  result.SetResolve(std::move(gathered));
  return result;
}

}  // namespace mozilla
