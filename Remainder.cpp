#include "Remainder.h"

BSTR Remainder::ConvertMBSToBSTR(const string& str) {
    int wslen = ::MultiByteToWideChar(CP_ACP, 0, str.data(), str.length(), NULL, 0);

    BSTR wsdata = ::SysAllocStringLen(NULL, wslen);
    ::MultiByteToWideChar(CP_ACP, 0, str.data(), str.length(), wsdata, wslen);

    return wsdata;
}

// -------------------------------------------------------- Display Tasks --------------------------------------------------- //

int Remainder::readEvent()
{
    //  ------------------------------------------------------
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