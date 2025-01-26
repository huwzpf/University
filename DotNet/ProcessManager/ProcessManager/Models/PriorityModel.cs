using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Diagnostics;

namespace ProcessManager.Models
{
    public class PriorityModel
    {
        public ProcessPriorityClass Priority { get; set; }
        public string Name { get; set; }

        public PriorityModel(ProcessPriorityClass priority, string name)
        {
            Priority = priority;
            Name = name;
        }
    }
}
