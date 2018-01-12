using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Data;

namespace WFNetLib
{
    public abstract class FileOP
    {
        public static byte[] LoadFile(string fileName)
        {
            FileInfo f = new FileInfo(fileName);
            Stream stream = File.OpenRead(f.FullName);
            byte[] pBuf = new byte[stream.Length];
            stream.Read(pBuf, 0, (int)stream.Length);
            stream.Close();
            return pBuf;
        }
        public static void SaveFile(string fileName, byte[] pBuf, int offset, int count)
        {
            FileInfo f = new FileInfo(fileName);
            Stream stream = File.Create(f.FullName);
            stream.Write(pBuf, offset, count);
            stream.Flush();
            stream.Close();
        }
    # region "文件的读写操作"

        /// <summary>
        /// 以文件流的形式读取指定文件的内容
        /// </summary>
        /// <param name="file">指定的文件及其全路径</param>
        /// <returns>返回 String</returns>
        public static string ReadFile(string file)
        {
            string strResult = "";

            FileStream fStream = new FileStream(file, FileMode.Open, FileAccess.Read);
            StreamReader sReader = new StreamReader(fStream, Encoding.Default);
            try
            {
                strResult = sReader.ReadToEnd();
            }
            catch { }
            finally
            {
                fStream.Flush();
                fStream.Close();
                sReader.Close();
            }
            return strResult;
        }
        /// <summary>
        /// 以文件流的形式将内容写入到指定文件中（如果该文件或文件夹不存在则创建）
        /// </summary>
        /// <param name="file">文件名和指定路径</param>
        /// <param name="fileContent">文件内容</param>
        /// <returns>返回布尔值</returns>
        public static void WriteFile(string file, byte[] fileContent)
        {
            FileInfo f = new FileInfo(file);
            // 如果文件所在的文件夹不存在则创建文件夹
            if (!Directory.Exists(f.DirectoryName))
                Directory.CreateDirectory(f.DirectoryName);
            FileStream fStream = new FileStream(file, FileMode.Create, FileAccess.Write);            
            try
            {
                fStream.Write(fileContent, 0, fileContent.Length);
            }
            catch (Exception exc)
            {
                throw new Exception(exc.ToString());
            }
            finally
            {
                fStream.Flush();
                fStream.Close();
            }
        }
        /// <summary>
        /// 以文件流的形式将内容写入到指定文件中（如果该文件或文件夹不存在则创建）
        /// </summary>
        /// <param name="file">文件名和指定路径</param>
        /// <param name="fileContent">文件内容</param>
        /// <returns>返回布尔值</returns>
        public static void WriteFile(string file, string fileContent)
        {
            WriteFile(file, fileContent, true);
        }
        /// <summary>
        /// 以文件流的形式将内容写入到指定文件中（如果该文件或文件夹不存在则创建）
        /// </summary>
        /// <param name="file">文件名和指定路径</param>
        /// <param name="fileContent">文件内容</param>
        /// <param name="Append">是否追加指定内容到该文件中</param>
        /// <returns>返回布尔值</returns>
        public static void WriteFile(string file, string fileContent, bool Append)
        {
            FileInfo f = new FileInfo(file);
            // 如果文件所在的文件夹不存在则创建文件夹
            if (!Directory.Exists(f.DirectoryName)) 
                Directory.CreateDirectory(f.DirectoryName);
            StreamWriter sWriter = new StreamWriter(file, Append);
            try
            {
                sWriter.Write(fileContent);
            }
            catch (Exception exc)
            {
                throw new Exception(exc.ToString());
            }
            finally
            {
                sWriter.Flush();
                sWriter.Close();
            }
        }
    # endregion

    # region "文件夹信息的读取"

        /// <summary>
        /// 获取指定目录下的所有目录及其文件信息
        /// </summary>
        /// <param name="dir">文件夹</param>
        /// <param name="method">获取方式。</param>
        /// <returns>DataTable</returns>
        public static DataTable getDirectoryAllInfos(string dir, FileOPMethod method)
        {
            DataTable Dt;
            try
            {
                DirectoryInfo dInfo = new DirectoryInfo(dir);
                Dt = getDirectoryAllInfo(dInfo, method);
            }
            catch (Exception exc)
            {
                throw new Exception(exc.ToString());
            }
            return Dt;
        }
        /// <summary>
        /// 获取指定目录下的所有目录及其文件信息
        /// </summary>
        /// <param name="d">实例化的目录</param>
        /// <param name="method">获取方式。1、仅获取文件夹结构  2、仅获取文件结构  3、同时获取文件和文件夹信息</param>
        /// <returns></returns>
        private static DataTable getDirectoryAllInfo(DirectoryInfo d, FileOPMethod method)
        {
            DataTable Dt = new DataTable();
            DataRow Dr;
            Dt.Columns.Add("name");			//名称
            Dt.Columns.Add("rname");		//名称
            Dt.Columns.Add("content_type");	//文件MIME类型，如果是文件夹则置空
            Dt.Columns.Add("type");			//类型：1为文件夹，2为文件
            Dt.Columns.Add("path");			//文件路径
            Dt.Columns.Add("creatime");		//创建时间
            Dt.Columns.Add("size");			//文件大小
            // 获取文件夹结构信息
            DirectoryInfo[] dirs = d.GetDirectories();
            foreach (DirectoryInfo dir in dirs)
            {
                if (method == FileOPMethod.File)
                {
                    Dt = DataTableOP.copyDT(Dt, getDirectoryAllInfo(dir, method));
                }
                else
                {
                    Dr = Dt.NewRow();
                    Dr[0] = dir.Name;//名称
                    Dr[1] = dir.FullName;//名称
                    Dr[2] = "";//文件MIME类型，如果是文件夹则置空
                    Dr[3] = 1;//类型：1为文件夹，2为文件
                    Dr[4] = dir.FullName.Replace(dir.Name, "");//文件路径
                    Dr[5] = dir.CreationTime;//创建时间
                    Dr[6] = "";//文件大小
                    Dt.Rows.Add(Dr);
                    Dt = DataTableOP.copyDT(Dt, getDirectoryAllInfo(dir, method));
                }
            }
            // 获取文件结构信息
            if (method != FileOPMethod.Folder)
            {
                FileInfo[] files = d.GetFiles();
                foreach (FileInfo file in files)
                {
                    Dr = Dt.NewRow();
                    Dr[0] = file.Name;
                    Dr[1] = file.FullName;
                    Dr[2] = file.Extension.Replace(".", "");
                    Dr[3] = 2;
                    Dr[4] = file.DirectoryName + "\\";
                    Dr[5] = file.CreationTime;
                    Dr[6] = file.Length;

                    Dt.Rows.Add(Dr);
                }
            }
            return Dt;
        }

        
        /// <summary>
        /// 获取指定文件夹的信息，如：文件夹大小，文件夹数，文件数
        /// </summary>
        /// <param name="dir">指定文件夹路径</param>
        /// <returns>返回 String</returns>
        public static long[] getDirInfos(string dir)
        {
            long[] intResult = new long[3];
            DirectoryInfo d = new DirectoryInfo(dir);
            intResult = DirInfo(d);
            return intResult;
        }


        private static long[] DirInfo(DirectoryInfo d)
        {
            long[] intResult = new long[3];
            long Size = 0;
            long Dirs = 0;
            long Files = 0;
            // 计算文件大小
            FileInfo[] files = d.GetFiles();
            Files += files.Length;
            foreach (FileInfo file in files)
            {
                Size += file.Length;
            }
            // 计算文件夹
            DirectoryInfo[] dirs = d.GetDirectories();
            Dirs += dirs.Length;
            foreach (DirectoryInfo dir in dirs)
            {
                Size += DirInfo(dir)[0];
                Dirs += DirInfo(dir)[1];
                Files += DirInfo(dir)[2];
            }

            intResult[0] = Size;
            intResult[1] = Dirs;
            intResult[2] = Files;
            return intResult;
        }


        /// <summary>
        /// 获取指定目录的目录信息
        /// </summary>
        /// <param name="dir">指定目录</param>
        /// <param name="method">获取方式。1、仅获取文件夹结构  2、仅获取文件结构  3、同时获取文件和文件夹信息</param>
        /// <returns>返回 DataTable</returns>
        public static DataTable getDirectoryInfos(string dir, FileOPMethod method)
        {
            DataTable Dt = new DataTable();
            DataRow Dr;

            Dt.Columns.Add("name");//名称
            Dt.Columns.Add("type");//类型：1为文件夹，2为文件
            Dt.Columns.Add("size");//文件大小，如果是文件夹则置空
            Dt.Columns.Add("content_type");//文件MIME类型，如果是文件夹则置空
            Dt.Columns.Add("createTime");//创建时间
            Dt.Columns.Add("lastWriteTime");//最后修改时间
            Dt.Columns.Add("filename");//文件名
            // 获取文件夹结构信息
            if (method != FileOPMethod.File)
            {
                for (int i = 0; i < getDirs(dir).Length; i++)
                {
                    Dr = Dt.NewRow();
                    DirectoryInfo d = new DirectoryInfo(getDirs(dir)[i]);

                    Dr[0] = d.Name;
                    Dr[1] = 1;
                    Dr[2] = "";
                    Dr[3] = "";
                    Dr[4] = d.CreationTime;
                    Dr[5] = d.LastWriteTime;

                    Dt.Rows.Add(Dr);
                }
            }

            // 获取文件结构信息
            if (method != FileOPMethod.Folder)
            {
                for (int i = 0; i < getFiles(dir).Length; i++)
                {
                    Dr = Dt.NewRow();
                    FileInfo f = new FileInfo(getFiles(dir)[i]);

                    Dr[0] = f.Name;
                    Dr[1] = 2;
                    Dr[2] = f.Length;
                    Dr[3] = f.Extension.Replace(".", "");
                    Dr[4] = f.CreationTime;
                    Dr[5] = f.LastWriteTime;
                    Dr[6] = System.IO.Path.GetFileNameWithoutExtension(f.Name);
                    Dt.Rows.Add(Dr);
                }
            }

            return Dt;
        }


        public static string[] getDirs(string dir)
        {
            return Directory.GetDirectories(dir);
        }

        public static string[] getFiles(string dir)
        {
            return Directory.GetFiles(dir);
        }

    # endregion

    # region "文件系统的相应操作"
        /// <summary>
        /// 判断文件或文件夹是否存在
        /// </summary>
        /// <param name="file">指定文件及其路径</param>
        /// <param name="method">判断方式</param>
        /// <returns>返回布尔值</returns>
        public static bool IsExist(string file, FileOPMethod method)
        {
            try
            {
                if (method == FileOPMethod.File)
                {
                    return File.Exists(file);
                }
                else if (method == FileOPMethod.Folder)
                {
                    return Directory.Exists(file);
                }
                else
                {
                    return false;
                }
            }
            catch (Exception exc)
            {
                throw new Exception(exc.ToString());
            }
        }

    # region "新建"

        /// <summary>
        /// 新建文件或文件夹
        /// </summary>
        /// <param name="file">文件或文件夹及其路径</param>
        /// <param name="method">新建方式</param>
        public static void Create(string file, FileOPMethod method)
        {
            try
            {
                if (method == FileOPMethod.File)
                {
                    WriteFile(file, "");
                }
                else if (method == FileOPMethod.Folder)
                {
                    Directory.CreateDirectory(file);
                }
            }
            catch (Exception exc)
            {
                throw new Exception(exc.ToString());
            }
        }

    # endregion


    # region "复制"

    # region "复制文件"
        /// <summary>
        /// 复制文件，如果目标文件已经存在则覆盖掉
        /// </summary>
        /// <param name="oldFile">源文件</param>
        /// <param name="newFile">目标文件</param>
        public static void CopyFile(string oldFile, string newFile)
        {
            try
            {
                File.Copy(oldFile, newFile, true);
            }
            catch (Exception exc)
            {
                throw new Exception(exc.ToString());
            }
        }

        /// <summary>
        /// 以流的形式复制拷贝文件
        /// </summary>
        /// <param name="oldPath">源文件</param>
        /// <param name="newPath">目标文件</param>
        /// <returns></returns>
        public static bool CopyFileStream(string oldPath, string newPath)
        {
            try
            {
                //建立两个FileStream对象
                FileStream fsOld = new FileStream(oldPath, FileMode.Open, FileAccess.Read);
                FileStream fsNew = new FileStream(newPath, FileMode.Create, FileAccess.Write);

                //分别建立一个读写类
                BinaryReader br = new BinaryReader(fsOld);
                BinaryWriter bw = new BinaryWriter(fsNew);

                //将读取文件流的指针指向流的头部
                br.BaseStream.Seek(0, SeekOrigin.Begin);
                //将写入文件流的指针指向流的尾部
                bw.BaseStream.Seek(0, SeekOrigin.End);

                while (br.BaseStream.Position < br.BaseStream.Length)
                {
                    //从br流中读取一个Byte并马上写入bw流
                    bw.Write(br.ReadByte());
                }
                //释放所有被占用的资源
                br.Close();
                bw.Close();
                fsOld.Flush();
                fsOld.Close();
                fsNew.Flush();
                fsNew.Close();
                return true;
            }
            catch
            {
                return false;
            }

        }

    # endregion

    # region "复制文件夹"

        /// <summary>
        /// 复制文件夹中的所有内容及其子目录所有文件
        /// </summary>
        /// <param name="oldDir">源文件夹及其路径</param>
        /// <param name="newDir">目标文件夹及其路径</param>
        public static void CopyDirectory(string oldDir, string newDir)
        {
            try
            {
                DirectoryInfo dInfo = new DirectoryInfo(oldDir);
                CopyDirInfo(dInfo, oldDir, newDir);
            }
            catch (Exception exc)
            {
                throw new Exception(exc.ToString());
            }
        }

        private static void CopyDirInfo(DirectoryInfo od, string oldDir, string newDir)
        {
            // 寻找文件夹
            if (!IsExist(newDir, FileOPMethod.Folder)) 
                Create(newDir, FileOPMethod.Folder);
            DirectoryInfo[] dirs = od.GetDirectories();
            foreach (DirectoryInfo dir in dirs)
            {
                CopyDirInfo(dir, dir.FullName, newDir + dir.FullName.Replace(oldDir, ""));
            }
            // 寻找文件
            FileInfo[] files = od.GetFiles();
            foreach (FileInfo file in files)
            {
                CopyFile(file.FullName, newDir + file.FullName.Replace(oldDir, ""));
            }
        }

    # endregion

    # endregion


    # region "移动"

        /// <summary>
        /// 移动文件或文件夹
        /// </summary>
        /// <param name="oldFile">原始文件或文件夹</param>
        /// <param name="newFile">目标文件或文件夹</param>
        /// <param name="method">移动方式：1、为移动文件，2、为移动文件夹</param>
        public static void Move(string oldFile, string newFile, FileOPMethod method)
        {
            try
            {
                if (method == FileOPMethod.File)
                {
                    File.Move(oldFile, newFile);
                }
                if (method == FileOPMethod.Folder)
                {
                    Directory.Move(oldFile, newFile);
                }
            }
            catch (Exception exc)
            {
                throw new Exception(exc.ToString());
            }
        }

    # endregion


    # region "删除"

        /// <summary>
        /// 删除文件或文件夹
        /// </summary>
        /// <param name="file">文件或文件夹及其路径</param>
        /// <param name="method">删除方式：1、为删除文件，2、为删除文件夹</param>
        public static void Delete(string file, FileOPMethod method)
        {
            try
            {
                if (method == FileOPMethod.File)
                {
                    File.Delete(file);
                }
                if (method == FileOPMethod.Folder)
                {
                    Directory.Delete(file, true);//删除该目录下的所有文件以及子目录
                }
            }
            catch (Exception exc)
            {
                throw new Exception(exc.ToString());
            }
        }

    # endregion

    # endregion
    }
    /// <summary>
    /// 文件系统的处理对象
    /// </summary>
    public enum FileOPMethod
    {
        /// <summary>
        /// 仅用于处理文件夹
        /// </summary>
        Folder = 0,
        /// <summary>
        /// 仅用于处理文件
        /// </summary>
        File,
        /// <summary>
        /// 文件和文件夹都参与处理
        /// </summary>
        All
    }
}
