#define _CRT_SECURE_NO_WARNINGS
#include "com_boom_JNITest.h"
#include <string.h>
//C文件或Cpp文件是控制前者参数
//static是控制后者参数
//函数实现
JNIEXPORT jstring JNICALL Java_com_boom_JNITest_getStringFromC
(JNIEnv *env, jclass jcs){
	//WinExec("C:\\Program Files (x86)\\Tencent\\QQ\\Bin\\QQScLauncher.exe",0);

	//JNIEnv 结构体指针
	//env二级指针
	//代表Java运行环境，调用Java中的代码
	//简单的实现
	//将C的字符串转为一个java字符串
	return (*env)->NewStringUTF(env,"C String");
}

JNIEXPORT jstring JNICALL Java_com_boom_JNITest_getString2FromC
(JNIEnv *env, jobject jobj){
	return (*env)->NewStringUTF(env, "C String 2");
}

//每个native函数，都至少有两个参数（JNIEnv*,jclass或者jobject)
//1）当native方法为静态方法时：
//jclass 代表native方法所属类的class对象(JniTest.class)
//2）当native方法为非静态方法时：
//jobject 代表native方法所属的对象

//基本数据
//Java基本数据类型与JNI数据类型的映射关系
//Java类型->JNI类型->C类型

/*
boolean jboolean
byte jbyte;
char jchar;
short jshort;
int jint;
long jlong;
float jfloat;
double jdouble;
void void
*/

//引用类型(对象)
//String jstring
//object jobject
//数组,基本数据类型的数组
//byte[] jByteArray
//对象数组
//object[](String[]) jobjectArray

//C/C++访问Java的成员



//前两个只是开始
//后面四个是展示static属性和非静态属性 static方法和非静态方法

JNIEXPORT jstring JNICALL Java_com_boom_JNITest_accessField
(JNIEnv *env, jobject jobj){
	//jobj是t对象，NdkTest.class
	jclass cls = (*env)->GetObjectClass(env,jobj);
	//jfieldID
	//属性名称，属性签名
	jfieldID fid = (*env)->GetFieldID(env,cls,"name","Ljava/lang/String;");
	//jason >> super Jason
	//获取name属性的值
	//Get<Type>Field 
	jstring jstr = (*env)->GetObjectField(env,jobj,fid);
	printf("jstr:%#x\n",&jstr);
	//jstring-->c字符串
	//isCopy 是否复制 （true代表复制，false代表不复制）
	char *c_str = (*env)->GetStringUTFChars(env,jstr,NULL);
	//拼接得到新的字符串
	char text[20] = "super";
	strcat(text,c_str);
	//c字符串 -> jstring
	jstring new_jstr = (*env)->NewStringUTF(env,text);
	//修改name
	//Set<Type>Field
	(*env)->SetObjectField(env,jobj,fid,new_jstr);
	printf("new_jstr:%#x",&new_jstr);
	return new_jstr;
}
//访问静态属性
JNIEXPORT void JNICALL Java_com_boom_JNITest_accessStaticField
(JNIEnv *env, jobject jobj){
	//jclass
	jclass cls = (*env)->GetObjectClass(env,jobj);
	//jfieldID
	jfieldID fid = (*env)->GetStaticFieldID(env,cls,"age","I");
	//GetStatic<Type>Field
	jint age = (*env)->GetStaticIntField(env,cls,fid);
	age++;
	//修改
	//SetStatic<Type>Field
	(*env)->SetStaticIntField(env,cls,fid,age);
}

//访问java方法
JNIEXPORT void JNICALL Java_com_boom_JNITest_accessMethod
(JNIEnv *env, jobject jobj){
	//jclass
	jclass cls = (*env)->GetObjectClass(env,jobj);
	//jmethodID
	jmethodID mid = (*env)->GetMethodID(env,cls,"getRandomInt","(I)I");
	//调用
	//Call<Type>Method
	jint random = (*env)->CallIntMethod(env,jobj,mid,200);
	printf("random num:%#x",random);
}

//访问Java静态方法
JNIEXPORT void JNICALL Java_com_boom_JNITest_accessStaticMethod
(JNIEnv *env, jobject jobj){
	//jclass
	jclass cls = (*env)->GetObjectClass(env,jobj);
	//jmethodID    签名: ()Ljava/lang/String; 通过javap -s-p 类的犬类名  获取
	jmethodID mid = (*env)->GetStaticMethodID(env,cls,"getUUID","()Ljava/lang/String;");
	//调用
	//CallStatic<Type>Method
	jstring uuid = (*env)->CallStaticObjectMethod(env,cls,mid);

	//随机文件名称 uuid.txt
	//jstring  -> char*
	//isCopy JNI_FALSE ,代表java和C操作的是同一个字符串
	char *uuid_str = (*env)->GetStringUTFChars(env,uuid,JNI_FALSE);
	//拼接
	char filename[100];
	sprintf(filename,"E://s.text",uuid_str);
	FILE *fp = fopen(filename,"w");
	fputs("i love u",fp);
	fclose(fp);
}

//访问构造方法
//使用java.util.Date产生一个当前的时间戳
JNIEXPORT jobject JNICALL Java_com_boom_JNITest_accessConstaructor
(JNIEnv * env, jobject jobj){
	//jclass
	jclass cls = (*env)->FindClass(env,"java/util/Date");
	//jmethodID
	jmethodID constaructor_mid = (*env)->GetMethodID(env,cls,"<init>","()V");
	//实例一个Date对象
	jobject date_obj = (*env)->NewObject(env,cls,constaructor_mid);
	//调用getTime方法
	jmethodID mid = (*env)->GetObjectClass(env,date_obj);
	jlong time = (*env)->CallLongMethod(env,date_obj,mid);
	
	printf("\ntime:%lld\n",time);
	return date_obj;
}

//调用父类的方法
JNIEXPORT void JNICALL Java_com_boom_JNITest_accessNovirtualMethod
(JNIEnv *env, jobject jobj){
	jclass cls = (*env)->GetObjectClass(env, jobj);
	//获取man属性
	jfieldID fid = (*env)->GetFieldID(env,cls,"human","Lcom/boom/Human");
	//获取
	jobject human_obj = (*env)->GetObjectField(env,jobj,fid);
	//执行sayHi()方法
	jclass human_cls = (*env)->FindClass(env,"com/boom/Human");//注意传父类的名称
	jmethodID mid = (*env)->GetMethodID(env,human_cls,"sayHi","()V");
	//执行
	//(*env)->CallObjectMethod(env,human_obj,mid);
	//调用的父类方法
	(*env)->CallNonvirtualObjectMethod(env,human_obj,human_cls,mid);
}
//中文问题
JNIEXPORT jstring JNICALL Java_com_boom_JNITest_chineseChars
(JNIEnv *env, jobject jobj, jstring in){
	//输出
	//char *c_str = (*env)->GetStringUTFChars(env,in,JNI_FALSE);
	//printf("%s\n",c_str);

	//c->jstring
	char *c_str = "马蓉与宋江";
	//char c_str[] = "马蓉与宋江";
	//jstring jstr = (*env)->NewStringUTF(env,c_str);
	//执行String(byte bytes[],String charestName)构造方法需要的条件
	//1.jmethodID
	//2.byte数组
	//3.字符编码 jstring

	jclass str_cls = (*env)->FindClass(env, "java/lang/String");
	jmethodID constaructor_mid = (*env)->GetMethodID(env,str_cls,"<init>","([BLjava/lang/String;)V");
	
	//jbyte ->char
	//jbyteArray ->char[]
	jbyteArray bytes = (*env)->NewByteArray(env,strlen(c_str));
	//byte 数组赋值
	//0 -> strlen(c_str)  从头到尾
	//对等于，从c_str这个字符数组，复制到bytes这个字符数组
	(*env)->SetByteArrayRegion(env,bytes,0,strlen(c_str),c_str);
	//字符编码jstring 
	jstring charsetName = (*env)->NewStringUTF(env,"GB2312");

	//调用构造函数，返回编码之后的jstring
	return (*env)->NewObject(env,str_cls,constaructor_mid,bytes,charsetName);

}
//这个从源码中改的  是个指针函数
int compare(int *a,int *b){
	return (*a) - (*b);
}

//传入
JNIEXPORT void JNICALL Java_com_boom_JNITest_giveArray
(JNIEnv *env, jobject jobj, jintArray arr){
	//jintArray ->jint指针 -> c int 数组
	jint *elems = (*env)->GetIntArrayElements(env,arr,NULL);
	//printf("%#x,%#x\n",&elems,&arr);
	
	//数组的长度
	int len = (*env)->GetArrayLength(env,arr);
	//排序  调用指针函数
	qsort(elems,len,sizeof(jint),compare);
	//同步
	//mode
	//0 ,Java 数组进行更新，并且释放C/C++数组
	//JNI_ABORT java 数组不进行更新，但是释放C/C++数组
	//JNI_COMMIT java 数组进行更新，不释放C/C++数组（函数执行完，数组还是会释放）
	(*env)->ReleaseIntArrayElements(env,arr,elems,JNI_COMMIT);
}
//返回数组
JNIEXPORT jintArray JNICALL Java_com_boom_JNITest_getArray
(JNIEnv *env, jobject jobj, jint len){
	//创建一个指定大小的数组
	jintArray jint_arr = (*env)->NewIntArray(env,len);
	jint *elems = (*env)->GetIntArrayElements(env,jint_arr,NULL);
	int i = 0;
	for (;i<len;i++){
		elems[i] = i;
	}
	//同步
	(*env)->ReleaseIntArrayElements(env,jint_arr,elems,0);

	return jint_arr;
}

//JNI  引用变量
//引用类型：局部引用和全局引用
//作用：在JNI中告知虚拟机何时回收一个JNI变量

//局部引用
//1.访问一个很大的java对象，使用完之后，还要进行复杂的耗时操作
//2.创建了大量的局部引用，占用了太多的内存，而且这些局部引用跟后面的操作没有关联性

//模拟： 循环创建数组
JNIEXPORT void JNICALL Java_com_boom_JNITest_localRef
(JNIEnv *env, jobject jobj){
	int i = 0;
	//创建Date对象
	jclass cls = (*env)->FindClass(env,"java/util/Date");
	jmethodID constaructor_mid = (*env)->GetMethodID(env,cls,"<init>","()V");
	jobject obj = (*env)->NewObject(env,cls,constaructor_mid);
	//此处省略一百行代码

	//不再使用jobject对象
	//通知垃圾回收器回收这些对象
	(*env)->DeleteLocalRef(env,obj);
	//此处省略一百行代码

}

//全局引用
//
jstring  global_str;
//创建
JNIEXPORT void JNICALL Java_com_boom_JNITest_createGlobalRef
(JNIEnv *env, jobject jobj){
	jobject obj = (*env)->NewStringUTF(env,"jni development is powerful");
	global_str = (*env)->NewGlobalRef(env,obj);
}
//得到
JNIEXPORT jstring JNICALL Java_com_boom_JNITest_getGloballRef
(JNIEnv *env, jobject jobj){
	return global_str;
}
//释放
JNIEXPORT void JNICALL Java_com_boom_JNITest_deleteGlobalRef
(JNIEnv *env, jobject jobj){
	(*env)->DeleteGlobalRef(env, global_str);
}

//弱全局引用
//节省内存，在内存不足的时候，可以释放所引用的对象
//可以引用一个不常用的对象，如果为NULL,临时创建
//创建：New
	
//异常处理
//1.保证Java代码可以运行
//2.补救措施保证C代码继续运行

//JNI自己抛出的异常，在Java层无法被捕捉，只能在C层清空
//用户通过ThrowNew抛出异常
JNIEXPORT void JNICALL Java_com_boom_JNITest_exception
(JNIEnv *env, jobject jobj){
	jclass cls = (*env)->GetObjectClass(env,jobj);
	jfieldID fid = (*env)->GetFieldID(env, cls, "key2", "Ljava/lang/String;");
	//检测是否发生Java异常
	jthrowable exception = (*env)->ExceptionOccurred(env);
	if (exception !=NULL){
		//让Java代码可以继续运行
		//清空异常信息
		(*env)->ExceptionClear(env);
		//补救措施
		fid = (*env)->GetFieldID(env,cls,"name","Ljava/lang/String;");
	}
	//获取属性的值
	jstring jstr = (*env)->GetObjectField(env,jobj,fid);
	char *str = (*env)->GetStringUTFChars(env,jstr,NULL);
	//对比属性值是否合法
	if (_stricmp(str,"super jason")!=0){
		//人为抛出异常，给Java代码处理
		jclass newExcCls = (*env)->FindClass(env,"java/lang/IllegalArgumentException");
		(*env)->ThrowNew(env,newExcCls,"key's value is incalid!");
	}

}