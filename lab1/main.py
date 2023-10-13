#!/usr/bin/env python
#GUI application

from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
import PySimpleGUI as sg
import pandas as pd
import matplotlib.pyplot as plt
import ctypes as ct
import tkinter as tk

# In[]:
lib1 = ct.CDLL("/home/syatov430/VAZHNO/NM_Lab1/lab1/Q_test_lib.so")
lib1.rungeKuttaAdaptive.restype = ct.c_int
lib1.rungeKuttaAdaptive.argtypes = [ct.c_double, ct.c_double, ct.c_double, ct.c_double, ct.c_double, ct.c_double, ct.c_int]

lib1.rungeKutta.restype = ct.c_int
lib1.rungeKutta.argtypes = [ct.c_double, ct.c_double, ct.c_double, ct.c_double, ct.c_int]
# In[]:
df_tmp = pd.DataFrame({
    '    Xi    ': [],
    '    Vi    ': [],
    '    V2i    ': [],
    '    ОЛП    ': [],
    '    Hi    ': [],
    '    C1    ': [],
    '    C2    ': [],
})

fig, graf = plt.subplots(figsize=(5, 5))
# graf = plt.plot([1, 2, 3, 4], [1, 2, 3, 4])

list_q = ['Тестовая', 'Основная1', 'Основная2']

frame_start_condition = [
    [sg.Radio('1', default=True, group_id=1, key="-U1-"), sg.Radio('-1', group_id=1, key="-U-1-")],
]

frame_adaptive = [
    [sg.Radio('on', default=True, group_id=2, key="-ADAPTIVE-"), sg.Radio('off', default=False, group_id=2, key='-DEFAULT-')],
]

frame_output_data = [
    [sg.Output(size=(55, 15), key="-DATA-")],
]

# In[]:
column1 = [
    [sg.DropDown(list_q, default_value=list_q[0], size=(25, 1), key='-SELECTOR-')],
    [sg.Frame("Начальные Условия", frame_start_condition, size=(180, 50))],
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
]

column_table = [
    [sg.Table(values=df_tmp.values.tolist(), headings=df_tmp.columns.tolist(),
            alternating_row_color='darkblue', key='-TABLE-',
            row_height = 25, vertical_scroll_only=False, size=(200, 100),
            justification='left', auto_size_columns=True)],
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
        u0 = 1. if window["-U1-"].Get() == True else -1.
        if select_query == list_q[0]:
            if select_adaptive:
                lib1.rungeKuttaAdaptive(0., u0, float(window["-HSTART-"].Get()), float(window["-XMAX-"].Get()), float(window["-EPS-"].Get()), float(window["-EPSOUT-"].Get()), int(window["-NMAX-"].Get())) # x0, u0, h0, xmax, eps, eps_out, nmax
            else:
                lib1.rungeKutta(0., u0, float(window["-HSTART-"].Get()), float(window["-XMAX-"].Get()), int(window["-NMAX-"].Get()))

        df = pd.read_table('output.txt', sep = "\t+", engine='python')
        
        window["-TABLE-"].update(values = df.values.tolist())

        print("\n  U0 = " + str(u0))

        graf = plt.plot(pd.Series(df['x']).tolist(), pd.Series(df['v']).tolist())
        canvas.draw()

window.close()