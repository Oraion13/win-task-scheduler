#include "UpdateTask.h"

HRESULT UpdateTask::updateTriggers(HRESULT& hr, ITaskDefinition* pTask, ITaskFolder* pRootFolder, ITriggerCollection* pTriggerCollection) {
    // -------------------------------------------------
    // Iterate over the triggers to get a trigger
    long pCount = 0;
    pTriggerCollection->get_Count(&pCount);

    while (1) {
        ITrigger* pTrigger = NULL;
        cout << endl;
        cout << "Index\t | \tTask Trigger Type" << endl;
        for (int i = 0; i < pCount; i++) {
            ITrigger* pTrigger_itr = NULL;
            pTriggerCollection->get_Item(i, &pTrigger_itr);

            cout << i + 1 << "\t |";
            TASK_TRIGGER_TYPE2 pTtype = TASK_TRIGGER_DAILY;
            pTrigger_itr->get_Type(&pTtype);

            cout << pTtype << endl;

            pTrigger_itr->Release();
        }

        cout << "Enter '-1' to exit..." << endl;
        int index = validIOHandlers->getInt("Enter a Trigger index [-1 / Number]: ");

        if (index >= pCount) continue;

        if (index < 0) break;

        hr = pTriggerCollection->get_Item(index, &pTrigger);
        
        if (FAILED(hr)) {
            return hr;
        }

        // ***** Do update the existing trigger **** //
        cout << "choosen: " << endl;
        TASK_TRIGGER_TYPE2 pTtype;
        pTrigger->get_Type(&pTtype);

        cout << pTtype << endl;

        pTrigger->Release();
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
        string task_name = validIOHandlers->getString("Enter a Task Name to delete [0 / Name]: ");

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
        // -------------------------------------------------
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
            hr = pActionCollection->get_Item(0, &pAction);
            pActionCollection->Release();
            if (FAILED(hr)) {
                pTask->Release();
                pRootFolder->Release();
                pAction->Release();
                CoUninitialize();

                return 1;
            }

            //  Set the path to Notifier.exe.
            TCHAR path[MAX_PATH] = { 0 };
            GetModuleFileName(NULL, path, MAX_PATH);
            PathRemoveFileSpec(path);
            PathAppend(path, L"Notifier.exe");

            string task_title = validIOHandlers->getString("Enter Task title: ");
            string task_desc = validIOHandlers->getString("Enter Task description: ");

            task_title.append(" ");
            task_title.append(task_desc);
            wstring widestr = wstring(task_title.begin(), task_title.end());
            const wchar_t* args_w = widestr.c_str();

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

            //  Set the argumetns
            hr = pExecAction->put_Arguments(remainder->ConvertMBSToBSTR(task_title));

            if (FAILED(hr))
            {
                printf("\nCannot put action path: %x", hr);
                pRootFolder->Release();
                pTask->Release();
                CoUninitialize();
                return 1;
            }

            //  Set the path of the executable to notepad.exe.
            hr = pExecAction->put_Path(_bstr_t(path));
            pExecAction->Release();
            if (FAILED(hr))
            {
                printf("\nCannot put action path: %x", hr);
                pRootFolder->Release();
                pTask->Release();
                CoUninitialize();
                return 1;
            }

        }

        CoUninitialize();
    }

}