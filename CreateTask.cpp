#include "CreateTask.h"

// One Time Trigger
HRESULT CreateTask::createTimeTrigger(HRESULT& hr, ITrigger* pTrigger, ITaskFolder* pRootFolder, ITaskDefinition* pTask) {
    ITimeTrigger* pTDWMYTrigger = NULL;
    hr = pTrigger->QueryInterface(
        IID_ITimeTrigger, (void**)&pTDWMYTrigger);
    pTrigger->Release();
    if (FAILED(hr))
    {
        printf("\nQueryInterface call failed for ITimeTrigger: %x", hr);
        pRootFolder->Release();
        pTask->Release();
        CoUninitialize();
        return 1;
    }

    string trigger_id = getUUID(32);
    wstring widestr = wstring(trigger_id.begin(), trigger_id.end());
    const wchar_t* trigger_id_w = widestr.c_str();
    hr = pTDWMYTrigger->put_Id(remainder->ConvertMBSToBSTR(trigger_id));
    if (FAILED(hr))
        printf("\nCannot put trigger ID: %x", hr);

    //  Set the task to start at a certain time. The time 
    //  format should be YYYY-MM-DDTHH:MM:SS(+-)(timezone).

    cout << "Enter Start Date and Time: " << endl;
    string start_date = validIOHandlers->getDate();
    string start_time = validIOHandlers->getHourMinute();
    start_date.append("T");
    start_date.append(start_time);
    start_date.append(":00");
    start_date.append("+05:30");

    widestr = wstring(start_date.begin(), start_date.end());
    const wchar_t* start_date_w = widestr.c_str();
    hr = pTDWMYTrigger->put_StartBoundary(remainder->ConvertMBSToBSTR(start_date));
    if (FAILED(hr))
        printf("\nCannot put start boundary on trigger: %x", hr);

    cout << "Enter Task Expiry Date and Time: " << endl;
    string end_date = validIOHandlers->getDate();
    string end_time = validIOHandlers->getHourMinute();
    end_date.append("T");
    end_date.append(end_time);
    end_date.append(":00");
    end_date.append("+05:30");

    widestr = wstring(end_date.begin(), end_date.end());
    const wchar_t* end_date_w = widestr.c_str();
    hr = pTDWMYTrigger->put_EndBoundary(remainder->ConvertMBSToBSTR(end_date));
    if (FAILED(hr))
    {
        printf("\nCannot add end boundary to trigger: %x", hr);
        pRootFolder->Release();
        pTask->Release();
        CoUninitialize();
        return 1;
    }

    pTDWMYTrigger->Release();

    return hr;
}

// Daily Trigger
HRESULT CreateTask::createDailyTrigger(HRESULT& hr, ITrigger* pTrigger, ITaskFolder* pRootFolder, ITaskDefinition* pTask) {
    IDailyTrigger* pTDWMYTrigger = NULL;
    hr = pTrigger->QueryInterface(
        IID_IDailyTrigger, (void**)&pTDWMYTrigger);
    pTrigger->Release();
    if (FAILED(hr))
    {
        printf("\nQueryInterface call failed for IDailyTrigger: %x", hr);
        pRootFolder->Release();
        pTask->Release();
        CoUninitialize();
        return 1;
    }

    string trigger_id = getUUID(32);
    wstring widestr = wstring(trigger_id.begin(), trigger_id.end());
    const wchar_t* trigger_id_w = widestr.c_str();
    hr = pTDWMYTrigger->put_Id(remainder->ConvertMBSToBSTR(trigger_id));
    if (FAILED(hr))
        printf("\nCannot put trigger ID: %x", hr);

    //  Set the task to start at a certain time. The time 
    //  format should be YYYY-MM-DDTHH:MM:SS(+-)(timezone).

    cout << "Enter Start Date and Time: " << endl;
    string start_date = validIOHandlers->getDate();
    string start_time = validIOHandlers->getHourMinute();
    start_date.append("T");
    start_date.append(start_time);
    start_date.append(":00");
    start_date.append("+05:30");

    widestr = wstring(start_date.begin(), start_date.end());
    const wchar_t* start_date_w = widestr.c_str();
    hr = pTDWMYTrigger->put_StartBoundary(remainder->ConvertMBSToBSTR(start_date));
    if (FAILED(hr))
        printf("\nCannot put start boundary on trigger: %x", hr);

    cout << "Enter Task Expiry Date and Time: " << endl;
    string end_date = validIOHandlers->getDate();
    string end_time = validIOHandlers->getHourMinute();
    end_date.append("T");
    end_date.append(end_time);
    end_date.append(":00");
    end_date.append("+05:30");

    widestr = wstring(end_date.begin(), end_date.end());
    const wchar_t* end_date_w = widestr.c_str();
    hr = pTDWMYTrigger->put_EndBoundary(remainder->ConvertMBSToBSTR(end_date));
    if (FAILED(hr))
    {
        printf("\nCannot add end boundary to trigger: %x", hr);
        pRootFolder->Release();
        pTask->Release();
        CoUninitialize();
        return 1;
    }

    //  Define the interval for the daily trigger. An interval of 2 produces an
    //  every other day schedule
    cout << endl;
    if (validIOHandlers->isY("Add Interval [Y/n]? ")) {
        int interval = validIOHandlers->getInt("Enter the number of Intervals: ");
        hr = pTDWMYTrigger->put_DaysInterval((short)interval);
        if (FAILED(hr))
        {
            printf("\nCannot put days interval: %x", hr);
            pRootFolder->Release();
            pTDWMYTrigger->Release();
            pTask->Release();
            CoUninitialize();
            return 1;
        }
    }

    // Add a repetition to the trigger so that it repeats
    // five times.
    cout << endl;
    if (validIOHandlers->isY("Add Repetation principle [Y/n]? ")) {
        IRepetitionPattern* pRepetitionPattern = NULL;
        hr = pTDWMYTrigger->get_Repetition(&pRepetitionPattern);
        pTDWMYTrigger->Release();
        if (FAILED(hr))
        {
            printf("\nCannot get repetition pattern: %x", hr);
            pRootFolder->Release();
            pTask->Release();
            CoUninitialize();
            return 1;
        }

        cout << "Repetation: " << endl;
        string repetation_duration_str = validIOHandlers->getPeriodTime("Enter Duration: ");
        hr = pRepetitionPattern->put_Duration(remainder->ConvertMBSToBSTR(repetation_duration_str));
        if (FAILED(hr))
        {
            printf("\nCannot put repetition duration: %x", hr);
            pRootFolder->Release();
            pRepetitionPattern->Release();
            pTask->Release();
            CoUninitialize();
            return 1;
        }

        string repetation_interval_str = validIOHandlers->getPeriodTime("Enter Interval: ");
        hr = pRepetitionPattern->put_Interval(remainder->ConvertMBSToBSTR(repetation_interval_str));
        pRepetitionPattern->Release();
        if (FAILED(hr))
        {
            printf("\nCannot put repetition interval: %x", hr);
            pRootFolder->Release();
            pTask->Release();
            CoUninitialize();
            return 1;
        }
    }


    return hr;
}

// Weekly Trigger
HRESULT CreateTask::createWeeklyTrigger(HRESULT& hr, ITrigger* pTrigger, ITaskFolder* pRootFolder, ITaskDefinition* pTask) {
    IWeeklyTrigger* pTDWMYTrigger = NULL;
    hr = pTrigger->QueryInterface(
        IID_IWeeklyTrigger, (void**)&pTDWMYTrigger);
    pTrigger->Release();
    if (FAILED(hr))
    {
        printf("\nQueryInterface call failed for IWeeklyTrigger: %x", hr);
        pRootFolder->Release();
        pTask->Release();
        CoUninitialize();
        return 1;
    }

    string trigger_id = getUUID(32);
    wstring widestr = wstring(trigger_id.begin(), trigger_id.end());
    const wchar_t* trigger_id_w = widestr.c_str();
    hr = pTDWMYTrigger->put_Id(remainder->ConvertMBSToBSTR(trigger_id));
    if (FAILED(hr))
        printf("\nCannot put trigger ID: %x", hr);

    //  Set the task to start at a certain time. The time 
    //  format should be YYYY-MM-DDTHH:MM:SS(+-)(timezone).

    cout << "Enter Start Date and Time: " << endl;
    string start_date = validIOHandlers->getDate();
    string start_time = validIOHandlers->getHourMinute();
    start_date.append("T");
    start_date.append(start_time);
    start_date.append(":00");
    start_date.append("+05:30");

    widestr = wstring(start_date.begin(), start_date.end());
    const wchar_t* start_date_w = widestr.c_str();
    hr = pTDWMYTrigger->put_StartBoundary(remainder->ConvertMBSToBSTR(start_date));
    if (FAILED(hr))
        printf("\nCannot put start boundary on trigger: %x", hr);

    cout << "Enter Task Expiry Date and Time: " << endl;
    string end_date = validIOHandlers->getDate();
    string end_time = validIOHandlers->getHourMinute();
    end_date.append("T");
    end_date.append(end_time);
    end_date.append(":00");
    end_date.append("+05:30");

    widestr = wstring(end_date.begin(), end_date.end());
    const wchar_t* end_date_w = widestr.c_str();
    hr = pTDWMYTrigger->put_EndBoundary(remainder->ConvertMBSToBSTR(end_date));
    if (FAILED(hr))
    {
        printf("\nCannot add end boundary to trigger: %x", hr);
        pRootFolder->Release();
        pTask->Release();
        CoUninitialize();
        return 1;
    }

    //  Define the interval for the weekly trigger. 
    //  An interval of 2 produces an
    //  every other week schedule
    cout << endl;
    if (validIOHandlers->isY("Add Interval [Y/n]? ")) {
        int interval = validIOHandlers->getInt("Enter the number of weeks Intervals [Number]: ");
        hr = pTDWMYTrigger->put_WeeksInterval((short)interval);
        if (FAILED(hr))
        {
            printf("\nCannot put weeks interval: %x", hr);
            pRootFolder->Release();
            pTDWMYTrigger->Release();
            pTask->Release();
            CoUninitialize();
            return 1;
        }
    }

    cout << endl;
    if (validIOHandlers->isY("Add Days-of-Week to recurr [Y/n]? ")) {
        int daysOfWeek = validIOHandlers->getInt("Enter the days of week interval [Number] (1 - 7): ");
        hr = pTDWMYTrigger->put_DaysOfWeek((short)daysOfWeek);    // 2 Runs on Monday
        pTDWMYTrigger->Release();
        if (FAILED(hr))
        {
            printf("\nCannot put days of week interval: %x", hr);
            pRootFolder->Release();
            pTask->Release();
            CoUninitialize();
            return 1;
        }
    }

    return hr;
}

// Monthly Trigger
HRESULT CreateTask::createMonthlyTrigger(HRESULT& hr, ITrigger* pTrigger, ITaskFolder* pRootFolder, ITaskDefinition* pTask) {
    IMonthlyTrigger* pTDWMYTrigger = NULL;
    hr = pTrigger->QueryInterface(
        IID_IMonthlyTrigger, (void**)&pTDWMYTrigger);
    pTrigger->Release();
    if (FAILED(hr))
    {
        printf("\nQueryInterface call failed for IMonthlyTrigger: %x", hr);
        pRootFolder->Release();
        pTask->Release();
        CoUninitialize();
        return 1;
    }

    string trigger_id = getUUID(32);
    wstring widestr = wstring(trigger_id.begin(), trigger_id.end());
    const wchar_t* trigger_id_w = widestr.c_str();
    hr = pTDWMYTrigger->put_Id(remainder->ConvertMBSToBSTR(trigger_id));
    if (FAILED(hr))
        printf("\nCannot put trigger ID: %x", hr);

    //  Set the task to start at a certain time. The time 
    //  format should be YYYY-MM-DDTHH:MM:SS(+-)(timezone).

    cout << "Enter Start Date and Time: " << endl;
    string start_date = validIOHandlers->getDate();
    string start_time = validIOHandlers->getHourMinute();
    start_date.append("T");
    start_date.append(start_time);
    start_date.append(":00");
    start_date.append("+05:30");

    widestr = wstring(start_date.begin(), start_date.end());
    const wchar_t* start_date_w = widestr.c_str();
    hr = pTDWMYTrigger->put_StartBoundary(remainder->ConvertMBSToBSTR(start_date));
    if (FAILED(hr))
        printf("\nCannot put start boundary on trigger: %x", hr);

    cout << "Enter Task Expiry Date and Time: " << endl;
    string end_date = validIOHandlers->getDate();
    string end_time = validIOHandlers->getHourMinute();
    end_date.append("T");
    end_date.append(end_time);
    end_date.append(":00");
    end_date.append("+05:30");

    widestr = wstring(end_date.begin(), end_date.end());
    const wchar_t* end_date_w = widestr.c_str();
    hr = pTDWMYTrigger->put_EndBoundary(remainder->ConvertMBSToBSTR(end_date));
    if (FAILED(hr))
    {
        printf("\nCannot add end boundary to trigger: %x", hr);
        pRootFolder->Release();
        pTask->Release();
        CoUninitialize();
        return 1;
    }

    //  Define at which date of month the trigger should execute
    if (validIOHandlers->isY("Set the days of the month during which the task runs [Y/n]? ")) {
        int interval = validIOHandlers->getInt("Enter the Days of Month [Number](1 - 31): ");
        hr = pTDWMYTrigger->put_DaysOfMonth((short)interval);
        if (FAILED(hr))
        {
            printf("\nCannot put weeks interval: %x", hr);
            pRootFolder->Release();
            pTDWMYTrigger->Release();
            pTask->Release();
            CoUninitialize();
            return 1;
        }
    }

    if (validIOHandlers->isY("Set the months of the year during which the task runs [Y/n]? ")) {
        int daysOfWeek = validIOHandlers->getInt("Enter the Month of Year [Number](1 - 12): ");
        hr = pTDWMYTrigger->put_MonthsOfYear((short)daysOfWeek);    // 2 Runs on Febraury
        pTDWMYTrigger->Release();
        if (FAILED(hr))
        {
            printf("\nCannot put days of week interval: %x", hr);
            pRootFolder->Release();
            pTask->Release();
            CoUninitialize();
            return 1;
        }
    }

    return hr;
}

string CreateTask::getUUID(size_t len) {
    static const char x[] = "0123456789abcdef";

    string uuid;
    uuid.reserve(len);

    random_device rd;
    mt19937 gen(rd());

    uniform_int_distribution < > dis(0, sizeof(x) - 2);
    for (size_t i = 0; i < len; i++)
        uuid += x[dis(gen)];

    return uuid;
}

// Registration info
HRESULT CreateTask::registrationInfo(HRESULT& hr, ITaskDefinition* pTask, IRegistrationInfo* pRegInfo, ITaskFolder* pRootFolder) {
    hr = pTask->get_RegistrationInfo(&pRegInfo);
    if (FAILED(hr))
    {
        printf("\nCannot get identification pointer: %x", hr);
        pRootFolder->Release();
        pTask->Release();
        CoUninitialize();
        return 1;
    }

    string author_name = validIOHandlers->getString("Enter the Author name: ");
    wstring widestr = wstring(author_name.begin(), author_name.end());
    const wchar_t* author_name_w = widestr.c_str();
    hr = pRegInfo->put_Author(remainder->ConvertMBSToBSTR(author_name));
    pRegInfo->Release();
    if (FAILED(hr))
    {
        printf("\nCannot put identification info: %x", hr);
        pRootFolder->Release();
        pTask->Release();
        CoUninitialize();
        return 1;
    }

    return hr;
}

// Principal
HRESULT CreateTask::principal(HRESULT& hr, ITaskDefinition* pTask, IPrincipal* pPrincipal, ITaskFolder* pRootFolder) {
    hr = pTask->get_Principal(&pPrincipal);
    if (FAILED(hr))
    {
        printf("\nCannot get principal pointer: %x", hr);
        pRootFolder->Release();
        pTask->Release();
        CoUninitialize();
        return 1;
    }

    //  Set up principal logon type to interactive logon
    hr = pPrincipal->put_LogonType(TASK_LOGON_S4U); // ******* Hide the cmd when notification ****** 
    pPrincipal->Release();
    if (FAILED(hr))
    {
        printf("\nCannot put principal info: %x", hr);
        pRootFolder->Release();
        pTask->Release();
        CoUninitialize();
        return 1;
    }

    return hr;
}

// Settings
HRESULT CreateTask::settings(HRESULT& hr, ITaskDefinition* pTask, ITaskSettings* pSettings, ITaskFolder* pRootFolder) {
    hr = pTask->get_Settings(&pSettings);
    if (FAILED(hr))
    {
        printf("\nCannot get settings pointer: %x", hr);
        pRootFolder->Release();
        pTask->Release();
        CoUninitialize();
        return 1;
    }

    //  Set setting values for the task.  
    hr = pSettings->put_StartWhenAvailable(VARIANT_TRUE);
    hr = pSettings->put_DisallowStartIfOnBatteries(VARIANT_FALSE); // *** settings to change ***
    pSettings->Release();
    if (FAILED(hr))
    {
        printf("\nCannot put setting information: %x", hr);
        pRootFolder->Release();
        pTask->Release();
        CoUninitialize();
        return 1;
    }

    // Set the idle settings for the task.
    IIdleSettings* pIdleSettings = NULL;
    hr = pSettings->get_IdleSettings(&pIdleSettings);
    if (FAILED(hr))
    {
        printf("\nCannot get idle setting information: %x", hr);
        pRootFolder->Release();
        pTask->Release();
        CoUninitialize();
        return 1;
    }

    hr = pIdleSettings->put_WaitTimeout(_bstr_t(L"PT5M"));
    pIdleSettings->Release();
    if (FAILED(hr))
    {
        printf("\nCannot put idle setting information: %x", hr);
        pRootFolder->Release();
        pTask->Release();
        CoUninitialize();
        return 1;
    }

    return hr;
}

// Trigger
HRESULT CreateTask::trigger(HRESULT& hr, ITaskDefinition* pTask, ITriggerCollection* pTriggerCollection, ITrigger* pTrigger, ITaskFolder* pRootFolder, bool isNewTrigger) {
    cout << "Task Recurrsive type..." << endl;
    cout << "1 - One Time Task" << endl;
    cout << "2 - Daily Task" << endl;
    cout << "3 - Weekly Task" << endl;
    cout << "4 - Monthly Task" << endl;
    int trigger_choice = validIOHandlers->getInt("Enter a choice [Number](Default - 1): ");

    TASK_TRIGGER_TYPE2 trigger_type = TASK_TRIGGER_TIME;
    switch (trigger_choice)
    {
    case 1:
        trigger_type = TASK_TRIGGER_TIME;
        break;
    case 2:
        trigger_type = TASK_TRIGGER_DAILY;
        break;
    case 3:
        trigger_type = TASK_TRIGGER_WEEKLY;
        break;
    case 4:
        trigger_type = TASK_TRIGGER_MONTHLY;
        break;
    default:
        break;
    }

    // For a new Trigger
    if (isNewTrigger) {
        hr = pTriggerCollection->Create(trigger_type, &pTrigger);
        pTriggerCollection->Release();
        if (FAILED(hr))
        {
            printf("\nCannot create trigger: %x", hr);
            pRootFolder->Release();
            pTask->Release();
            CoUninitialize();
            return 1;
        }
    }

    // Choose a trigger type
    switch (trigger_choice)
    {
    case 1:
        hr = createTimeTrigger(hr, pTrigger, pRootFolder, pTask);
        break;
    case 2:
        hr = createDailyTrigger(hr, pTrigger, pRootFolder, pTask);
        break;
    case 3:
        hr = createWeeklyTrigger(hr, pTrigger, pRootFolder, pTask);
        break;
    case 4:
        hr = createMonthlyTrigger(hr, pTrigger, pRootFolder, pTask);
        break;
    default:
        hr = createTimeTrigger(hr, pTrigger, pRootFolder, pTask);
        break;
    }

    return hr;
}

// ----------------------------------------------------------- Create Tasks ------------------------------------------ //

int CreateTask::createEvent()
{
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
    //  Create a name for the task.
    string task_name = validIOHandlers->getString("Enter Task name: ");
    wstring widestr = wstring(task_name.begin(), task_name.end());
    const wchar_t* task_name_w = widestr.c_str();

    //  Set the path to Notifier.exe.
    TCHAR path[MAX_PATH] = { 0 };
    GetModuleFileName(NULL, path, MAX_PATH);
    PathRemoveFileSpec(path);
    PathAppend(path, L"Notifier.exe");

    string task_title = validIOHandlers->getString("Enter Task title: ");
    string task_desc = validIOHandlers->getString("Enter Task description: ");

    task_title.append(" ");
    task_title.append(task_desc);
    widestr = wstring(task_title.begin(), task_title.end());
    const wchar_t* args_w = widestr.c_str();

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
    ITaskFolder* pRoot1Folder = NULL;
    //BSTR ddl = NULL;
    //pRootFolder->GetSecurityDescriptor(ATTRIBUTE_SECURITY_INFORMATION, &ddl);

    hr = pService->GetFolder(_bstr_t(L"\\"), &pRoot1Folder);
    if (FAILED(hr))
    {
        printf("Cannot get Root folder pointer: %x", hr);
        pService->Release();
        CoUninitialize();
        return 1;
    }

    VARIANT sddl{};
    ITaskFolder* pRootFolder = NULL;
    hr = pRoot1Folder->CreateFolder(remainder->ConvertMBSToBSTR("WinTaskScheduler"), sddl, &pRootFolder);
    if (FAILED(hr))
    {
        pService->GetFolder(_bstr_t(L"\\WinTaskScheduler"), &pRootFolder);
    }

    //  If the same task exists, remove it.
    pRootFolder->DeleteTask(remainder->ConvertMBSToBSTR(task_name), 0);

    //  Create the task definition object to create the task.
    ITaskDefinition* pTask = NULL;
    hr = pService->NewTask(0, &pTask);

    pService->Release();  // COM clean up.  Pointer is no longer used.
    if (FAILED(hr))
    {
        printf("Failed to CoCreate an instance of the TaskService class: %x", hr);
        pRootFolder->Release();
        CoUninitialize();
        return 1;
    }

    //  ------------------------------------------------------
    //  Get the registration info for setting the identification.
    IRegistrationInfo* pRegInfo = NULL;
    hr = registrationInfo(hr, pTask, pRegInfo, pRootFolder);

    //  ------------------------------------------------------
    //  Create the principal for the task - these credentials
    //  are overwritten with the credentials passed to RegisterTaskDefinition
    IPrincipal* pPrincipal = NULL;
    hr = principal(hr, pTask, pPrincipal, pRootFolder);

    //  ------------------------------------------------------
    //  Create the settings for the task
    ITaskSettings* pSettings = NULL;
    hr = settings(hr, pTask, pSettings, pRootFolder);

    //  ------------------------------------------------------
    //  Get the trigger collection to insert the time trigger.
    ITriggerCollection* pTriggerCollection = NULL;
    hr = pTask->get_Triggers(&pTriggerCollection);
    if (FAILED(hr))
    {
        printf("\nCannot get trigger collection: %x", hr);
        pRootFolder->Release();
        pTask->Release();
        CoUninitialize();
        return 1;
    }

    //  Add the time trigger to the task.
    ITrigger* pTrigger = NULL;
    hr = trigger(hr, pTask, pTriggerCollection, pTrigger, pRootFolder, true);
    
    // Do add interval and repetation for tasks except one time tasks

    //  ------------------------------------------------------
    //  Add an action to the task. This task will execute notepad.exe.     
    IActionCollection* pActionCollection = NULL;

    //  Get the task action collection pointer.
    hr = pTask->get_Actions(&pActionCollection);
    if (FAILED(hr))
    {
        printf("\nCannot get Task collection pointer: %x", hr);
        pRootFolder->Release();
        pTask->Release();
        CoUninitialize();
        return 1;
    }

    //  Create the action, specifying that it is an executable action.
    IAction* pAction = NULL;
    hr = pActionCollection->Create(TASK_ACTION_EXEC, &pAction);
    pActionCollection->Release();
    if (FAILED(hr))
    {
        printf("\nCannot create the action: %x", hr);
        pRootFolder->Release();
        pTask->Release();
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

    //  Set the argumetns
    hr = pExecAction->put_Arguments(remainder->ConvertMBSToBSTR(task_title));

    if (FAILED(hr))
    {
        printf("\nCannot put action arguments: %x", hr);
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

    //  ------------------------------------------------------
    //  Save the task in the root folder.
    IRegisteredTask* pRegisteredTask = NULL;
    hr = pRootFolder->RegisterTaskDefinition(
        remainder->ConvertMBSToBSTR(task_name),
        pTask,
        TASK_CREATE_OR_UPDATE,
        _variant_t(),
        _variant_t(),
        TASK_LOGON_INTERACTIVE_TOKEN,
        _variant_t(L""),
        &pRegisteredTask);
    if (FAILED(hr))
    {
        printf("\nError saving the Task : %x", hr);
        pRootFolder->Release();
        pTask->Release();
        CoUninitialize();
        return 1;
    }

    printf("\n Success! Task successfully registered. \n");

    //  Clean up.
    pRootFolder->Release();
    pTask->Release();
    pRegisteredTask->Release();
    CoUninitialize();
    return 0;
}