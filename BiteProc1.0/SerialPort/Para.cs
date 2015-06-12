using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SerialPort
{
    internal class WaveSeg
    {
        /// <summary>
        /// Voltage level,0-Low,1-High
        /// </summary>
        public int VS;

        /// <summary>
        /// last Time
        /// </summary>
        public double time;
    }

    internal class Para
    {
       public string name = "COM14";
       public System.IO.Ports.Parity parity = System.IO.Ports.Parity.Even;
       public System.IO.Ports.StopBits stopBits = System.IO.Ports.StopBits.Two;
       public int Baud = 38400;
       public int Bits = 8;
       public WaveSeg[] wave;
       public double segTime = 10;
       public double poSegTime = 10;
       public double timeOut = 5;

       public static int Str2Int(string ss)
       {
           int m = 0;
           int.TryParse(ss, out m);
           return m;
       }

       public static double Str2Double(string ss)
       {
           double  m = 0;
           double.TryParse(ss, out m);
           return  m;
       }

       public string BaudStr
       {
           get { return Baud.ToString(); }
           set { Baud = Str2Int(value); }
       }

       public string BitsStr
       {
           get { return Bits.ToString(); }
           set { Bits = Str2Int(value); }
       }

       public string parityStr
       {
           get { return parity.ToString(); }
           set { parity = (System.IO.Ports.Parity)Enum.Parse(typeof(System.IO.Ports.Parity), value);}
       }

       public string stopBitsStr
       {
           get { return stopBits.ToString(); }
           set { stopBits = (System.IO.Ports.StopBits)Enum.Parse(typeof(System.IO.Ports.StopBits), value); }
       }

       internal string waveStr
       {
           get
           {
               int n = wave.Length;
               string ss = "";
               for (int i = 0; i < n; i++)
               {
                   string s2 = wave[i].VS.ToString() + ":" + wave[i].time.ToString();
                   if (i > 0) ss += "-";
                   ss += s2;
               }
               return ss;
           }
           set
           {
               string ss = value;
               string[] s1 = ss.Split(new char[] { '-' });
               int n = s1.Length;
               wave = new WaveSeg[n];
               for (int i = 0; i < n; i++)
               {
                   string[] sg = s1[i].Split(new char[] { ':' });
                   if (sg.Length > 1)
                   {
                       wave[i] = new WaveSeg();
                       wave[i].VS = (sg[0] == "0" ? 0 : 1);
                       wave[i].time = Str2Double(sg[1]);
                   }

               }
           }
       }

       public string segTimeStr
       {
           get { return segTime .ToString(); }
           set { segTime = Str2Double(value); }
       }

       public string poSegTimeStr
       {
           get { return poSegTime.ToString(); }
           set { poSegTime = Str2Double(value); }
       }

       public string timeOutStr
       {
           get { return timeOut.ToString(); }
           set { timeOut = Str2Double(value); }
       }
   }
}
