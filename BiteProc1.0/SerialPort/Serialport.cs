using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;

namespace SerialPort
{
    public class Serialport
    {
        public Serialport() {

            try      //for test open the port
            {
                SP.PortName = SPData.name;
                SP.BaudRate = SPData.Baud;
                SP.DataBits = SPData.Bits;
                SP.Parity = SPData.parity;
                SP.StopBits = SPData.stopBits;
                SP.Open();
                SP.Close();
            }
            catch
            {
                Console.WriteLine("Open the port failed\n");
            }
        }
        public void Start()
        {
            ForBackSpeed = LeftRightSpeed = 0;
            th = new Thread(new ThreadStart(PowerOn));
            th.IsBackground = true;
            isRun = true;
            th.Start();
        }
        public void POff()
        {
            isRun = false;
            ForBackSpeed = LeftRightSpeed = 0;
        }

        public bool IsRun()
        {
            return isRun;
        }

        /// <summary>
        /// 前进后退 speed
        /// </summary>
        public void Run(int speed)
        {
            int vv = speed;
            if (vv > 100) vv = 100;
            if (vv < -100) vv = -100;
            ForBackSpeed = (byte)vv;
        }

        /// <summary>
        /// 左右转 speed
        /// </summary>
        public void LeftRight(int speed)
        {
            int vv = speed;
            if (vv > 60) vv = 60;
            if (vv < -60) vv = -60;
            LeftRightSpeed = (byte)vv;
        }
        public void SetContrmode(int m)
        {
            CtrlMode = m;
        }

        
        //private 

        private void PowerOn()
        {
            HiPerfTimer ht = new HiPerfTimer();
            double tt;
            double xx;
            InitPort();
            if (SP.IsOpen) SP.Close();
            SP.Open();

            RevStatus = true;
            int num = 5;
            for (int i = 0; i < num; i++)
            {
                  Boolean fb = false;
                  DataForceZero = fb;
                  RevStatus = !fb;
                  ht.Start();
                  tt = SPData.wave[i].time;
                  string ss = "Output ";
                  ss += (SPData.wave[i].VS == 0) ? "Low" : "High";
                  ss += " Time:" + SPData.wave[i].time.ToString() + "mS";
                  xx = ht.DurationInMs;
                  while (xx < tt) { xx = ht.DurationInMs; }
            }

            DataForceZero = false;
            RevStatus = false;
            wrBuf[0] = 0x53;
            wrBuf[1] = 0xAC;
            wrBuf[2] = 0x00;
            ht.Start();
            int dNum = Send(wrBuf, 2, rdBuf, 4);
            while (ht.DurationInMs < SPData.poSegTime) ;

            for (int i = 0; i < 5; i++)
            {
                wrBuf[0] = 0x4a;
                wrBuf[1] = 0xC0;
                wrBuf[2] = 0x00;
                wrBuf[3] = 0x00;
                wrBuf[4] = 0x00;
                wrBuf[5] = GetJYZ(wrBuf, 5);
                ht.Start();
                dNum = Send(wrBuf, 6, rdBuf, 7);
                while (ht.DurationInMs < SPData.segTime) ;
            }

            while (isRun)
            {
                wrBuf[0] = 0x4a;
                byte kk = KeyStatus;
                wrBuf[1] = kk;
                wrBuf[2] = 0x00;
                if (kk != 0x00)
                {
                    wrBuf[3] = 0x00;
                    wrBuf[4] = 0x00;
                }
                else
                {
                    wrBuf[3] = ForBackSpeed;
                    wrBuf[4] = LeftRightSpeed;
                }

                wrBuf[5] = GetJYZ(wrBuf, 5);
                ht.Start();
                dNum = Send(wrBuf, 6, rdBuf, 7);
                GetStatus(rdBuf, 6, dNum);
                while (ht.DurationInMs < SPData.segTime) ;
            }
            SP.Close();
        }
        private void InitPort()
        {
            Boolean op = SP.IsOpen;
            if (op) SP.Close();
            SP.PortName = SPData.name;
            SP.BaudRate = SPData.Baud;
            SP.DataBits = SPData.Bits;
            SP.Parity = SPData.parity;
            SP.StopBits = SPData.stopBits;
            if (op)
            {
                try
                {
                    SP.Open();
                }
                catch (System.IO.IOException e)
                {
                    Console.WriteLine(e.Message);
                }
            }
            if (SP.IsOpen) SP.Close();
            for (int i = 0; i < wrBuf.Length; i++) wrBuf[i] = 0x00;

        }
        private int Send(byte[] dat, int len, byte[] rev, int revlen)
        {
            HiPerfTimer ht = new HiPerfTimer();
            double sdTime = 0.3125 * len - 0.255;
            ht.Start();
            SP.Write(wrBuf, 0, len);
            while (ht.DurationInMs < sdTime) ;

            RevStatus = true;

            ht.Start();
            if (revlen > 0)
            {
                Boolean flag = true;
                while (SP.BytesToRead < revlen && flag)
                {
                    if (ht.DurationInMs > SPData.timeOut) flag = false;
                }
                int num = SP.BytesToRead;
                int m = SP.Read(rev, 0, num);
                RevStatus = false;
                return m;
            }
            else
            {
                RevStatus = false;
                return 0;
            }
        }

        private void GetStatus(byte[] buf, int start, int len)
        {
            byte ss = GetJYZ(buf, start, len - start);
            if (ss == 0x02)
            {
                if (buf[start] == 0xfe && buf[start + 1] == 0x54)
                {
                    if ((buf[start + 2] & 0x40) == 0x40) ActCtrlMode = 2;
                    else if ((buf[start + 2] & 0x20) == 0x20) ActCtrlMode = 1;
                    else ActCtrlMode = 0;
                    MaxSpeed = buf[start + 5];
                }
            }
        }
        /// <summary>
        /// Send or Receive --- true:Receive;False:Send
        /// </summary>
        private Boolean RevStatus
        {
            set
            {
                if (!SP.IsOpen) SP.Open();
                SP.RtsEnable = !value;
            }
        }
        /// <summary>
        /// Force output low level,true:Force
        /// </summary>
        private Boolean DataForceZero
        {
            set
            {
                if (!SP.IsOpen) SP.Open();
                SP.DtrEnable = !value;
            }
        }
        private byte KeyStatus
        {

            get
            {
                byte xx = 0x0;
                // 0,前进后退;1,座椅仰躺;2,靠背仰躺
                if (ctrlMode != ActCtrlMode)
                {
                    if (ActCtrlMode == 0)
                    {
                        if (ctrlMode == 1) xx |= 0x08;
                        else if (ctrlMode == 2) xx |= 0x10;
                    }
                    else if (ActCtrlMode == 1) xx |= 0x08;
                    else if (ActCtrlMode == 2) xx |= 0x10;
                }
                //if (bZYYT) xx |= 0x08;
                //if (bKBYT) xx |= 0x10;
                if (bSDS) xx |= 0x04;
                if (bSDJ) xx |= 0x02;
                return xx;
            }
        }
        /// <summary>
        /// 获取最大前进速度
        /// </summary>
        private int MaxRunSpeed
        {
            get
            {
                return MaxSpeed;
            }

        }
        private byte GetJYZ(byte[] dat, int num)
        {
            return GetJYZ(dat, 0, num);
        }
        private byte GetJYZ(byte[] dat, int start, int num)
        {
            int sum = 0;
            for (int i = start; i < start + num; i++) sum += dat[i];
            sum = ~sum;
            return (byte)sum;
        }
        /// <summary>
        /// 设置/获取控制模式: 0,轮椅行走;1,座椅;2,靠背/脚架;
        /// </summary>
        private int CtrlMode
        {
            get
            {
                return ActCtrlMode;
            }
            set
            {
                ctrlMode = value;
            }
        }
        private byte str2Byte(string ss)
        {
            int xx;
            int.TryParse(ss, out xx);
            return (byte)xx;

        }
        private string Byte2Str(byte dat)
        {
            string ss = dat.ToString("X");
            if (ss.Length == 1) ss = "0" + ss;
            return ss;
        }

        private System.Threading.Thread th;
        private Boolean isRun = false;
        private byte[] wrBuf = new byte[1024];
        private byte[] rdBuf = new byte[1024];
        private System.IO.Ports.SerialPort SP = new System.IO.Ports.SerialPort() ;
        private Para SPData = new Para();
        private Boolean bSDS = false;
        private Boolean bSDJ = false;
        private byte ForBackSpeed = 0;
        private byte LeftRightSpeed = 0;

        private int MaxSpeed = 0;
        private int ctrlMode = 0;
        private int ActCtrlMode = 0;
    }
}
