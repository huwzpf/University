using System;
using System.IO;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Forms;
using System.Text.RegularExpressions;

namespace WpfApp1
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
       
        public class Prompt
        {
            private Form prompt;
            private System.Windows.Forms.TextBox textBox;
            private System.Windows.Forms.CheckBox hidden, read, archive, system;
            private Radio file, dir;
            private TreeViewItem path;
            private bool confirmed = false;

            public void Confirm(object sender, EventArgs e)
            {
                if(textBox.Text != "" && (file.Checked || dir.Checked ))
                {
                    if (file.Checked && Regex.IsMatch(textBox.Text, @"^\w{1,8}.(html|txt|php)") )
                    {
                        
                        File.Create(path.Tag as string + "\\" + textBox.Text);
                        prompt.Close();
                        this.confirmed = true;
                    }
                    else if (dir.Checked)
                    {
                        Directory.CreateDirectory(path.Tag as string + "\\" + textBox.Text);
                        prompt.Close();
                        this.confirmed = true;
                    }
                    else
                    {
                        System.Windows.Forms.MessageBox.Show("Incorrect parameteres",
                        "Error",
                        MessageBoxButtons.OK,
                        MessageBoxIcon.Error);
                    }
                }
                else
                {
                    System.Windows.Forms.MessageBox.Show("Incorrect parameteres",
                        "Error",
                        MessageBoxButtons.OK,
                        MessageBoxIcon.Error);
                }
                
            }
            public class Radio : System.Windows.Forms.RadioButton
            {
                public string GroupName { get; set; }
            }

            private void Clicked(object sender, EventArgs e)
            {
                Radio rb = (sender as Radio);

                if (!rb.Checked)
                {
                    foreach (var c in prompt.Controls)
                    {
                        if (c is Radio && (c as Radio).GroupName == rb.GroupName)
                        {
                            (c as Radio).Checked = false;
                        }
                    }

                    rb.Checked = true;
                }
            }

            public (string, string) ShowDialog(TreeViewItem path)
            {
                this.path = path;
                prompt = new Form()
                {
                    Width = 500,
                    Height = 400,
                    FormBorderStyle = FormBorderStyle.FixedDialog,
                    Text = "Create",
                    StartPosition = FormStartPosition.CenterScreen
                };
                System.Windows.Forms.Label textLabel1 = new System.Windows.Forms.Label() { Left = 50, Top = 20, Text = "Name" };
                textBox = new System.Windows.Forms.TextBox() { Left = 50, Top = 50, Width = 400 };
                prompt.Controls.Add(textBox);
                prompt.Controls.Add(textLabel1);

                System.Windows.Forms.Label textLabel2 = new System.Windows.Forms.Label() { Left = 50, Top = 80, Text = "Hidden" };
                hidden = new System.Windows.Forms.CheckBox() { Left = 150, Top = 80, };
                prompt.Controls.Add(textLabel2);
                prompt.Controls.Add(hidden);

                System.Windows.Forms.Label textLabel3 = new System.Windows.Forms.Label() { Left = 50, Top = 110, Text = "Read only" };
                read = new System.Windows.Forms.CheckBox() { Left = 150, Top = 110, };
                prompt.Controls.Add(textLabel3);
                prompt.Controls.Add(read);

                System.Windows.Forms.Label textLabel4 = new System.Windows.Forms.Label() { Left = 50, Top = 140, Text = "Archive" };
                archive = new System.Windows.Forms.CheckBox() { Left = 150, Top = 140, };
                prompt.Controls.Add(textLabel4);
                prompt.Controls.Add(archive);

                System.Windows.Forms.Label textLabel5 = new System.Windows.Forms.Label() { Left = 50, Top = 170, Text = "System" };
                system = new System.Windows.Forms.CheckBox() { Left = 150, Top = 170, };
                prompt.Controls.Add(textLabel5);
                prompt.Controls.Add(system);


                file = new Radio();
                file.Text = "File";
                file.AutoSize = true;
                file.AutoCheck = false;
                file.Top = 260;
                file.Left = 50;
                file.GroupName = "a";
                file.Click += Clicked;
                prompt.Controls.Add(file);

                dir = new Radio();
                dir.Text = "Directory";
                dir.AutoSize = true;
                dir.AutoCheck = false;
                dir.Top = 260;
                dir.Left = 150;
                dir.GroupName = "a";
                dir.Click += Clicked;
                prompt.Controls.Add(dir);



                System.Windows.Forms.Button confirmation = new System.Windows.Forms.Button() { Text = "Ok", Left = 350, Width = 100, Top = 300 };
                confirmation.Click += Confirm;
                prompt.Controls.Add(confirmation);
                
                prompt.ShowDialog();
                if (confirmed)
                {
                    string fullpath = path.Tag as string + "\\" + textBox.Text;

                    if (file.Checked)
                    {
                        FileInfo fi = new FileInfo(fullpath);
                        if (read.Checked) fi.Attributes |= FileAttributes.ReadOnly;
                        if (hidden.Checked) fi.Attributes |= FileAttributes.Hidden;
                        if (system.Checked) fi.Attributes |= FileAttributes.System;
                        if (archive.Checked) fi.Attributes |= FileAttributes.Archive;
                        File.SetAttributes(fullpath, fi.Attributes);
                        return ("file", fullpath);

                    }
                    else
                    {
                        DirectoryInfo di = new DirectoryInfo(fullpath);
                        if (read.Checked) di.Attributes |= FileAttributes.ReadOnly;
                        if (hidden.Checked) di.Attributes |= FileAttributes.Hidden;
                        if (system.Checked) di.Attributes |= FileAttributes.System;
                        if (archive.Checked) di.Attributes |= FileAttributes.Archive;
                        File.SetAttributes(fullpath, di.Attributes);
                        return ("dir", fullpath);
                    }

                }
                else
                {
                    return (null, null);
                }
   
            }
        }

        public MainWindow()
        {
            InitializeComponent();
        }

        private void Open(object sender, RoutedEventArgs e)
        {
            var dlg = new FolderBrowserDialog() { Description = "Select directory to open" };
            DialogResult result = dlg.ShowDialog();
            DirectoryInfo dirinfo = new DirectoryInfo(dlg.SelectedPath);
            TreeViewItem root = new TreeViewItem { Header = "Files", Tag = "Files"};
            tv.Items.Add(root);
            showContents(dirinfo, root);
            root.IsExpanded = true;
        }
        private void Exit(object sender, RoutedEventArgs e)
        {
            System.Windows.Application.Current.Shutdown();
        }

        private void DeleteDir(object sender, RoutedEventArgs e)
        {
            Directory.Delete(GetSelectedPathAndRemoveFromTree(), true);
        }

   
        private void DeleteFile(object sender, RoutedEventArgs e)
        {
            File.Delete(GetSelectedPathAndRemoveFromTree());
        }



        private void OpenFile(object sender, RoutedEventArgs e)
        {
            TreeViewItem selected = tv.SelectedItem as TreeViewItem;
            if (selected != null)
            {
                Text.Text = File.ReadAllText(selected.Tag as string);
            }
                
        }
        private void Create(object sender, RoutedEventArgs e)
        {
            TreeViewItem selected = tv.SelectedItem as TreeViewItem;
            if(selected != null)
            {
                Prompt p = new Prompt();
                (string type, string path) = p.ShowDialog(selected);
                if (type == "dir")
                {
                    CreateDirNode(new DirectoryInfo(path), selected);
                }
                else if (type == "file")
                {
                    CreateFileNode(selected, new FileInfo(path));
                }
            }
            
        }

        private void showContents(DirectoryInfo dirinfo, TreeViewItem parent)
        {

            DirectoryInfo[] subdirs = dirinfo.GetDirectories();
            FileInfo[] files = dirinfo.GetFiles();
            TreeViewItem current = CreateDirNode(dirinfo, parent);


            foreach (DirectoryInfo subdir in subdirs)
            {
                showContents(subdir, current);
            }

            foreach (FileInfo file in files)
            {
                CreateFileNode(current, file);

            }

        }

        public void CreateFileNode(TreeViewItem current, FileInfo file)
        {
            TreeViewItem fileItem = new TreeViewItem { Header = file.Name, Tag = file.FullName };
            System.Windows.Controls.ContextMenu contextMenu;
            System.Windows.Controls.MenuItem opt1, opt2;
            contextMenu = new System.Windows.Controls.ContextMenu();
            opt1 = new System.Windows.Controls.MenuItem();
            opt1.Header = "Delete";
            opt1.Click += DeleteFile;
            opt2 = new System.Windows.Controls.MenuItem();
            opt2.Header = "Open";
            opt2.Click += OpenFile;
            contextMenu.Items.Add(opt1);
            contextMenu.Items.Add(opt2);
            fileItem.ContextMenu = contextMenu;
            fileItem.Selected += UpdateStatus;
            current.Items.Add(fileItem);
        }

        private TreeViewItem CreateDirNode(DirectoryInfo dirinfo, TreeViewItem parent)
        {
            TreeViewItem current = new TreeViewItem();
            System.Windows.Controls.ContextMenu contextMenu;
            System.Windows.Controls.MenuItem opt1, opt2;
            current = new TreeViewItem { Header = dirinfo.Name, Tag = dirinfo.FullName };
            current.Selected += UpdateStatus;

            contextMenu = new System.Windows.Controls.ContextMenu();
            opt1 = new System.Windows.Controls.MenuItem();
            opt1.Header = "Delete";
            opt1.Click += DeleteDir;
            opt2 = new System.Windows.Controls.MenuItem();
            opt2.Header = "Create";
            opt2.Click += Create;
            contextMenu.Items.Add(opt1);
            contextMenu.Items.Add(opt2);
            current.ContextMenu = contextMenu;
            parent.Items.Add(current);
            return current;
        }

        private string GetSelectedPathAndRemoveFromTree()
        {
            TreeViewItem selected = tv.SelectedItem as TreeViewItem;
            if (selected != null)
            {
                DirectoryInfo dirinfo = new DirectoryInfo(selected.Tag as string);
                if ((dirinfo.Attributes & FileAttributes.ReadOnly) == FileAttributes.ReadOnly)
                {
                    File.SetAttributes(selected.Tag as string, dirinfo.Attributes & ~FileAttributes.ReadOnly);
                }
            }
            if (selected.Parent is System.Windows.Controls.TreeView)
            {
                tv.Items.Remove(selected);
            }
            else
            {
                (selected.Parent as TreeViewItem).Items.Remove(selected);
            }
            
            return selected.Tag as string;
        }

        private void UpdateStatus(object sender, RoutedEventArgs e)
        {
            TreeViewItem selected = tv.SelectedItem as TreeViewItem;
            if (selected != null)
            {
                DirectoryInfo dirinfo = new DirectoryInfo(selected.Tag as string);
                Status.Text = dirinfo.getAttributes();
            }
                
        }
 
    }

    static class FSInfoExt
    {
        public static string getAttributes(this FileSystemInfo f)
        {
            string r = "-", a = "-", h = "-", s = "-";
            FileAttributes attributes = f.Attributes;
            if ((attributes & FileAttributes.ReadOnly) == FileAttributes.ReadOnly)
            {
                r = "r";
            }
            if ((attributes & FileAttributes.Archive) == FileAttributes.Archive)
            {
                a = "a";
            }
            if ((attributes & FileAttributes.Hidden) == FileAttributes.Hidden)
            {
                h = "h";
            }
            if ((attributes & FileAttributes.System) == FileAttributes.System)
            {
                s = "s";
            }

            return r + a + h + s;
        }
    }
}
