   M S B A M L   `   `   `  ����I  EProcessManager, Version=1.0.0.0, Culture=neutral, PublicKeyToken=null/clr-namespace:ProcessManagerProcessManager  5   	   E'clr-namespace:ProcessManager.ViewModelsProcessManager.ViewModels  5   	       ProcessManager.MainWindow   5   	   W SPresentationCore, Version=6.0.2.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35\ XPresentationFramework, Version=6.0.2.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35e aSystem.Windows.Controls.Ribbon, Version=6.0.2.0, Culture=neutral, PublicKeyToken=b77a5c561934e089R NWindowsBase, Version=6.0.2.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35F 9http://schemas.microsoft.com/winfx/2006/xaml/presentation     5   	   R NSystem.Xaml, Version=6.0.2.0, Culture=neutral, PublicKeyToken=b77a5c561934e089:x,http://schemas.microsoft.com/winfx/2006/xaml     5   	   @d2http://schemas.microsoft.com/expression/blend/2008     5   	   Jmc;http://schemas.openxmlformats.org/markup-compatibility/2006     5   	   .localclr-namespace:ProcessManager     5   	   >
viewModels'clr-namespace:ProcessManager.ViewModels     5   	     � Title$  Process Viewer��5	   	   $	��450��6    $	��800��6-   .��5      �  � RowDefinitions 5   
   �� 5      $
��Auto �6   �� 5      $
��Auto �6   �� 5      $��* �6   .+�5   
   ��  �� Orientation$ 
Horizontal=�6   $��0��6.   $��5q�6;   .+�5      �� $��Refresh��6   $
��10,5q�6J   ��6'   �� *RefreshCommand+6J   d� 5      $��Toggle Auto Refresh��6   $
��10,5q�6d    d� 	IsChecked 69   �� *IsAutoRefreshEnabled+6d   �� 5      $��Interval (sec):��6   $��Center=�6/   $	��5,0q�6J   �� 5      $��50��6\   ��6   �� *RefreshInterval+ �� UpdateSourceTrigger$ PropertyChanged=�6\   �� 5      $��Filter:��6   $��Center=�6'   $
��10,0q�6B   �� 5      $	��100��6W   ��6   �� *
FilterText+$ PropertyChanged=�6W   �� 5      $��Kill Process��6    � ToolTip$% Terminates the selected process��5      ��5      �� *KillProcessCommand+5      �� 5      $��Change Priority��6   $. (Changes priority of the selected process��5      ��5      �� *ChangePriorityCommand+5      �� 5!       �� DisplayMemberPath$
 Name��5"      ��5!      �� *
Priorities+ �� SelectedValue 5#      �� *SelectedPriority+ �� Mode$ TwoWay=�5%      �� 5&   
   $ 
Horizontal=�6   $��1��6.   $��5q�6;   .+�5'      *   $ProcessManager.ProcessDetailsControl  5(      �� 5,   
   $��2��6m   ��6   �� *ProcessView+	 �� SelectedItem	 67   �� *SelectedProcess+$ TwoWay=���5-      �� 5.      
 �� 
TargetType"
 ��6   .�5/      ��  �� Property �@�ToolTip �� Value 60   �� *	StartTime+50       �� View 52      �� 53      .P�54      �� $��ID��6%    �� DisplayMemberBinding 61   �� *Id+ �� HeaderTemplate 55      �� 56      .R�57   "   �� $��ID��6)   ��66   �� *DataContext.SortByIdCommand+ �� RelativeSource ��  �� AncestorType ListView58      5:      �� 5;      $
��Name��6%    63   �� *Name+ 5<      �� 5=      .R�5>   "   �� $
��Name��6)   ��68   �� *DataContext.SortByNameCommand+ ��  ListView5?      5A      5B      5D      5E      5F      