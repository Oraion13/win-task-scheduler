#include "UpdateTask.h"

HRESULT UpdateTask::updateTriggers(HRESULT& hr, ITaskDefinition* pTask, ITaskFolder* pRootFolder, ITriggerCollection* pTriggerCollection) {
    // -------------------------------------------------
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
    // Iterate over the triggers to get a trigger
    long pCount = 0;
    pTriggerCollection->get_Count(&pCount);

        cout << endl;
        cout << "Index\t | \tTask Trigger Type | \tStart Boundary\t | \tEnd Boundary" << endl;
        for (int i = 0; i < pCount; i++) {
            ITrigger* pTrigger_itr = NULL;
            pTriggerCollection->get_Item(i+1, &pTrigger_itr);

            cout << i + 1 << "\t | \t";
            // get start boundary
            BSTR* trigg_start = new BSTR;
            pTrigger_itr->get_StartBoundary(trigg_start);

            // get start boundary
            BSTR* trigg_end = new BSTR;
            pTrigger_itr->get_EndBoundary(trigg_end);

            // get trigger type
            TASK_TRIGGER_TYPE2 *pType = new TASK_TRIGGER_TYPE2;
            pTrigger_itr->get_Type(pType);

            // print the details
            cout << trigger_arr[*pType]
                << " | " <<  remainder->callConvertWCSToMBS(trigg_start) 
                << " | " << remainder->callConvertWCSToMBS(trigg_end) << endl;

            // change start boundary
            bool change_flag = false;
            if (validIOHandlers->isY("Change Start Boundary [Y/n]?: ")) {
                cout << "Enter Start Date and Time: " << endl;
                string start_date = validIOHandlers->getDate();
                string start_time = validIOHandlers->getHourMinute();
                start_date.append("T");
                start_date.append(start_time);
                start_date.append(":00");
                start_date.append("+05:30");

                wstring widestr = wstring(start_date.begin(), start_date.end());
                const wchar_t* start_date_w = widestr.c_str();
                hr = pTrigger_itr->put_StartBoundary(remainder->ConvertMBSToBSTR(start_date));
                if (FAILED(hr))
                    printf("\nCannot put start boundary on trigger: %x", hr);

                change_flag = true;
            }

            // change end boundary
            if (change_flag || validIOHandlers->isY("Change End Boundary [Y/n]?: ")) {
                cout << "Enter Task Expiry Date and Time: " << endl;
                string end_date = validIOHandlers->getDate();
                string end_time = validIOHandlers->getHourMinute();
                end_date.append("T");
                end_date.append(end_time);
                end_date.append(":00");
                end_date.append("+05:30");

                wstring widestr = wstring(end_date.begin(), end_date.end());
                const wchar_t* end_date_w = widestr.c_str();
                hr = pTrigger_itr->put_EndBoundary(remainder->ConvertMBSToBSTR(end_date));
                if (FAILED(hr))
                    printf("\nCannot put end boundary on trigger: %x", hr);
            }

            pTrigger_itr->Release();
        }

    return hr;
}

// ------------------------------------------------------ Update Tasks --------------------------------------------------- //
int UpdateTask::updateEvent() {
    cout << "Enter '0' to exit...";
    while (1) {
        // display the tasks
        remainder->readEvent();

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
        pService->Release();
        if (FAILED(hr))
        {
            printf("Cannot get Root folder pointer: %x", hr);
            CoUninitialize();
            return 1;
        }

        cout << endl;
        string task_name = validIOHandlers->getString("Enter a Task Name to Update [0 / Name]: ");

        if (task_name.compare("0") == 0) {
            cout << "Exiting...!" << endl;
            break;
        }
        //  If the same task exists, update it.
        //--------------------------------------------------
        // Get the registered task
        IRegisteredTask* pRegisteredTask = NULL;
        hr = pRootFolder->GetTask(remainder->ConvertMBSToBSTR(task_name), &pRegisteredTask);
        if (FAILED(hr)) {
            pRegisteredTask->Release();
            CoUninitialize();
            return 1;
        }

        // -------------------------------------------------
        // Get the task definition
        ITaskDefinition* pTask = NULL;
        hr = pRegisteredTask->get_Definition(&pTask);
        pRegisteredTask->Release();
        if (FAILED(hr)) {
            pTask->Release();
            CoUninitialize();
            return 1;
        }

        // If need to change the triggers, UPDATE TRIGGERS
        // Get triggers
        ITriggerCollection* pTriggerCollection = NULL;
        hr = pTask->get_Triggers(&pTriggerCollection);
        if (FAILED(hr)) {
            pTriggerCollection->Release();
            pRootFolder->Release();
            CoUninitialize();
            return 1;
        }
        // ------------------------ Update Trigger --------------------------- //
        if (validIOHandlers->isY("Update triggers [Y/n]?: ")) {
            hr = updateTriggers(hr, pTask, pRootFolder, pTriggerCollection);

            if (FAILED(hr)) {
                pTask->Release();
                pRootFolder->Release();
                pTriggerCollection->Release();
                CoUninitialize();

                return 1;
            }
        }

        // ------------------- New Trigger ---------------- //
        if (validIOHandlers->isY("Add a new trigger [Y/n]?: ")) {
            ITrigger* pTrigger = NULL;
            hr = createTask->trigger(hr, pTask, pTriggerCollection, pTrigger, pRootFolder, true);

            if (FAILED(hr)) {
                pTask->Release();
                pRootFolder->Release();
                pTriggerCollection->Release();
                CoUninitialize();

                return 1;
            }
        }

        // put back triggers
        pTask->put_Triggers(pTriggerCollection);
        pTriggerCollection->Release();

        // ----------------- Change Title and Message ---------- //
        if (validIOHandlers->isY("Change Title and Description for Remainder [Y/n]?: ")) {
            IActionCollection* pActionCollection = NULL;
            hr = pTask->get_Actions(&pActionCollection);
            if (FAILED(hr)) {
                pTask->Release();
                pRootFolder->Release();
                pActionCollection->Release();
                CoUninitialize();

                return 1;
            }

            IAction* pAction = NULL;
            hr = pActionCollection->get_Item(1, &pAction);
            pActionCollection->Release();
            if (FAILED(hr)) {
                pTask->Release();
                pRootFolder->Release();
                pAction->Release();
                CoUninitialize();

                return 1;
            }
            
            IExecAction* pExecAction = NULL;
            //  QI for the executable task pointer.
            hr = pAction->QueryInterface(
                IID_IExecAction, (void**)&pExecAction);
            pAction->Release();
            if (FAILED(hr))
            {
                printf("\nQueryInterface call failed for IExecAction: %x", hr);
                pRootFolder->Release();
                pTask->Release();
                CoUninitialize();
                return 1;
            }

            // get the old arguments
            BSTR* prev_args = new BSTR;
            pExecAction->get_Arguments(prev_args);
            
            cout << "Previous args: " << remainder->callConvertWCSToMBS(prev_args) << endl;

            // get arguments
            string task_title = validIOHandlers->getString("Enter Task title: ");
            string task_desc = validIOHandlers->getString("Enter Task description: ");

            task_title.append(" 0809x ");
            task_title.append(task_desc);
            wstring widestr = wstring(task_title.begin(), task_title.end());
            const wchar_t* args_w = widestr.c_str();

            //  Set the argumetns
            hr = pExecAction->put_Arguments(remainder->ConvertMBSToBSTR(task_title));
            pExecAction->Release();
            if (FAILED(hr))
            {
                printf("\nCannot update action arguments: %x", hr);
                pRootFolder->Release();
                pTask->Release();
                CoUninitialize();
                return 1;
            }

            // put back actions
            pTask->put_Actions(pActionCollection);
            pActionCollection->Release();

        }

        pRootFolder->Release();
        pTask->Release();
        CoUninitialize();
    }

}