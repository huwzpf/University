using System;
using System.Collections.Generic;
using System.IO;
using System.Runtime.Serialization.Formatters.Binary;
using System.Runtime.Serialization;

namespace c_sharp_1
{
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

    static class DInfoExt
    {
       public static DateTime getOldest(this DirectoryInfo d)
        {
           
            DirectoryInfo[] subdirs = d.GetDirectories();
            FileInfo[] files = d.GetFiles();
            if (subdirs.Length + files.Length == 0)
            {
                return d.CreationTime;
            }
            else
            {
                DateTime oldest = DateTime.Now;
                DateTime t;

                foreach (DirectoryInfo subdir in subdirs)
                {
                    t = subdir.getOldest();
                    if (DateTime.Compare(t, oldest) < 0)
                    {
                        oldest = t;
                    }
                }
                foreach(FileInfo file in files)
                {
                    t = file.CreationTime;
                    if (DateTime.Compare(t, oldest) < 0)
                    {
                        oldest = t;
                    }
                }
                return oldest;
            }
           
        }
    }

    class Program
    {
        [Serializable]
        public class stringComparer : IComparer<string>
        {
            public int Compare(string a, string b)
            {
                int dif = a.Length - b.Length;
                if (dif != 0)
                {
                    return dif;
                }
                else
                {
                    return a.CompareTo(b);
                }
            }

        }
        static void showContents(DirectoryInfo dirinfo, int level = 0)
        {
            string spacing = "";
            for (int i = 0; i < level; i++)
            {
                spacing += "\t";
            }
            DirectoryInfo[] subdirs = dirinfo.GetDirectories();
            FileInfo[] files = dirinfo.GetFiles();

            Console.WriteLine("{0} ({1}) {2}", spacing + dirinfo.Name, subdirs.Length + files.Length, dirinfo.getAttributes());
        
           
            foreach (DirectoryInfo subdir in subdirs)
            {
                showContents(subdir, level + 1);
            }
            spacing += "\t";
            
            foreach (FileInfo file in files)
            {
                Console.WriteLine("{0} {1} bajtów {2}",spacing + file.Name, file.Length, file.getAttributes());
            }
            

        }


        public static long getDirLen(DirectoryInfo dir)
        {
            return dir.GetDirectories().Length + dir.GetFiles().Length;
        }

        public static void SerializeList(SortedList<string, long> list)
        {
            FileStream fs = new FileStream("data.dat", FileMode.Create);

            // Construct a BinaryFormatter and use it to serialize the data to the stream.
            BinaryFormatter formatter = new BinaryFormatter();
            try
            {
                formatter.Serialize(fs, list);
            }
            catch 
            {
                Console.WriteLine("error");
            }
            finally
            {
                fs.Close();
            }
        }

        public static SortedList<string, long> DeserializeList()
        {
            SortedList<string, long> list = null;
            FileStream fs = new FileStream("data.dat", FileMode.Open);
            try
            {
                BinaryFormatter formatter = new BinaryFormatter();

                list = (SortedList<string, long>)formatter.Deserialize(fs);
            }
            catch
            {
                Console.WriteLine("error ");
            }
            finally
            {
                fs.Close();
            }
            return list;
        }

        static void Main(string[] args)
        {
            if (args.Length < 1)
            {
                Console.WriteLine("error");
            }
            else
            {
                string dirname = args[0];
                try
                {
                    DirectoryInfo dirinfo = new DirectoryInfo(dirname);
                    showContents(dirinfo);
                    Console.WriteLine("Najstarszy plik: {0}", dirinfo.getOldest());

                    SortedList<string, long> list = new SortedList<string, long>(new stringComparer());
                    DirectoryInfo[] subdirs = dirinfo.GetDirectories();
                    FileInfo[] files = dirinfo.GetFiles();

                    foreach (DirectoryInfo subdir in subdirs)
                    {
                        list.Add(subdir.Name, getDirLen(subdir));
                    }
                    foreach (FileInfo file in files)
                    {
                        list.Add(file.Name, file.Length);
                    }
                    SerializeList(list);
                    SortedList<string, long> deserializedList = DeserializeList();
                    if (deserializedList == null)
                    {
                        Console.WriteLine("error");
                    }
                    else
                    {
                        foreach (KeyValuePair<string, long> pair in deserializedList)
                        {
                            Console.WriteLine("{0} -> {1}", pair.Key, pair.Value);
                        }
                    }
                   
                }
                catch
                {
                    Console.WriteLine("error");
                }

            }
            
            Console.ReadLine();
        }
    }
}
