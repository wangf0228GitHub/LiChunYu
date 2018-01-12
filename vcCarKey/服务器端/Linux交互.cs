using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Net;
using System.Text;
using System.IO;

namespace 服务器端
{
    public class Linux_101
    {
        public static readonly string DefaultUserAgent = "Mozilla/4.0 (compatible; MSIE 8.0; Windows NT 6.1; Trident/4.0; SLCC2; .NET CLR 2.0.50727; .NET CLR 3.5.30729; .NET CLR 3.0.30729; Media Center PC 6.0; .NET4.0C; .NET4.0E; doyo 2.0.3)";
                
        public CookieCollection Cookies;
        public IDictionary<string, string> HeaderParameters;
        string host;
        public string Content;
        public Linux_101()
        {
            Cookies = new CookieCollection();
            HeaderParameters = new Dictionary<string, string>();
            host = "hdts.dyndns-web.com";
            HeaderParameters["Connection"] = "keep-alive";
            HeaderParameters["Host"] = host;
            HeaderParameters["Accept"] = "text/html, */*";
        }        
        /// <summary>  
        /// 创建POST方式的HTTP请求  
        /// </summary>  
        /// <param name="url">请求的URL</param>  
        /// <param name="parameters">随同请求POST的参数名称及参数值字典</param>  
        /// <param name="timeout">请求的超时时间</param>  
        /// <param name="userAgent">请求的客户端浏览器信息，可以为空</param>  
        /// <param name="requestEncoding">发送HTTP请求时所用的编码</param>  
        /// <param name="cookies">随同HTTP请求发送的Cookie信息，如果不需要身份验证可以为空</param>  
        /// <returns></returns>  
        public HttpWebResponse CreatePostHttpResponse(string url, string content)
        {
            return CreatePostHttpResponse(url, content, null);
        }
        public HttpWebResponse CreatePostHttpResponse(string url, string content, CookieCollection newCookies)
        {
            if (string.IsNullOrEmpty(url))
            {
                throw new ArgumentNullException("url");
            }
            HttpWebRequest request = null;
            request = WebRequest.Create(url) as HttpWebRequest;
            request.Method = "POST";
            request.UserAgent = DefaultUserAgent;
            //             if (!(HeaderParameters == null || HeaderParameters.Count == 0))
            //             {
            //                 foreach (string key in HeaderParameters.Keys)
            //                 {
            //                     request.Headers[key] = HeaderParameters[key];
            //                 }
            //             }
            //             if (timeout.HasValue)
            //             {
            //                 request.Timeout = timeout.Value;
            //             }
            if (Cookies != null && Cookies.Count != 0)
            {
                foreach (Cookie c in Cookies)
                {
                    request.CookieContainer.Add(c);
                }
            }
            if (newCookies != null && newCookies.Count != 0)
            {
                foreach (Cookie c in newCookies)
                {
                    request.CookieContainer.Add(c);
                    Cookies.Add(c);
                }
                //request.CookieContainer.Add(cookies);  
            }
            //如果需要POST数据  
            if (content != null)
            {
                //StringBuilder buffer = new StringBuilder();
                int i = 0;
                byte[] data = Encoding.ASCII.GetBytes(content.ToString());
                using (Stream stream = request.GetRequestStream())
                {
                    stream.Write(data, 0, data.Length);
                }
            }
            HttpWebResponse Response = request.GetResponse() as HttpWebResponse;
            if (Response.Cookies.Count != 0)
            {
                foreach (Cookie c in Response.Cookies)
                {
                    Cookies.Add(c);
                }
            }

            if (Response.Headers["ContentEncoding"] != null)
            {
                Stream receiveStream = Response.GetResponseStream();
                StreamReader sr = new StreamReader(receiveStream, Encoding.GetEncoding(Response.Headers["ContentEncoding"].ToString()));
                Content = sr.ReadToEnd();
            }
            else
            {
                try
                {
                    Stream receiveStream = Response.GetResponseStream();
                    StreamReader sr = new StreamReader(receiveStream);
                    Content = sr.ReadToEnd();
                }
                catch
                {

                }
            }
            return Response;
        }
        public HttpWebResponse HPassUpload(string url, string content, CookieCollection newCookies)
        {
            if (string.IsNullOrEmpty(url))
            {
                throw new ArgumentNullException("url");
            }
            HttpWebRequest request = null;
            request = WebRequest.Create(url) as HttpWebRequest;
            request.Method = "POST";
            request.UserAgent = DefaultUserAgent;
            //             if (!(HeaderParameters == null || HeaderParameters.Count == 0))
            //             {
            //                 foreach (string key in HeaderParameters.Keys)
            //                 {
            //                     request.Headers[key] = HeaderParameters[key];
            //                 }
            //             }
            //             if (timeout.HasValue)
            //             {
            //                 request.Timeout = timeout.Value;
            //             }
            if (Cookies != null && Cookies.Count != 0)
            {
                foreach (Cookie c in Cookies)
                {
                    request.CookieContainer.Add(c);
                }
            }
            if (newCookies != null && newCookies.Count != 0)
            {
                foreach (Cookie c in newCookies)
                {
                    request.CookieContainer.Add(c);
                    Cookies.Add(c);
                }
                //request.CookieContainer.Add(cookies);  
            }
            //如果需要POST数据  
            if (content != null)
            {
                //StringBuilder buffer = new StringBuilder();
                int i = 0;
                byte[] data = Encoding.ASCII.GetBytes(content.ToString());
                using (Stream stream = request.GetRequestStream())
                {
                    stream.Write(data, 0, data.Length);
                }
            }
            HttpWebResponse Response = request.GetResponse() as HttpWebResponse;
            if (Response.Cookies.Count != 0)
            {
                foreach (Cookie c in Response.Cookies)
                {
                    Cookies.Add(c);
                }
            }

            if (Response.Headers["ContentEncoding"] != null)
            {
                Stream receiveStream = Response.GetResponseStream();
                StreamReader sr = new StreamReader(receiveStream, Encoding.GetEncoding(Response.Headers["ContentEncoding"].ToString()));
                Content = sr.ReadToEnd();
            }
            else
            {
                try
                {
                    Stream receiveStream = Response.GetResponseStream();
                    StreamReader sr = new StreamReader(receiveStream);
                    Content = sr.ReadToEnd();
                }
                catch
                {

                }
            }
            return Response;
        }
    }
}