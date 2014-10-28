function varargout = OpenBBoxDBBrowser(varargin)
% OPENBBOXDBBROWSER MATLAB code for OpenBBoxDBBrowser.fig
%      OPENBBOXDBBROWSER, by itself, creates a new OPENBBOXDBBROWSER or raises the existing
%      singleton*.
%
%      H = OPENBBOXDBBROWSER returns the handle to a new OPENBBOXDBBROWSER or the handle to
%      the existing singleton*.
%
%      OPENBBOXDBBROWSER('CALLBACK',hObject,eventData,handles,...) calls the local
%      function named CALLBACK in OPENBBOXDBBROWSER.M with the given input arguments.
%
%      OPENBBOXDBBROWSER('Property','Value',...) creates a new OPENBBOXDBBROWSER or raises the
%      existing singleton*.  Starting from the left, property value pairs are
%      applied to the GUI before OpenBBoxDBBrowser_OpeningFcn gets called.  An
%      unrecognized property name or invalid value makes property application
%      stop.  All inputs are passed to OpenBBoxDBBrowser_OpeningFcn via varargin.
%
%      *See GUI Options on GUIDE's Tools menu.  Choose "GUI allows only one
%      instance to run (singleton)".
%
% See also: GUIDE, GUIDATA, GUIHANDLES

% Edit the above text to modify the response to help OpenBBoxDBBrowser

% Last Modified by GUIDE v2.5 27-Oct-2014 18:57:38

% Begin initialization code - DO NOT EDIT
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
                   'gui_Singleton',  gui_Singleton, ...
                   'gui_OpeningFcn', @OpenBBoxDBBrowser_OpeningFcn, ...
                   'gui_OutputFcn',  @OpenBBoxDBBrowser_OutputFcn, ...
                   'gui_LayoutFcn',  [] , ...
                   'gui_Callback',   []);
if nargin && ischar(varargin{1})
    gui_State.gui_Callback = str2func(varargin{1});
end

if nargout
    [varargout{1:nargout}] = gui_mainfcn(gui_State, varargin{:});
else
    gui_mainfcn(gui_State, varargin{:});
end
% End initialization code - DO NOT EDIT

% --- Executes just before OpenBBoxDBBrowser is made visible.
function OpenBBoxDBBrowser_OpeningFcn(hObject, eventdata, handles, varargin)
% This function has no output args, see OutputFcn.
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% varargin   command line arguments to OpenBBoxDBBrowser (see VARARGIN)

% Choose default command line output for OpenBBoxDBBrowser
clc
handles.output = hObject;
handles.query.taskid = 0;
handles.query.subjectid = 0;
handles.query.datefrom = '0000-00-00';
handles.query.dateto = '0000-00-00';

handles.datatype(1).list = char('[NONE]', 'cumulative time course', 'times');
handles.datatype(2).list = char('[NONE]', 'times');

handles.graphics.type = {'[NONE]', 'cumulative time course', 'times'};
handles.graphics.color = {'r', 'g', 'b', 'c', 'k'}; % for different trials
handles.graphics.linestyle = {'-','--', ':', '-.'}; % for different subjects
handles.graphics.axes = {'time (s)','reward ordinal';'reward ordinal','interval'};
handles.database.dir = '';

addpath(strcat(pwd,'/mksqlite'));

set(handles.date_to2,'Visible', 'off')
set(handles.plot_event1_list,'Visible', 'off')
set(handles.plot_event2_list,'Visible', 'off')
% Update handles structure
guidata(hObject, handles);

% UIWAIT makes OpenBBoxDBBrowser wait for user response (see UIRESUME)
% uiwait(handles.figure1);


% --- Outputs from this function are returned to the command line.
function varargout = OpenBBoxDBBrowser_OutputFcn(hObject, eventdata, handles) 
% varargout  cell array for returning output args (see VARARGOUT);
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Get default command line output from handles structure
varargout{1} = handles.output;

% --- Executes on button press in open_database.
function open_database_Callback(hObject, eventdata, handles)
% hObject    handle to open_database (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
list1 = '[NONE]';
list2 = '[NONE]';

[FileName,PathName] = uigetfile('*.sqlite', 'Select OpenBBox SQLite File');
if FileName ~= 0
    handles.query.dir = strcat(PathName, FileName);
    mksqlite('open', handles.query.dir, 'ro','single');
    set(handles.database_name, 'string', FileName);
    handles.query.subject = mksqlite('SELECT id, tag FROM subject');
    handles.query.taskfile = mksqlite('SELECT id, filename, timeserver, file FROM taskfile');
    for i = 1:length(handles.query.subject)
        list1 = char(list1, handles.query.subject(i).tag);
    end
    for i = 1:length(handles.query.taskfile)
        list2 = char(list2, handles.query.taskfile(i).filename);
    end
end
set(handles.subject_name_list, 'String', list1)
set(handles.taskfile_name_list, 'String', list2)
guidata(hObject, handles);

% --- Executes on selection change in subject_name_list.
function subject_name_list_Callback(hObject, eventdata, handles)
% hObject    handle to subject_name_list (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
index = get(handles.subject_name_list,'Value');
handles.query.subjectid = index - 1;
guidata(hObject, handles);


% --- Executes during object creation, after setting all properties.
function subject_name_list_CreateFcn(hObject, eventdata, handles)
% hObject    handle to subject_name_list (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end

% --- Executes on selection change in taskfile_name_list.
function taskfile_name_list_Callback(hObject, eventdata, handles)
% hObject    handle to taskfile_name_list (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
index = get(handles.taskfile_name_list,'Value');
handles.query.taskid = index - 1;

list = '[NONE]';
if handles.query.taskid ~= 0
	file = handles.query.taskfile(handles.query.taskid).file;
    Caret = find(file==94);
    
    for i = 1:length(Caret)
        if file(Caret(i)-1)==10 && file(Caret(i)+1) < 91 && file(Caret(i)+1) > 64
            j = 0;
            while file(Caret(i)+j) ~= 32 && file(Caret(i)+j) ~=  9
                event(j+1) = file(Caret(i)+j);
                j = j+1;
            end
            list = char(list, char(event));
            clear event
        end
    end
    set(handles.plot_event1_list, 'String', list)
    set(handles.plot_event2_list, 'String', list)
end

guidata(hObject, handles);

% --- Executes during object creation, after setting all properties.
function taskfile_name_list_CreateFcn(hObject, eventdata, handles)
% hObject    handle to taskfile_name_list (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on button press in taskfile_details.
function taskfile_details_Callback(hObject, eventdata, handles)
% hObject    handle to taskfile_details (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
if handles.query.taskid ~= 0
	msgbox(char(handles.query.taskfile(handles.query.taskid).file), handles.query.taskfile(handles.query.taskid).filename);
else
	msgbox('NONE','Task File');
end
guidata(hObject, handles);

% --- Executes during object creation, after setting all properties.
function date_example_CreateFcn(hObject, eventdata, handles)
% hObject    handle to date_example (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called


% --- Executes on button press in search_behaviortask.
function search_behaviortask_Callback(hObject, eventdata, handles)
% hObject    handle to search_behaviortask (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
if ~isempty(handles.query.dir)
    if handles.query.taskid == 0
        msgbox('Require Specific Task')
    else
        taskid = strcat(' AND idtaskfile= ', num2str(handles.query.taskid));
        if handles.query.subjectid == 0
            subjectid = '';
        else
            subjectid = strcat(' AND idsubject=', num2str(handles.query.subjectid));
        end

        if any(str2num(strrep(handles.query.datefrom, '-', ' ')))
            fromdate = strcat(' strftime(''%s'', ''', handles.query.datefrom, ''')');
        else
            fromdate = strcat(' strftime(''%s'', ''1970-01-01'')');
        end

        if any(str2num(strrep(handles.query.dateto, '-', ' ')))
            todate = strcat(' AND strftime(''%s'',''', handles.query.dateto, ''')');
        else
            todate = strcat(' AND strftime(''%s'', ''2050-01-01'')');
        end
        handles.query.command_behaviortask = [];
        handles.query.command_behaviortask = strcat('SELECT id, idsubject, idtaskfile, timestart FROM behaviortask WHERE timeend BETWEEN ',...
                                                fromdate, todate,' AND timestart BETWEEN ', fromdate, todate, subjectid, taskid);
        handles.query.behaviortask = mksqlite(handles.query.command_behaviortask);
        for i = 1:length(handles.query.behaviortask)
            datatotable(i,1).id = handles.query.behaviortask(i).id;
            datatotable(i,1).subject = handles.query.subject(handles.query.behaviortask(i).idsubject).tag;
            datatotable(i,1).task = handles.query.behaviortask(i).idtaskfile;
            x = mksqlite(strcat('SELECT datetime(', handles.query.behaviortask(i).timestart, ',''unixepoch'', ''localtime'')'));
            datatotable(i,1).startdate = getfield(x, char(fieldnames(x)));
            datatotable(i,1).selected = false;
        end
        set(handles.behaviortask_table, 'Data', struct2cell(datatotable)');
        guidata(hObject, handles);
    end
else
    msgbox('NO DATABASE!!')
end



function behaviortask_from_date_edit_Callback(hObject, eventdata, handles)
% hObject    handle to behaviortask_from_date_edit (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
handles.query.datefrom = get(handles.behaviortask_from_date_edit, 'String');
guidata(hObject, handles);


% --- Executes during object creation, after setting all properties.
function behaviortask_from_date_edit_CreateFcn(hObject, eventdata, handles)
% hObject    handle to behaviortask_from_date_edit (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


function behaviortask_to_date_edit_Callback(hObject, eventdata, handles)
% hObject    handle to behaviortask_to_date_edit (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
handles.query.dateto = get(handles.behaviortask_to_date_edit, 'String');
guidata(hObject, handles);


% --- Executes during object creation, after setting all properties.
function behaviortask_to_date_edit_CreateFcn(hObject, eventdata, handles)
% hObject    handle to behaviortask_to_date_edit (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes when entered data in editable cell(s) in behaviortask_table.
function behaviortask_table_CellEditCallback(hObject, eventdata, handles)
% hObject    handle to behaviortask_table (see GCBO)
% eventdata  structure with the following fields (see UITABLE)
%	Indices: row and column indices of the cell(s) edited
%	PreviousData: previous data for the cell(s) edited
%	EditData: string(s) entered by the user
%	NewData: EditData or its converted form set on the Data property. Empty if Data was not changed
%	Error: error string when failed to convert EditData to appropriate value for Data
% handles    structure with handles and user data (see GUIDATA)


% --- Executes when selected cell(s) is changed in behaviortask_table.
function behaviortask_table_CellSelectionCallback(hObject, eventdata, handles)
% hObject    handle to behaviortask_table (see GCBO)
% eventdata  structure with the following fields (see UITABLE)
%	Indices: row and column indices of the cell(s) currently selecteds
% handles    structure with handles and user data (see GUIDATA)
%handles.plot.


% --- Executes on button press in apply.
function apply_Callback(hObject, eventdata, handles)
% hObject    handle to apply (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
table = get(handles.behaviortask_table, 'Data');
handles.present.id = [];
handles.present.subject = {};

j = 0;
k = 1;
for i = 1:size(table, 1)
    if table{i, 5} == 1
        handles.present.legend{k} = strcat(table{i,2}, '@', table{i,4});
        handles.present.id = [handles.present.id table{i, 1}];
        n = strmatch(table{i, 2}, handles.present.subject);
        if isempty(n)
            j = j+1;
            handles.present.subject{j} = table{i, 2};
            handles.present.graphicstyle{k} = strcat(handles.graphics.linestyle{j}, handles.graphics.color{1});
        else
            handles.present.graphicstyle{k} = strcat(handles.graphics.linestyle{n(1)}, handles.graphics.color{length(n)+1});
        end
        k = k+1;
    end
end

list = '[NONE]';
if length(handles.present.id)>0     
    for i = 1:length(handles.present.id)
        command_behaviorevent = strcat('SELECT idpacket, timesec, timeusec, pin, eventtype, pintype, pinContext FROM behavioreventpacket WHERE idtask = '...
                                ,num2str(handles.present.id(i))...
                                ,' AND ((eventtype + pintype) = 1 OR pin = 0)')
        
        handles.present.data(i).behaviorevent = mksqlite(command_behaviorevent);
        list = char(list, num2str(handles.present.id(i)));        
    end
end

set(handles.behavior_task_list, 'String', list)
guidata(hObject, handles);

% --- Executes on selection change in behavior_task_list.
function behavior_task_list_Callback(hObject, eventdata, handles)
% hObject    handle to behavior_task_list (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
contents = cellstr(get(hObject,'String'));
if ~strcmp(contents{get(hObject,'Value')}, '[NONE]')
    set(handles.behaviorevent_table, 'Data', struct2cell(handles.present.data(find(handles.present.id == str2num(contents{get(hObject,'Value')}))).behaviorevent)');
end
guidata(hObject, handles);
% Hints: contents = cellstr(get(hObject,'String')) returns behavior_task_list contents as cell array
%        contents{get(hObject,'Value')} returns selected item from behavior_task_list


% --- Executes during object creation, after setting all properties.
function behavior_task_list_CreateFcn(hObject, eventdata, handles)
% hObject    handle to behavior_task_list (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: popupmenu controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on selection change in plot_datatype_list.
function plot_datatype_list_Callback(hObject, eventdata, handles)
% hObject    handle to plot_datatype_list (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
index = get(handles.plot_datatype_list,'Value');
if index == 2    
    set(handles.plot_event1_list,'Visible', 'on')
    set(handles.plot_type_list, 'String', handles.datatype(1).list)
elseif index == 3
    set(handles.date_to2,'Visible', 'on')
    set(handles.plot_event1_list,'Visible', 'on')
    set(handles.plot_event2_list,'Visible', 'on')
    set(handles.plot_type_list, 'String', handles.datatype(2).list)
elseif index == 1
    set(handles.date_to2,'Visible', 'off')
    set(handles.plot_event1_list,'Visible', 'off')
    set(handles.plot_event2_list,'Visible', 'off')
    set(handles.plot_type_list, 'String', [])
end
handles.present.event1 = [];
handles.present.event2 = [];
guidata(hObject, handles);
% Hints: contents = cellstr(get(hObject,'String')) returns plot_datatype_list contents as cell array
%        contents{get(hObject,'Value')} returns selected item from plot_datatype_list


% --- Executes during object creation, after setting all properties.
function plot_datatype_list_CreateFcn(hObject, eventdata, handles)
% hObject    handle to plot_datatype_list (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: popupmenu controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on selection change in plot_event1_list.
function plot_event1_list_Callback(hObject, eventdata, handles)
% hObject    handle to plot_event1_list (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
contents = cellstr(get(hObject,'String'));
handles.present.event1 = contents{get(hObject,'Value')};
guidata(hObject, handles);
% Hints: contents = cellstr(get(hObject,'String')) returns plot_event1_list contents as cell array
%        contents{get(hObject,'Value')} returns selected item from plot_event1_list


% --- Executes during object creation, after setting all properties.
function plot_event1_list_CreateFcn(hObject, eventdata, handles)
% hObject    handle to plot_event1_list (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: popupmenu controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on selection change in plot_event2_list.
function plot_event2_list_Callback(hObject, eventdata, handles)
% hObject    handle to plot_event2_list (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
contents = cellstr(get(hObject,'String'));
handles.present.event2 = contents{get(hObject,'Value')};
guidata(hObject, handles);
% Hints: contents = cellstr(get(hObject,'String')) returns plot_event2_list contents as cell array
%        contents{get(hObject,'Value')} returns selected item from plot_event2_list


% --- Executes during object creation, after setting all properties.
function plot_event2_list_CreateFcn(hObject, eventdata, handles)
% hObject    handle to plot_event2_list (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: popupmenu controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on selection change in plot_type_list.
function plot_type_list_Callback(hObject, eventdata, handles)
% hObject    handle to plot_type_list (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
contents = cellstr(get(hObject,'String'));
handles.present.graphicstype = [];
handles.present.graphicstype = contents{get(hObject,'Value')};

guidata(hObject, handles);
% Hints: contents = cellstr(get(hObject,'String')) returns plot_type_list contents as cell array
%        contents{get(hObject,'Value')} returns selected item from plot_type_list


% --- Executes during object creation, after setting all properties.
function plot_type_list_CreateFcn(hObject, eventdata, handles)
% hObject    handle to plot_type_list (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: popupmenu controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on button press in plot_button.
function plot_button_Callback(hObject, eventdata, handles)
% hObject    handle to plot_button (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
if ~isempty(handles.present.graphicstype)
    table = get(handles.behaviortask_table, 'Data');
    n = strmatch(handles.present.graphicstype, handles.graphics.type);
    switch n
        case 1
            
        case 2
            for i = 1: length(handles.present.id)
                data = handles.present.data(i).behaviorevent;
                pinname = extractfield(data, 'pinContext');
                indexs = strmatch(upper(handles.present.event1), pinname);
                timesec = extractfield(data, 'timesec');
                handles.present.plot(i).x = timesec(indexs)-timesec(1);
                handles.present.plot(i).y = 1:length(handles.present.plot(i).x);
                figure(2),plot(handles.present.plot(i).x, handles.present.plot(i).y,handles.present.graphicstyle{i})
                hold on
            end
            xlabel(handles.graphics.axes{1,1})
            ylabel(handles.graphics.axes{1,2})
        case 3
            x = 2;
        otherwise
            disp('')
    end
    
    figure(2),legend(handles.present.legend)
end
guidata(hObject, handles);
