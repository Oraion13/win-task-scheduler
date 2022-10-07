#include "Remainder.h"

BSTR Remainder::ConvertMBSToBSTR(const string& str) {
    int wslen = ::MultiByteToWideChar(CP_ACP, 0, str.data(), str.length(), NULL, 0);

    BSTR wsdata = ::SysAllocStringLen(NULL, wslen);
    ::MultiByteToWideChar(CP_ACP, 0, str.data(), str.length(), wsdata, wslen);

    return wsdata;
}

string Remainder::ConvertWCSToMBS(const wchar_t* pstr, long wslen) {
    int len = ::WideCharToMultiByte(CP_ACP, 0, pstr, wslen, NULL, 0, NULL, NULL);

    string dblstr(len, '\0');
    len = ::WideCharToMultiByte(CP_ACP, 0, pstr, wslen, &dblstr[0], len, NULL, NULL);

    return dblstr;
}

string Remainder::callConvertWCSToMBS(BSTR* bstr) {
    UINT length = SysStringLen(*bstr);        // Ask COM for the size of the BSTR
    wchar_t* wcs_string = new wchar_t[length + 1]; // Note: SysStringLen doesn't 
    // include the space needed for the NULL

    wcscpy_s(wcs_string, length + 1, *bstr);

    return ConvertWCSToMBS(wcs_string, wcslen(wcs_string));
}

// -------------------------------------------------------- Display Tasks --------------------------------------------------- //

int Remainder::readEvent()
{
    //  ------------------------------------------------------
    string trigger_arr[] = {
    "TASK_TRIGGER_EVENT",
        "ONE TIME",
        "DAILY",
        "WEEKLY",
        "MONTHLY",
        "TASK_TRIGGER_MONTHLYDOW",
        "TASK_TRIGGER_IDLE",
        "TASK_TRIGGER_REGISTRATION",
        "TASK_TRIGGER_BOOT",
        "TASK_TRIGGER_LOGON",
        "TASK_TRIGGER_SESSION_STATE_CHANGE",
        "TASK_TRIGGER_CUSTOM_TRIGGER_01"
    };

    string action_arr[] = {
        "TASK_ACTION_EXEC",
        "TASK_ACTION_COM_HANDLER",
        "TASK_ACTION_SEND_EMAIL",
        "TASK_ACTION_SHOW_MESSAGE"
    };

    //  Initialize COM.
    HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
    if( FAILED(hr) )
    {
        printf("\nCoInitializeEx failed: %x", hr );
        return 1;
    }

    //  Set general COM security levels.
    hr = CoInitializeSecurity(
        NULL,
        -1,
        NULL,
        NULL,
        RPC_C_AUTHN_LEVEL_PKT_PRIVACY,
        RPC_C_IMP_LEVEL_IMPERSONATE,
        NULL,
        0,
        NULL);

    if( FAILED(hr) )
    {
        printf("\nCoInitializeSecurity failed: %x", hr );
        CoUninitialize();
        return 1;
    }

    //  ------------------------------------------------------
    //  Create an instance of the Task Service. 
    ITaskService *pService = NULL;
    hr = CoCreateInstance( CLSID_TaskScheduler,
                           NULL,
                           CLSCTX_INPROC_SERVER,
                           IID_ITaskService,
                           (void**)&pService );  
    if (FAILED(hr))
    {
          printf("Failed to CoCreate an instance of the TaskService class: %x", hr);
          CoUninitialize();
          return 1;
    }
        
    //  Connect to the task service.
    hr = pService->Connect(_variant_t(), _variant_t(),
        _variant_t(), _variant_t());
    if( FAILED(hr) )
    {
        printf("ITaskService::Connect failed: %x", hr );
        pService->Release();
        CoUninitialize();
        return 1;
    }

    //  ------------------------------------------------------
    //  Get the pointer to the root task folder.
    ITaskFolder *pRootFolder = NULL;
    hr = pService->GetFolder( _bstr_t( L"\\WinTaskScheduler") , &pRootFolder );
    
    pService->Release();
    if( FAILED(hr) )
    {
        printf("Cannot get Root Folder pointer: %x", hr );
        CoUninitialize();
        return 1;
    }
    
    //  -------------------------------------------------------
    //  Get the registered tasks in the folder.
    IRegisteredTaskCollection* pTaskCollection = NULL;
    hr = pRootFolder->GetTasks( NULL, &pTaskCollection );

    pRootFolder->Release();
    if( FAILED(hr) )
    {
        printf("Cannot get the registered tasks.: %x", hr);
        CoUninitialize();
        return 1;
    }

    LONG numTasks = 0;
    hr = pTaskCollection->get_Count(&numTasks);

    if( numTasks == 0 )
     {
        printf("\nNo Tasks are currently running" );
        pTaskCollection->Release();
        CoUninitialize();
        return 1;
     }

    printf("\nNumber of Tasks : %d \n", numTasks );

    TASK_STATE taskState;
    
    for(LONG i=0; i < numTasks; i++)
    {
        cout << endl << "--------------------------------------------------" << endl;
        IRegisteredTask* pRegisteredTask = NULL;
        hr = pTaskCollection->get_Item( _variant_t(i+1), &pRegisteredTask );
        
        if( SUCCEEDED(hr) )
        {
            // Task name
            BSTR taskName = NULL;
            hr = pRegisteredTask->get_Name(&taskName);
            if( SUCCEEDED(hr) )
            {
                printf("\nTask Name: %S", taskName);
                SysFreeString(taskName);

                // Task state
                hr = pRegisteredTask->get_State(&taskState);
                if (SUCCEEDED (hr) )
                    printf("\n\tState: %d", taskState);
                else 
                    printf("\n\tCannot get the registered task state: %x", hr);

                // Task last run time
                DATE last_run_time = NULL;
                hr = pRegisteredTask->get_LastRunTime(&last_run_time);
                COleDateTime last_run_time_c = COleDateTime(last_run_time);
                if (SUCCEEDED(hr)) {
                    printf("\n\tLast Run Time: %f = \t", last_run_time);
                    cout << last_run_time_c.GetDay() << "-";
                    cout << last_run_time_c.GetMonth() << "-";
                    cout << last_run_time_c.GetYear() << " ";
                    cout << last_run_time_c.GetHour() << ":";
                    cout << last_run_time_c.GetMinute() << endl;
                }
                else
                    printf("\n\tCannot get the registered task Last Run Time: %x", hr);

                // Task Next run time
                DATE next_run_time = NULL;
                hr = pRegisteredTask->get_NextRunTime(&next_run_time);
                COleDateTime next_run_time_c = COleDateTime(next_run_time);
                if (SUCCEEDED(hr)) {
                    printf("\n\tNext Run Time: %f = \t", next_run_time);
                    cout << last_run_time_c.GetDay() << "-";
                    cout << last_run_time_c.GetMonth() << "-";
                    cout << last_run_time_c.GetYear() << " ";
                    cout << last_run_time_c.GetHour() << ":";
                    cout << last_run_time_c.GetMinute() << endl;
                }
                else
                    printf("\n\tCannot get the registered task Next Run Time: %x", hr);

                // Get the Task
                ITaskDefinition* pTask = NULL;
                pRegisteredTask->get_Definition(&pTask);
                
                // Get Triggers
                ITriggerCollection* pTriggers = NULL;
                pTask->get_Triggers(&pTriggers);

                // Get Actions
                IActionCollection* pActions = NULL;
                pTask->get_Actions(&pActions);
                pTask->Release();

                // Traverse Triggers
                long triggers_len = 0;
                pTriggers->get_Count(&triggers_len);
                cout << "\n\tIndex\t | \tTask Trigger Type | \tStart Boundary\t | \tEnd Boundary" << endl;
                for (int i = 0; i < triggers_len; i++) {
                    ITrigger* pTrigger_itr = NULL;
                    pTriggers->get_Item(i + 1, &pTrigger_itr);

                    cout << "\t" << i + 1 << "\t | \t";
                    // get start boundary
                    BSTR* trigg_start = new BSTR;
                    pTrigger_itr->get_StartBoundary(trigg_start);

                    // get start boundary
                    BSTR* trigg_end = new BSTR;
                    pTrigger_itr->get_EndBoundary(trigg_end);

                    // get trigger type
                    TASK_TRIGGER_TYPE2* pType = new TASK_TRIGGER_TYPE2;
                    pTrigger_itr->get_Type(pType);

                    // print the details
                    cout << trigger_arr[*pType] << "\t\t"
                        << " | " << callConvertWCSToMBS(trigg_start).substr(0, 10) << " " << callConvertWCSToMBS(trigg_start).substr(11, 18)
                        << " | " << callConvertWCSToMBS(trigg_end).substr(0, 10) << " " << callConvertWCSToMBS(trigg_end).substr(11, 18) << endl;

                    pTrigger_itr->Release();
                }

                pTriggers->Release();

                // Traverse Actions
                long actions_len = 0;
                pActions->get_Count(&actions_len);

                cout << endl;
                for (int i = 1; i <= actions_len; i++) {
                    // Get the action
                    IAction* pAction = NULL;
                    pActions->get_Item(i, &pAction);

                    // get action type
                    TASK_ACTION_TYPE* task_action_type = new TASK_ACTION_TYPE;
                    pAction->get_Type(task_action_type);

                    cout << "\t" << "Action Type   : " << action_arr[*task_action_type] << endl;

                    // get Execution pointer
                    IExecAction* pExecAction = NULL;
                    hr = pAction->QueryInterface(
                        IID_IExecAction, (void**)&pExecAction);

                    // Get exec path and arguments
                    BSTR* exec_path = new BSTR;
                    pExecAction->get_Path(exec_path);
                    BSTR* exec_args = new BSTR;
                    pExecAction->get_Arguments(exec_args);

                    cout << "\t" << "Execution Path: " << callConvertWCSToMBS(exec_path) << endl;
                    cout << "\t" << "Argumetns     : " << callConvertWCSToMBS(exec_args) << endl;

                    pExecAction->Release();
                    pAction->Release();
                }

                pActions->Release();
            }
            else
            {
                printf("\nCannot get the registered task name: %x", hr);
            }
            pRegisteredTask->Release();
        }
        else
        {
            printf("\nCannot get the registered task item at index=%d: %x", i+1, hr);
        }
    }

    pTaskCollection->Release();
    CoUninitialize();
    return 0;
}

// ---------------------------------------------- Delete Tasks ----------------------------------------------- //
int Remainder::deleteEvent() {
    cout << "Enter '0' to exit...";
    while (1) {
        // display the tasks
        readEvent();

        //  ------------------------------------------------------
        //  Initialize COM.
        HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
        if (FAILED(hr))
        {
            printf("\nCoInitializeEx failed: %x", hr);
            return 1;
        }

        //  Set general COM security levels.
        hr = CoInitializeSecurity(
            NULL,
            -1,
            NULL,
            NULL,
            RPC_C_AUTHN_LEVEL_PKT_PRIVACY,
            RPC_C_IMP_LEVEL_IMPERSONATE,
            NULL,
            0,
            NULL);

        if (FAILED(hr))
        {
            printf("\nCoInitializeSecurity failed: %x", hr);
            CoUninitialize();
            return 1;
        }

        //  ------------------------------------------------------
        //  Create an instance of the Task Service. 
        ITaskService* pService = NULL;
        hr = CoCreateInstance(CLSID_TaskScheduler,
            NULL,
            CLSCTX_INPROC_SERVER,
            IID_ITaskService,
            (void**)&pService);
        if (FAILED(hr))
        {
            printf("Failed to create an instance of ITaskService: %x", hr);
            CoUninitialize();
            return 1;
        }

        //  Connect to the task service.
        hr = pService->Connect(_variant_t(), _variant_t(),
            _variant_t(), _variant_t());
        if (FAILED(hr))
        {
            printf("ITaskService::Connect failed: %x", hr);
            pService->Release();
            CoUninitialize();
            return 1;
        }

        //  ------------------------------------------------------
        //  Get the pointer to the root task folder.  This folder will hold the
        //  new task that is registered.
        ITaskFolder* pRootFolder = NULL;

        hr = pService->GetFolder(_bstr_t(L"\\WinTaskScheduler"), &pRootFolder);
        if (FAILED(hr))
        {
            printf("Cannot get Root folder pointer: %x", hr);
            pService->Release();
            CoUninitialize();
            return 1;
        }

        cout << endl;
        string task_name = validIOHandlers->getString("Enter a Task Name to delete [0 / Name]: ");

        if (task_name.compare("0") == 0) {
            cout << "Exiting...!" << endl;
            break;
        }
        //  If the same task exists, remove it.
        pRootFolder->DeleteTask(ConvertMBSToBSTR(task_name), 0);

        pRootFolder->Release();
        pService->Release();
        CoUninitialize();
    }

    return 0;
}