   M S B A M L   `   `   `  ����I  EProcessManager, Version=1.0.0.0, Culture=neutral, PublicKeyToken=null/clr-namespace:ProcessManagerProcessManager  5      *    $ProcessManager.ProcessDetailsControl   5      W SPresentationCore, Version=6.0.2.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35\ XPresentationFramework, Version=6.0.2.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35e aSystem.Windows.Controls.Ribbon, Version=6.0.2.0, Culture=neutral, PublicKeyToken=b77a5c561934e089R NWindowsBase, Version=6.0.2.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35F 9http://schemas.microsoft.com/winfx/2006/xaml/presentation     5      R NSystem.Xaml, Version=6.0.2.0, Culture=neutral, PublicKeyToken=b77a5c561934e089:x,http://schemas.microsoft.com/winfx/2006/xaml     5      Jmc;http://schemas.openxmlformats.org/markup-compatibility/2006     5      @d2http://schemas.microsoft.com/expression/blend/2008     5      .localclr-namespace:ProcessManager     5        � DataContext  5	      �� *SelectedProcess+.��5
      �� $��10q�6   .+�5   
   �� $��Process Details��6   $
��Bold"�6+   �� 5   
   ��6   �� *Name+�� 5   
   ��6   �� *Id+�� 5   
   ��6   �� *
CpuUsage+ �� StringFormat$ CPU Usage: {0}%���� 5   
   ��6   �� *Memory+$ Memory: {0} bytes���� 5   
   ��6   �� *
Priority+$ Priority: {0}��9� 5   
   ��6   �� *ThreadCount+$ Thread Count: {0}����5      �� 5       �� 
TargetType" 9�6   .�5      ��  �� Property � y� �� Value$ 	Collapsed=�63    �� Triggers 5      �� 5       �� Binding 6&   �� *initialized+ �� Value True6F   .a�5      ��  � y�$ Visible=�6;   5      5      .��5      �� ��6   �� *	Threads+��5      �� 5      .R�5      �� ��6$   �� *
ThreadId+5       5"      5#      5$      5%      