/*
 * Copyright (C) 2022 ARM Limited or its affiliates. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef RV2_CONFIG_H__
#define RV2_CONFIG_H__

#include "RTE_Components.h"
#include CMSIS_device_header

//-------- <<< Use Configuration Wizard in Context Menu >>> --------------------

// <h> CMSIS-RTOS2 Test Suite Configuration
//   <o>Test Runner Thread Stack size <128-16384>
//   <i> Defines stack size for test runner thread.
//   <i> Default: 1024
#define MAIN_THREAD_STACK                 1024

// <o> Tick Timer Interval [us] <1-1000000>
// <i> Defines kernel tick timer interval value.
// <i> Default: 1000
#define RTOS2_TICK_FREQ                   1000

// <h> Object Implementation Specific Configuration
// <o> Maximum Thread Flags
// <i> Maximum number of Thread Flags available per Thread object
#define MAX_THREADFLAGS_CNT               31
// <o> Maximum Event Flags
// <i> Maximum number of Event Flags available per Event Flags object
#define MAX_EVENTFLAGS_CNT                24
// <o> Maximum Mutex Locks
// <i> Maximum number of recursive Mutex locks per Mutex object
#define MAX_MUTEX_LOCK_CNT                255
// <o> Maximum Semaphore Tokens
// <i> Maximum number of available tokens per Semaphore object
#define MAX_SEMAPHORE_TOKEN_CNT           255
// </h>

// <h> Disable Test Cases
// <i> Uncheck to disable an individual test case
//   <e0>Kernel Information and Control
//     <q01>TC_osKernelInitialize_1
//     <q02>TC_osKernelGetInfo_1
//     <q03>TC_osKernelGetState_1
//     <q04>TC_osKernelGetState_2
//     <q05>TC_osKernelStart_1
//     <q06>TC_osKernelLock_1
//     <q07>TC_osKernelLock_2
//     <q08>TC_osKernelUnlock_1
//     <q09>TC_osKernelUnlock_2
//     <q10>TC_osKernelRestoreLock_1
//     <q11>TC_osKernelSuspend_1
//     <q12>TC_osKernelResume_1
//     <q13>TC_osKernelGetTickCount_1
//     <q14>TC_osKernelGetTickFreq_1
//     <q15>TC_osKernelGetSysTimerCount_1
//     <q16>TC_osKernelGetSysTimerFreq_1
#define TC_OSKERNEL_EN                    1
#define TC_OSKERNELINITIALIZE_1_EN        1
#define TC_OSKERNELGETINFO_1_EN           1
#define TC_OSKERNELGETSTATE_1_EN          1
#define TC_OSKERNELGETSTATE_2_EN          0
#define TC_OSKERNELSTART_1_EN             1
#define TC_OSKERNELLOCK_1_EN              1
#define TC_OSKERNELLOCK_2_EN              0
#define TC_OSKERNELUNLOCK_1_EN            1
#define TC_OSKERNELUNLOCK_2_EN            0
#define TC_OSKERNELRESTORELOCK_1_EN       1
#define TC_OSKERNELSUSPEND_1_EN           0
#define TC_OSKERNELRESUME_1_EN            0
#define TC_OSKERNELGETTICKCOUNT_EN        1
#define TC_OSKERNELGETTICKFREQ_EN         1
#define TC_OSKERNELGETSYSTIMERCOUNT_EN    1
#define TC_OSKERNELGETSYSTIMERFREQ_EN     1
//   </e>

//   <e0>Thread Management
//     <q01>TC_osThreadNew_1
//     <q02>TC_osThreadNew_2
//     <q03>TC_osThreadNew_3
//     <q04>TC_osThreadNew_4
//     <q05>TC_osThreadNew_5
//     <q06>TC_osThreadNew_6
//     <q07>TC_osThreadNew_7
//     <q08>TC_osThreadGetName_1
//     <q09>TC_osThreadGetId_1
//     <q10>TC_osThreadGetState_1
//     <q11>TC_osThreadGetState_2
//     <q12>TC_osThreadGetState_3
//     <q13>TC_osThreadSetPriority_1
//     <q14>TC_osThreadSetPriority_2
//     <q15>TC_osThreadGetPriority_1
//     <q16>TC_osThreadYield_1
//     <q17>TC_osThreadSuspend_1
//     <q18>TC_osThreadResume_1
//     <q19>TC_osThreadDetach_1
//     <q20>TC_osThreadDetach_2
//     <q21>TC_osThreadJoin_1
//     <q22>TC_osThreadJoin_2
//     <q23>TC_osThreadJoin_3
//     <q24>TC_osThreadExit_1
//     <q25>TC_osThreadTerminate_1
//     <q26>TC_osThreadGetStackSize_1
//     <q27>TC_osThreadGetStackSpace_1
//     <q28>TC_osThreadGetCount_1
//     <q29>TC_osThreadEnumerate_1
//     <q30>TC_ThreadNew
//     <q31>TC_ThreadMultiInstance
//     <q32>TC_ThreadTerminate
//     <q33>TC_ThreadRestart
//     <q34>TC_ThreadPriorityExec
//     <q35>TC_ThreadYield
//     <q36>TC_ThreadSuspendResume
//     <q37>TC_ThreadReturn
//     <q38>TC_ThreadAllocation
#define TC_OSTHREAD_EN                    1
#define TC_OSTHREADNEW_1_EN               1
#define TC_OSTHREADNEW_2_EN               1
#define TC_OSTHREADNEW_3_EN               0
#define TC_OSTHREADNEW_4_EN               1
#define TC_OSTHREADNEW_5_EN               1
#define TC_OSTHREADNEW_6_EN               1
#define TC_OSTHREADNEW_7_EN               1
#define TC_OSTHREADGETNAME_1_EN           1
#define TC_OSTHREADGETID_1_EN             1
#define TC_OSTHREADGGETSTATE_1_EN         1
#define TC_OSTHREADGGETSTATE_2_EN         1
#define TC_OSTHREADGGETSTATE_3_EN         0
#define TC_OSTHREADSETPRIORITY_1_EN       1
#define TC_OSTHREADSETPRIORITY_2_EN       1
#define TC_OSTHREADGETPRIORITY_1_EN       1
#define TC_OSTHREADYIELD_1_EN             1
#define TC_OSTHREADSUSPEND_1_EN           1
#define TC_OSTHREADRESUME_1_EN            1
#define TC_OSTHREADDETACH_1_EN            0
#define TC_OSTHREADDETACH_2_EN            0
#define TC_OSTHREADJOIN_1_EN              0
#define TC_OSTHREADJOIN_2_EN              0
#define TC_OSTHREADJOIN_3_EN              0
#define TC_OSTHREADEXIT_1_EN              1
#define TC_OSTHREADTERMINATE_1_EN         1
#define TC_OSTHREADGETSTACKSIZE_1_EN      0
#define TC_OSTHREADGETSTACKSPACE_1_EN     1
#define TC_OSTHREADGETCOUNT_1_EN          1
#define TC_OSTHREADENUMERATE_1_EN         1
#define TC_THREADNEW_EN                   1
#define TC_THREADMULTIINSTANCE_EN         1
#define TC_THREADTERMINATE_EN             1
#define TC_THREADRESTART_EN               1
#define TC_THREADPRIORITYEXEC_EN          1
#define TC_THREADYIELD_EN                 1
#define TC_THREADSUSPENDRESUME_EN         1
#define TC_THREADRETURN_EN                1
#define TC_THREADALLOCATION_EN            1
//   </e>

//   <e0>Thread Flags
//     <q01>TC_ThreadFlagsMainThread,
//     <q02>TC_ThreadFlagsChildThread,
//     <q03>TC_ThreadFlagsChildToParent
//     <q04>TC_ThreadFlagsChildToChild
//     <q05>TC_ThreadFlagsWaitTimeout
//     <q06>TC_ThreadFlagsCheckTimeout
//     <q07>TC_ThreadFlagsParam
//     <q08>TC_ThreadFlagsInterrupts
#define TC_OSTHREADFLAGS_EN               1
#define TC_THREADFLAGSMAINTHREAD_EN       1
#define TC_THREADFLAGSCHILDTHREAD_EN      1
#define TC_THREADFLAGSCHILDTOPARENT_EN    1
#define TC_THREADFLAGSCHILDTOCHILD_EN     1
#define TC_THREADFLAGSWAITTIMEOUT_EN      1
#define TC_THREADFLAGSCHECKTIMEOUT_EN     1
#define TC_THREADFLAGSPARAM_EN            1
#define TC_THREADFLAGSINTERRUPTS_EN       1
//   </e>

//   <e0>Generic Wait Functions
//     <q01>TC_GenWaitBasic
//     <q02>TC_GenWaitInterrupts
#define TC_OSDELAY_EN                     1
#define TC_GENWAITBASIC_EN                1
#define TC_GENWAITINTERRUPTS_EN           1
//   </e>

//   <e0>Timer Management
//     <q01>TC_osTimerNew_1
//     <q02>TC_osTimerNew_2
//     <q03>TC_osTimerNew_3
//     <q04>TC_osTimerGetName_1
//     <q05>TC_osTimerStart_1
//     <q06>TC_osTimerStop_1
//     <q07>TC_osTimerStop_2
//     <q08>TC_osTimerIsRunning_1
//     <q09>TC_osTimerDelete_1
//     <q10>TC_TimerAllocation
//     <q11>TC_TimerOneShot
//     <q12>TC_TimerPeriodic
#define TC_OSTIMER_EN                     1
#define TC_OSTIMERNEW_1_EN                1
#define TC_OSTIMERNEW_2_EN                1
#define TC_OSTIMERNEW_3_EN                1
#define TC_OSTIMERGETNAME_1_EN            1
#define TC_OSTIMERSTART_1_EN              1
#define TC_OSTIMERSTOP_1_EN               1
#define TC_OSTIMERSTOP_2_EN               1
#define TC_OSTIMERISRUNNING_1_EN          1
#define TC_OSTIMERDELETE_1_EN             1
#define TC_TIMERONESHOT_EN                1
#define TC_TIMERPERIODIC_EN               1
#define TC_TIMERALLOCATION_EN             1
//   </e>

//   <e0>Event Flags
//     <q01>TC_osEventFlagsNew_1
//     <q02>TC_osEventFlagsNew_2
//     <q03>TC_osEventFlagsNew_3
//     <q04>TC_osEventFlagsSet_1
//     <q05>TC_osEventFlagsClear_1
//     <q06>TC_osEventFlagsGet_1
//     <q07>TC_osEventFlagsWait_1
//     <q08>TC_osEventFlagsDelete_1
//     <q09>TC_osEventFlagsGetName_1
//     <q00>TC_EventFlagsAllocation,
//     <q11>TC_EventFlagsInterThreads,
//     <q12>TC_EventFlagsCheckTimeout,
//     <q13>TC_EventFlagsWaitTimeout,
//     <q14>TC_EventFlagsDeleteWaiting
#define TC_OSEVENTFLAGS_EN                1
#define TC_OSEVENTFLAGSNEW_1_EN           1
#define TC_OSEVENTFLAGSNEW_2_EN           1
#define TC_OSEVENTFLAGSNEW_3_EN           1
#define TC_OSEVENTFLAGSSET_1_EN           1
#define TC_OSEVENTFLAGSCLEAR_1_EN         1
#define TC_OSEVENTFLAGSGET_1_EN           1
#define TC_OSEVENTFLAGSWAIT_1_EN          1
#define TC_OSEVENTFLAGSDELETE_1_EN        1
#define TC_OSEVENTFLAGSGETNAME_1_EN       0
#define TC_EVENTFLAGSALLOCATION_EN        1
#define TC_EVENTFLAGSINTERTHREADS_EN      1
#define TC_EVENTFLAGSCHECKTIMEOUT_EN      1
#define TC_EVENTFLAGSWAITTIMEOUT_EN       1
#define TC_EVENTFLAGSDELETEWAITING_EN     1
//   </e>

//   <e0>Mutex Management
//     <q01>TC_osMutexNew_1
//     <q02>TC_osMutexNew_2
//     <q03>TC_osMutexNew_3
//     <q04>TC_osMutexNew_4
//     <q05>TC_osMutexNew_5
//     <q06>TC_osMutexNew_6
//     <q07>TC_osMutexGetName_1
//     <q08>TC_osMutexAcquire_1
//     <q09>TC_osMutexAcquire_2
//     <q10>TC_osMutexRelease_1
//     <q11>TC_osMutexGetOwner_1
//     <q12>TC_osMutexDelete_1
//     <q13>TC_MutexAllocation
//     <q14>TC_MutexCheckTimeout
//     <q15>TC_MutexRobust
//     <q16>TC_MutexPrioInherit
//     <q17>TC_MutexNestedAcquire
//     <q18>TC_MutexPriorityInversion
//     <q19>TC_MutexOwnership
#define TC_OSMUTEX_EN                     1
#define TC_OSMUTEXNEW_1_EN                1
#define TC_OSMUTEXNEW_2_EN                1
#define TC_OSMUTEXNEW_3_EN                1
#define TC_OSMUTEXNEW_4_EN                0
#define TC_OSMUTEXNEW_5_EN                1
#define TC_OSMUTEXNEW_6_EN                1
#define TC_OSMUTEXGETNAME_1_EN            0
#define TC_OSMUTEXACQUIRE_1_EN            1
#define TC_OSMUTEXACQUIRE_2_EN            1
#define TC_OSMUTEXRELEASE_1_EN            1
#define TC_OSMUTEXGETOWNER_1_EN           1
#define TC_OSMUTEXDELETE_1_EN             1
#define TC_MUTEXALLOCATION_EN             1
#define TC_MUTEXCHECKTIMEOUT_EN           1
#define TC_MUTEXROBUST_EN                 0
#define TC_MUTEXPRIOINHERIT_EN            1
#define TC_MUTEXNESTEDACQUIRE_EN          1
#define TC_MUTEXPRIORITYINVERSION_EN      1
#define TC_MUTEXOWNERSHIP_EN              0
//   </e>

//   <e0>Semaphores
//     <q01>TC_osSemaphoreNew_1
//     <q02>TC_osSemaphoreNew_2
//     <q03>TC_osSemaphoreNew_3
//     <q04>TC_osSemaphoreGetName_1
//     <q05>TC_osSemaphoreAcquire_1
//     <q06>TC_osSemaphoreRelease_1
//     <q07>TC_osSemaphoreGetCount_1
//     <q08>TC_osSemaphoreDelete_1
//     <q09>TC_SemaphoreAllocation
//     <q00>TC_SemaphoreCreateAndDelete
//     <q11>TC_SemaphoreObtainCounting
//     <q12>TC_SemaphoreObtainBinary
//     <q13>TC_SemaphoreWaitForBinary
//     <q14>TC_SemaphoreWaitForCounting
//     <q15>TC_SemaphoreZeroCount
//     <q16>TC_SemaphoreWaitTimeout
//     <q17>TC_SemaphoreCheckTimeout
#define TC_OSSEMAPHORE_EN                 1
#define TC_OSSEMAPHORENEW_1_EN            1
#define TC_OSSEMAPHORENEW_2_EN            1
#define TC_OSSEMAPHORENEW_3_EN            1
#define TC_OSSEMAPHOREGETNAME_1_EN        0
#define TC_OSSEMAPHOREACQUIRE_1_EN        1
#define TC_OSSEMAPHORERELEASE_1_EN        1
#define TC_OSSEMAPHOREGETCOUNT_1_EN       1
#define TC_OSSEMAPHOREDELETE_1_EN         1
#define TC_SEMAPHOREALLOCATION_EN         1
#define TC_SEMAPHORECREATEANDDELETE_EN    1
#define TC_SEMAPHOREOBTAINCOUNTING_EN     1
#define TC_SEMAPHOREOBTAINBINARY_EN       1
#define TC_SEMAPHOREWAITFORBINARY_EN      1
#define TC_SEMAPHOREWAITFORCOUNTING_EN    1
#define TC_SEMAPHOREZEROCOUNT_EN          1
#define TC_SEMAPHOREWAITTIMEOUT_EN        1
#define TC_SEMAPHORECHECKTIMEOUT_EN       1
//   </e>

//   <e0>Memory Pool
//     <q01>TC_osMemoryPoolNew_1
//     <q02>TC_osMemoryPoolNew_2
//     <q03>TC_osMemoryPoolNew_3
//     <q04>TC_osMemoryPoolGetName_1
//     <q05>TC_osMemoryPoolAlloc_1
//     <q06>TC_osMemoryPoolFree_1
//     <q07>TC_osMemoryPoolGetCapacity_1
//     <q08>TC_osMemoryPoolGetBlockSize_1
//     <q09>TC_osMemoryPoolGetCount_1
//     <q00>TC_osMemoryPoolGetSpace_1
//     <q11>TC_osMemoryPoolDelete_1
//     <q12>TC_MemPoolAllocation
//     <q13>TC_MemPoolAllocAndFree
//     <q14>TC_MemPoolAllocAndFreeComb
//     <q15>TC_MemPoolZeroInit
#define TC_OSMEMORYPOOL_EN                1
#define TC_OSMEMORYPOOLNEW_1_EN           1
#define TC_OSMEMORYPOOLNEW_2_EN           1
#define TC_OSMEMORYPOOLNEW_3_EN           1
#define TC_OSMEMORYPOOLGETNAME_1_EN       1
#define TC_OSMEMORYPOOLALLOC_1_EN         1
#define TC_OSMEMORYPOOLFREE_1_EN          1
#define TC_OSMEMORYPOOLGETCAPACITY_1_EN   1
#define TC_OSMEMORYPOOLGETBLOCKSIZE_1_EN  1
#define TC_OSMEMORYPOOLGETCOUNT_1_EN      1
#define TC_OSMEMORYPOOLGETSPACE_1_EN      1
#define TC_OSMEMORYPOOLDELETE_1_EN        1
#define TC_MEMPOOLALLOCATION_EN           1
#define TC_MEMPOOLALLOCANDFREE_EN         1
#define TC_MEMPOOLALLOCANDFREECOMB_EN     1
#define TC_MEMPOOLZEROINIT_EN             1
//   </e>

//   <e0>Message Queue
//   <i>Exclude Message Queue test cases from the test suite
//   <i>Message Queue test cases will not appear in test report
//     <q01>TC_osMessageQueueNew_1
//     <q02>TC_osMessageQueueNew_2
//     <q03>TC_osMessageQueueNew_3
//     <q04>TC_osMessageQueueGetName_1
//     <q05>TC_osMessageQueuePut_1
//     <q06>TC_osMessageQueuePut_2
//     <q07>TC_osMessageQueueGet_1
//     <q08>TC_osMessageQueueGet_2
//     <q09>TC_osMessageQueueGetCapacity_1
//     <q10>TC_osMessageQueueGetMsgSize_1
//     <q11>TC_osMessageQueueGetCount_1
//     <q12>TC_osMessageQueueGetSpace_1
//     <q13>TC_osMessageQueueReset_1
//     <q14>TC_osMessageQueueDelete_1
//     <q15>TC_MsgQAllocation
//     <q16>TC_MsgQBasic
//     <q17>TC_MsgQWait
//     <q18>TC_MsgQCheckTimeout
#define TC_OSMESSAGEQUEUE_EN              1
#define TC_OSMESSAGEQUEUENEW_1_EN         1
#define TC_OSMESSAGEQUEUENEW_2_EN         1
#define TC_OSMESSAGEQUEUENEW_3_EN         1
#define TC_OSMESSAGEQUEUEGETNAME_1_EN     0
#define TC_OSMESSAGEQUEUEPUT_1_EN         1
#define TC_OSMESSAGEQUEUEPUT_2_EN         1
#define TC_OSMESSAGEQUEUEGET_1_EN         1
#define TC_OSMESSAGEQUEUEGET_2_EN         1
#define TC_OSMESSAGEQUEUEGETCAPACITY_1_EN 1
#define TC_OSMESSAGEQUEUEGETMSGSIZE_1_EN  1
#define TC_OSMESSAGEQUEUEGETCOUNT_1_EN    1
#define TC_OSMESSAGEQUEUEGETSPACE_1_EN    1
#define TC_OSMESSAGEQUEUERESET_1_EN       1
#define TC_OSMESSAGEQUEUEDELETE_1_EN      1
#define TC_MSGQALLOCATION_EN              1
#define TC_MSGQBASIC_EN                   1
#define TC_MSGQWAIT_EN                    1
#define TC_MSGQCHECKTIMEOUT_EN            1
//   </e>
// </h>
// </h>

#endif /* RV2_CONFIG_H__ */
