using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Diagnostics;
using System.Collections.ObjectModel;

namespace ProcessManager.Models
{
    public class ProcessModel
    {
        private Process _process;
        public bool initialized { get; set; }
        public string Name { get; set; }
        public int Id { get; set; }
        public long Memory { get; set; }
        public int ThreadCount { get; set; }

        public ProcessModel(Process process)
        {
            initialized = true;
            Name = process.ProcessName;
            Id = process.Id;
            Memory = process.PrivateMemorySize64;
            ThreadCount = process.Threads.Count;
            _process = process;
        }
        public ObservableCollection<ThreadModel> Threads
        {
            get
            {
                ObservableCollection<ThreadModel> threads = new ObservableCollection<ThreadModel>();
                foreach (ProcessThread thread in _process.Threads)
                {
                    threads.Add(new ThreadModel { ThreadId = thread.Id });
                }

                return threads;
            }
        }

        public string CpuUsage
        {
            get
            {
                try
                {
                    return (GetCpuUsage() * 100).ToString();
                }
                catch (Exception e)
                {
                    return "Unknown";
                }
            }

        }

        public string Priority
        {
            get
            {
                try
                {
                    return _process.PriorityClass.ToString();
                }
                catch (Exception e)
                {
                    return "Unknown";
                }
            }
        }

        public string StartTime
        {
            get
            {
                try
                {
                    return _process.StartTime.ToString();
                }
                catch (Exception e)
                {
                    return DateTime.MinValue.ToString();
                }
            }
        }   

        public double GetCpuUsage()
        {
            return _process.TotalProcessorTime / (DateTime.Now - _process.StartTime);
        }

        public void Kill()
        {
            _process.Kill();
        }

        public void SetPriority(ProcessPriorityClass priority)
        {
            _process.PriorityClass = priority;
        }
    }
}
