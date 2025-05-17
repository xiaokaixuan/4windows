using System;
using System.Reflection;

namespace Wifi_Share
{
    public abstract class TSingleton<T> where T : class
    {
        /// <summary>
        /// Singleton
        /// </summary>
        private static class Nested
        {
            public static readonly T instance;
            static Nested() // Cancel BeforeFieldInit
            {
                ConstructorInfo constructor = typeof(T).GetConstructor(
                BindingFlags.Instance | BindingFlags.NonPublic | BindingFlags.DeclaredOnly, null, Type.EmptyTypes, null
                ) ?? throw new InvalidOperationException($"\"{typeof(T).FullName}\", 必须声明一个私有的或受保护的无参构造函数才能用作单例。");
                instance = (T)constructor.Invoke(null);
            }
        }
        public static T Instance => Nested.instance;
    }
}

