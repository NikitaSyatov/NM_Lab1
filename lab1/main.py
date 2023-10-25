#!/usr/bin/env python
#GUI application

from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
import PySimpleGUI as sg
import pandas as pd
import matplotlib.pyplot as plt
import ctypes as ct
import tkinter as tk
import sys
import traceback

# In[]:
lib1 = ct.CDLL("lab1/Q_test_lib.so")
lib1.rungeKuttaAdaptive.restype = ct.c_int
lib1.rungeKuttaAdaptive.argtypes = [ct.c_double, ct.c_double, ct.c_double, ct.c_double, ct.c_double, ct.c_double, ct.c_int]

lib1.rungeKutta.restype = ct.c_int
lib1.rungeKutta.argtypes = [ct.c_double, ct.c_double, ct.c_double, ct.c_double, ct.c_int]

# In[]:
lib2 = ct.CDLL("lab1/Q1_lib.so")
lib2.rungeKuttaAdaptive.restype = ct.c_int
lib2.rungeKuttaAdaptive.argtypes = [ct.c_double, ct.c_double, ct.c_double, ct.c_double, ct.c_double, ct.c_double, ct.c_int]

lib2.rungeKutta.restype = ct.c_int
lib2.rungeKutta.argtypes = [ct.c_double, ct.c_double, ct.c_double, ct.c_double, ct.c_int]

# In[]:
lib3 = ct.CDLL("lab1/Q2_lib.so")

lib3.RK4.restype = ct.c_int
lib3.RK4.argtypes = [ct.c_double, ct.c_double, ct.c_double, ct.c_double, ct.c_double, ct.c_double, ct.c_int, ct.c_double, ct.c_double, ct.c_int]

# In[]:

df_tmp = pd.DataFrame({
    '    1    ': [],
    '    2    ': [],
    '    3    ': [],
    '    4    ': [],
    '    5    ': [],
    '    6    ': [],
    '    7    ': [],
    '    8    ': [],
    '    9    ': [],
})

fig, graf = plt.subplots(figsize=(5, 5))
# graf = plt.plot([1, 2, 3, 4], [1, 2, 3, 4])

list_q = ['Тестовая', 'Основная1', 'Основная2']

frame_start_condition1 = [
    [sg.Radio('1', default=True, group_id=1, key="-U1-"), sg.Radio('-1', group_id=1, key="-U-1-")],
]

frame_start_condition2 = [
    [sg.Radio('1', default=True, group_id=3, key="-U2-"), sg.Radio('-1', group_id=3, key="-U-2-")],
]

frame_adaptive = [
    [sg.Radio('on', default=True, group_id=2, key="-ADAPTIVE-"), sg.Radio('off', default=False, group_id=2, key='-DEFAULT-')],
]

frame_output_data = [
    [sg.Output(size=(55, 17), key="-DATA-")],
    [sg.Button("Clear", size=(55, 1))],
]

# In[]:
column1 = [
    [sg.DropDown(list_q, default_value=list_q[0], size=(25, 1), key='-SELECTOR-')],
    [sg.Frame("Начальные Условия", frame_start_condition1, size=(180, 50))],
    [sg.Frame("Н. У. для u2 (основная2):", frame_start_condition2, size=(180, 50))],
    [sg.Frame("Контроль шага", frame_adaptive, size=(180, 50))],
    [sg.Submit(size=(24, 1))],
    [sg.Exit(size=(24, 1))]
]

column2 = [
    [sg.Text("Точность выхода на границу", size=(50, 1))],
    [sg.InputText(default_text="0.0000001", size=(30, 1), key='-EPSOUT-')],
    [sg.Text("Контроль погрешности")],
    [sg.InputText(default_text="0.0001", size=(30, 1), key='-EPS-')],
    [sg.Text("Число шагов")],
    [sg.InputText(default_text="1000", size=(30, 1), key='-NMAX-')],
    [sg.Text("Начальный шаг")],
    [sg.InputText(default_text="0.1", size=(30, 1), key='-HSTART-')],
    [sg.Text("Максимальный X")],
    [sg.InputText(default_text="20", size=(30, 1), key='-XMAX-')],
    [sg.Text("Параметр a: (основная2)")],
    [sg.InputText(default_text="1", size=(30,1), key="-A-")],
]

column_table = [
    [sg.Table(values=df_tmp.values.tolist(), headings=df_tmp.columns.tolist(),
            alternating_row_color='darkblue', key='-TABLE-',
            row_height = 25, vertical_scroll_only=False, size=(200, 100),
            justification='left')],
]

column_graf = [
    [sg.Canvas(key="-CANVAS-")],
]

# In[]:
layout = [
    [sg.Column(column1), sg.VerticalSeparator(), sg.Column(column2), sg.Frame("Данные", frame_output_data, element_justification='right')],
    [sg.HorizontalSeparator()],
    [sg.Frame("Таблица точек", column_table, size=(500, 500), key='-SIZETABLE-'), sg.VerticalSeparator(), sg.Column(column_graf, size=(500, 500), justification='left', key='-SIZEGRAF-')],
]

# In[]:
window = sg.Window('LAB1', layout, finalize=True, resizable=True, grab_anywhere=True)

last_w_size = window.size

canvas_elem = window["-CANVAS-"]
canvas = FigureCanvasTkAgg(fig, master=canvas_elem.Widget)
canvas.get_tk_widget().pack(side='top', fill='both', expand=True)

def update_title(table, headings):
    for cid, text in zip(df_tmp.columns.tolist(), headings):
        table.heading(cid, text=text)

def on_resize(event):
    if (last_w_size != window.size):
        width, height = window.size
        window.Element('-SIZETABLE-').set_size((window.size[0]/2 - 50, window.size[1]))
        window.Element('-SIZEGRAF-').set_size((window.size[0]/2, window.size[1]/1.4))
        window.Element('-CANVAS-').set_size((window.size[0]/2, window.size[1]/1.4))
        canvas_elem.Widget.pack(side="top", fill="both", expand=True)

window.TKroot.bind('<Configure>', on_resize)

while True:                             # The Event Loop
    event, values = window.read()
    window.FindElement('-DATA-').Update('')
    # print(event, values) #debug
    
    if event in (None, 'Exit', 'Cancel'):
        break
    if event == 'Submit':
        select_query = values['-SELECTOR-']
        select_adaptive = values['-ADAPTIVE-']
        id_adapt = True
        u0 = 1. if window["-U1-"].Get() == True else -1.
        u1 = 1. if window["-U2-"].Get() == True else -1.
        if select_query == list_q[0]:
            if select_adaptive:
                lib1.rungeKuttaAdaptive(0., u0, float(window["-HSTART-"].Get()), float(window["-XMAX-"].Get()), float(window["-EPS-"].Get()), float(window["-EPSOUT-"].Get()), int(window["-NMAX-"].Get())) # x0, u0, h0, xmax, eps, eps_out, nmax
            else:
                id_adapt = False
                lib1.rungeKutta(0., u0, float(window["-HSTART-"].Get()), float(window["-XMAX-"].Get()), int(window["-NMAX-"].Get()))
        elif select_query == list_q[1]:
            if select_adaptive:
                lib2.rungeKuttaAdaptive(0., u0, float(window["-HSTART-"].Get()), float(window["-XMAX-"].Get()), float(window["-EPS-"].Get()), float(window["-EPSOUT-"].Get()), int(window["-NMAX-"].Get())) # x0, u0, h0, xmax, eps, eps_out, nmax
            else:
                id_adapt = False
                lib2.rungeKutta(0., u0, float(window["-HSTART-"].Get()), float(window["-XMAX-"].Get()), int(window["-NMAX-"].Get()))
        elif select_query == list_q[2]:
            if select_adaptive:
                lib3.RK4(0., u0, u1, float(window["-HSTART-"].Get()), float(window["-XMAX-"].Get()), float(window["-A-"].Get()), int(window["-NMAX-"].Get()), float(window["-EPS-"].Get()), float(window["-EPSOUT-"].Get()), 1)
            else:
                lib3.RK4(0., u0, u1, float(window["-HSTART-"].Get()), float(window["-XMAX-"].Get()), float(window["-A-"].Get()), int(window["-NMAX-"].Get()), float(window["-EPS-"].Get()), float(window["-EPSOUT-"].Get()), 0)

    
        df = pd.read_table('output.txt', sep = "\t+", engine='python')

        table = window.Element("-TABLE-").Widget

        update_title(table, df.columns.tolist())
        
        window.Element("-TABLE-").Update(values = df.values.tolist())

        if id_adapt:
            count_row = df['x'].shape[0]
            print("\n  U0 = " + str(u0) +
                "\n  n = " + str(count_row-1) + 
                "\n  Число удвоений: " + str(sum(df['c2'].values.tolist())) +
                "\n  Число делений: " + str(sum(df['c1'].values.tolist())) + 
                "\n  Максимальный шаг: " + str(max(df['h'].tolist())) +
                "\n  Минимальный шаг: " + str(min(df['h'].tolist())))
        else:
            count_row = df['x'].shape[0]
            print("\n  U0 = " + str(u0) + "\n  n = " + str(count_row) + "\n  Шаг: " + window["-HSTART-"].Get())
        
        if select_query != list_q[2]:
            graf = plt.plot(pd.Series(df['x']).tolist(), pd.Series(df['v']).tolist(), 'b-')
        else:
            graf = plt.plot(pd.Series(df['v']).tolist(), pd.Series(df["v'"]).tolist())

        if select_query == list_q[0]:
            graf = plt.plot(pd.Series(df['x']).tolist(), pd.Series(df['u']).tolist(), 'r-')
        canvas.draw()
    if event == "Clear":
        fig.clear()
        canvas.draw()
    # try
    # except Exception as e:
    #     exc_type, exc_value, exc_traceback = sys.exc_info()
    
    # traceback.print_exception(exc_type, exc_value, exc_traceback)

window.close()