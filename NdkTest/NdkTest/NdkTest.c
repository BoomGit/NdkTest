#define _CRT_SECURE_NO_WARNINGS
#include "com_boom_JNITest.h"
#include <string.h>
//C�ļ���Cpp�ļ��ǿ���ǰ�߲���
//static�ǿ��ƺ��߲���
//����ʵ��
JNIEXPORT jstring JNICALL Java_com_boom_JNITest_getStringFromC
(JNIEnv *env, jclass jcs){
	//WinExec("C:\\Program Files (x86)\\Tencent\\QQ\\Bin\\QQScLauncher.exe",0);

	//JNIEnv �ṹ��ָ��
	//env����ָ��
	//����Java���л���������Java�еĴ���
	//�򵥵�ʵ��
	//��C���ַ���תΪһ��java�ַ���
	return (*env)->NewStringUTF(env,"C String");
}

JNIEXPORT jstring JNICALL Java_com_boom_JNITest_getString2FromC
(JNIEnv *env, jobject jobj){
	return (*env)->NewStringUTF(env, "C String 2");
}

//ÿ��native������������������������JNIEnv*,jclass����jobject)
//1����native����Ϊ��̬����ʱ��
//jclass ����native�����������class����(JniTest.class)
//2����native����Ϊ�Ǿ�̬����ʱ��
//jobject ����native���������Ķ���

//��������
//Java��������������JNI�������͵�ӳ���ϵ
//Java����->JNI����->C����

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

//��������(����)
//String jstring
//object jobject
//����,�����������͵�����
//byte[] jByteArray
//��������
//object[](String[]) jobjectArray

//C/C++����Java�ĳ�Ա



//ǰ����ֻ�ǿ�ʼ
//�����ĸ���չʾstatic���ԺͷǾ�̬���� static�����ͷǾ�̬����

JNIEXPORT jstring JNICALL Java_com_boom_JNITest_accessField
(JNIEnv *env, jobject jobj){
	//jobj��t����NdkTest.class
	jclass cls = (*env)->GetObjectClass(env,jobj);
	//jfieldID
	//�������ƣ�����ǩ��
	jfieldID fid = (*env)->GetFieldID(env,cls,"name","Ljava/lang/String;");
	//jason >> super Jason
	//��ȡname���Ե�ֵ
	//Get<Type>Field 
	jstring jstr = (*env)->GetObjectField(env,jobj,fid);
	printf("jstr:%#x\n",&jstr);
	//jstring-->c�ַ���
	//isCopy �Ƿ��� ��true�����ƣ�false�������ƣ�
	char *c_str = (*env)->GetStringUTFChars(env,jstr,NULL);
	//ƴ�ӵõ��µ��ַ���
	char text[20] = "super";
	strcat(text,c_str);
	//c�ַ��� -> jstring
	jstring new_jstr = (*env)->NewStringUTF(env,text);
	//�޸�name
	//Set<Type>Field
	(*env)->SetObjectField(env,jobj,fid,new_jstr);
	printf("new_jstr:%#x",&new_jstr);
	return new_jstr;
}
//���ʾ�̬����
JNIEXPORT void JNICALL Java_com_boom_JNITest_accessStaticField
(JNIEnv *env, jobject jobj){
	//jclass
	jclass cls = (*env)->GetObjectClass(env,jobj);
	//jfieldID
	jfieldID fid = (*env)->GetStaticFieldID(env,cls,"age","I");
	//GetStatic<Type>Field
	jint age = (*env)->GetStaticIntField(env,cls,fid);
	age++;
	//�޸�
	//SetStatic<Type>Field
	(*env)->SetStaticIntField(env,cls,fid,age);
}

//����java����
JNIEXPORT void JNICALL Java_com_boom_JNITest_accessMethod
(JNIEnv *env, jobject jobj){
	//jclass
	jclass cls = (*env)->GetObjectClass(env,jobj);
	//jmethodID
	jmethodID mid = (*env)->GetMethodID(env,cls,"getRandomInt","(I)I");
	//����
	//Call<Type>Method
	jint random = (*env)->CallIntMethod(env,jobj,mid,200);
	printf("random num:%#x",random);
}

//����Java��̬����
JNIEXPORT void JNICALL Java_com_boom_JNITest_accessStaticMethod
(JNIEnv *env, jobject jobj){
	//jclass
	jclass cls = (*env)->GetObjectClass(env,jobj);
	//jmethodID    ǩ��: ()Ljava/lang/String; ͨ��javap -s-p ���Ȯ����  ��ȡ
	jmethodID mid = (*env)->GetStaticMethodID(env,cls,"getUUID","()Ljava/lang/String;");
	//����
	//CallStatic<Type>Method
	jstring uuid = (*env)->CallStaticObjectMethod(env,cls,mid);

	//����ļ����� uuid.txt
	//jstring  -> char*
	//isCopy JNI_FALSE ,����java��C��������ͬһ���ַ���
	char *uuid_str = (*env)->GetStringUTFChars(env,uuid,JNI_FALSE);
	//ƴ��
	char filename[100];
	sprintf(filename,"E://s.text",uuid_str);
	FILE *fp = fopen(filename,"w");
	fputs("i love u",fp);
	fclose(fp);
}

//���ʹ��췽��
//ʹ��java.util.Date����һ����ǰ��ʱ���
JNIEXPORT jobject JNICALL Java_com_boom_JNITest_accessConstaructor
(JNIEnv * env, jobject jobj){
	//jclass
	jclass cls = (*env)->FindClass(env,"java/util/Date");
	//jmethodID
	jmethodID constaructor_mid = (*env)->GetMethodID(env,cls,"<init>","()V");
	//ʵ��һ��Date����
	jobject date_obj = (*env)->NewObject(env,cls,constaructor_mid);
	//����getTime����
	jmethodID mid = (*env)->GetObjectClass(env,date_obj);
	jlong time = (*env)->CallLongMethod(env,date_obj,mid);
	
	printf("\ntime:%lld\n",time);
	return date_obj;
}

//���ø���ķ���
JNIEXPORT void JNICALL Java_com_boom_JNITest_accessNovirtualMethod
(JNIEnv *env, jobject jobj){
	jclass cls = (*env)->GetObjectClass(env, jobj);
	//��ȡman����
	jfieldID fid = (*env)->GetFieldID(env,cls,"human","Lcom/boom/Human");
	//��ȡ
	jobject human_obj = (*env)->GetObjectField(env,jobj,fid);
	//ִ��sayHi()����
	jclass human_cls = (*env)->FindClass(env,"com/boom/Human");//ע�⴫���������
	jmethodID mid = (*env)->GetMethodID(env,human_cls,"sayHi","()V");
	//ִ��
	//(*env)->CallObjectMethod(env,human_obj,mid);
	//���õĸ��෽��
	(*env)->CallNonvirtualObjectMethod(env,human_obj,human_cls,mid);
}
//��������
JNIEXPORT jstring JNICALL Java_com_boom_JNITest_chineseChars
(JNIEnv *env, jobject jobj, jstring in){
	//���
	//char *c_str = (*env)->GetStringUTFChars(env,in,JNI_FALSE);
	//printf("%s\n",c_str);

	//c->jstring
	char *c_str = "�������ν�";
	//char c_str[] = "�������ν�";
	//jstring jstr = (*env)->NewStringUTF(env,c_str);
	//ִ��String(byte bytes[],String charestName)���췽����Ҫ������
	//1.jmethodID
	//2.byte����
	//3.�ַ����� jstring

	jclass str_cls = (*env)->FindClass(env, "java/lang/String");
	jmethodID constaructor_mid = (*env)->GetMethodID(env,str_cls,"<init>","([BLjava/lang/String;)V");
	
	//jbyte ->char
	//jbyteArray ->char[]
	jbyteArray bytes = (*env)->NewByteArray(env,strlen(c_str));
	//byte ���鸳ֵ
	//0 -> strlen(c_str)  ��ͷ��β
	//�Ե��ڣ���c_str����ַ����飬���Ƶ�bytes����ַ�����
	(*env)->SetByteArrayRegion(env,bytes,0,strlen(c_str),c_str);
	//�ַ�����jstring 
	jstring charsetName = (*env)->NewStringUTF(env,"GB2312");

	//���ù��캯�������ر���֮���jstring
	return (*env)->NewObject(env,str_cls,constaructor_mid,bytes,charsetName);

}
//�����Դ���иĵ�  �Ǹ�ָ�뺯��
int compare(int *a,int *b){
	return (*a) - (*b);
}

//����
JNIEXPORT void JNICALL Java_com_boom_JNITest_giveArray
(JNIEnv *env, jobject jobj, jintArray arr){
	//jintArray ->jintָ�� -> c int ����
	jint *elems = (*env)->GetIntArrayElements(env,arr,NULL);
	//printf("%#x,%#x\n",&elems,&arr);
	
	//����ĳ���
	int len = (*env)->GetArrayLength(env,arr);
	//����  ����ָ�뺯��
	qsort(elems,len,sizeof(jint),compare);
	//ͬ��
	//mode
	//0 ,Java ������и��£������ͷ�C/C++����
	//JNI_ABORT java ���鲻���и��£������ͷ�C/C++����
	//JNI_COMMIT java ������и��£����ͷ�C/C++���飨����ִ���꣬���黹�ǻ��ͷţ�
	(*env)->ReleaseIntArrayElements(env,arr,elems,JNI_COMMIT);
}
//��������
JNIEXPORT jintArray JNICALL Java_com_boom_JNITest_getArray
(JNIEnv *env, jobject jobj, jint len){
	//����һ��ָ����С������
	jintArray jint_arr = (*env)->NewIntArray(env,len);
	jint *elems = (*env)->GetIntArrayElements(env,jint_arr,NULL);
	int i = 0;
	for (;i<len;i++){
		elems[i] = i;
	}
	//ͬ��
	(*env)->ReleaseIntArrayElements(env,jint_arr,elems,0);

	return jint_arr;
}

//JNI  ���ñ���
//�������ͣ��ֲ����ú�ȫ������
//���ã���JNI�и�֪�������ʱ����һ��JNI����

//�ֲ�����
//1.����һ���ܴ��java����ʹ����֮�󣬻�Ҫ���и��ӵĺ�ʱ����
//2.�����˴����ľֲ����ã�ռ����̫����ڴ棬������Щ�ֲ����ø�����Ĳ���û�й�����

//ģ�⣺ ѭ����������
JNIEXPORT void JNICALL Java_com_boom_JNITest_localRef
(JNIEnv *env, jobject jobj){
	int i = 0;
	//����Date����
	jclass cls = (*env)->FindClass(env,"java/util/Date");
	jmethodID constaructor_mid = (*env)->GetMethodID(env,cls,"<init>","()V");
	jobject obj = (*env)->NewObject(env,cls,constaructor_mid);
	//�˴�ʡ��һ���д���

	//����ʹ��jobject����
	//֪ͨ����������������Щ����
	(*env)->DeleteLocalRef(env,obj);
	//�˴�ʡ��һ���д���

}

//ȫ������
//
jstring  global_str;
//����
JNIEXPORT void JNICALL Java_com_boom_JNITest_createGlobalRef
(JNIEnv *env, jobject jobj){
	jobject obj = (*env)->NewStringUTF(env,"jni development is powerful");
	global_str = (*env)->NewGlobalRef(env,obj);
}
//�õ�
JNIEXPORT jstring JNICALL Java_com_boom_JNITest_getGloballRef
(JNIEnv *env, jobject jobj){
	return global_str;
}
//�ͷ�
JNIEXPORT void JNICALL Java_com_boom_JNITest_deleteGlobalRef
(JNIEnv *env, jobject jobj){
	(*env)->DeleteGlobalRef(env, global_str);
}

//��ȫ������
//��ʡ�ڴ棬���ڴ治���ʱ�򣬿����ͷ������õĶ���
//��������һ�������õĶ������ΪNULL,��ʱ����
//������New
	
//�쳣����
//1.��֤Java�����������
//2.���ȴ�ʩ��֤C�����������

//JNI�Լ��׳����쳣����Java���޷�����׽��ֻ����C�����
//�û�ͨ��ThrowNew�׳��쳣
JNIEXPORT void JNICALL Java_com_boom_JNITest_exception
(JNIEnv *env, jobject jobj){
	jclass cls = (*env)->GetObjectClass(env,jobj);
	jfieldID fid = (*env)->GetFieldID(env, cls, "key2", "Ljava/lang/String;");
	//����Ƿ���Java�쳣
	jthrowable exception = (*env)->ExceptionOccurred(env);
	if (exception !=NULL){
		//��Java������Լ�������
		//����쳣��Ϣ
		(*env)->ExceptionClear(env);
		//���ȴ�ʩ
		fid = (*env)->GetFieldID(env,cls,"name","Ljava/lang/String;");
	}
	//��ȡ���Ե�ֵ
	jstring jstr = (*env)->GetObjectField(env,jobj,fid);
	char *str = (*env)->GetStringUTFChars(env,jstr,NULL);
	//�Ա�����ֵ�Ƿ�Ϸ�
	if (_stricmp(str,"super jason")!=0){
		//��Ϊ�׳��쳣����Java���봦��
		jclass newExcCls = (*env)->FindClass(env,"java/lang/IllegalArgumentException");
		(*env)->ThrowNew(env,newExcCls,"key's value is incalid!");
	}

}