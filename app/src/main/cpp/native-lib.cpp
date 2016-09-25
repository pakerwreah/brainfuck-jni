#include <jni.h>
#include <string>
#include <thread>
#include "Utils.h"

using namespace std;

static bool stop = true;
static jobject thiz;
static jmethodID method_print;
static JavaVM *jvm;

#define AttachThread \
JNIEnv *env = 0; \
jvm->AttachCurrentThread(&env, NULL);

#define DetachThread \
jvm->DetachCurrentThread();

jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *env;
    if (vm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_6) != JNI_OK) {
        return -1;
    }

    jvm = vm;

    return JNI_VERSION_1_6;
}

void jprint(JNIEnv *env, const char *text) {
    jstring str = env->NewStringUTF(text);
    env->CallVoidMethod(thiz, method_print, str);
    env->DeleteLocalRef(str);
}

void jprint(JNIEnv *env, char c) {
    jprint(env, string(1, c).c_str());
}

void error(JNIEnv *env, string msg, int n) {
    stringstream os;
    os << endl << "ERROR: " << msg << " at character " << n << endl;
    jprint(env, os.str().c_str());
    stop = true;
}

extern "C"
void Java_br_newm_paker_brainfuck_MainActivity_stop(JNIEnv *env, jobject instance) {
    stop = true;
}

const int N = 100000;
int len;
char *source;

extern "C"
void Java_br_newm_paker_brainfuck_MainActivity_execute(JNIEnv *_env, jobject instance, jstring _code) {

    stop = false;
    thiz = _env->NewGlobalRef(instance);
    const char *code = _env->GetStringUTFChars(_code, NULL);
    //copy
    len = _env->GetStringUTFLength(_code);
    source = new char[len + 1];
    memcpy(source, code, len * sizeof(char));
    source[len] = '\0';

    thread([&] {
        AttachThread

        jclass clazz = env->GetObjectClass(thiz);
        method_print = env->GetMethodID(clazz, "print", "(Ljava/lang/String;)V");
        jmethodID method_finish = env->GetMethodID(clazz, "finish", "()V");

        if (!len)
            error(env, "Code is empty!", 0);
        else if (len > N)
            error(env, "Code is too big!", 0);
        else {

            int array[N] = {0}, looppos[100];
            int pos = 0, currentloop = -1;

            for (int n = 0; n < len && !stop; n++) {
                switch (source[n]) {
                    case '+': {
                        array[pos]++;
                        break;
                    }
                    case '-': {
                        array[pos]--;
                        break;
                    }
                    case '>': {
                        pos++;
                        break;
                    }
                    case '<': {
                        if (pos == 0)
                            error(env, "Negative position", n);
                        pos--;
                        break;
                    }
                    case '.': {
                        jprint(env, array[pos]);
                        break;
                    }
                    case '[': {

                        if (array[pos]) {
                            currentloop++;
                            looppos[currentloop] = n;
                        }
                        else {
                            int ignoreloops = 1;

                            while (ignoreloops && !stop) {
                                n++;
                                if (n >= len) {
                                    error(env, "Loop not closed", n);
                                    break;
                                }

                                if (source[n] == ']')
                                    ignoreloops--;
                                else if (source[n] == '[')
                                    ignoreloops++;
                            }
                        }

                        break;
                    }
                    case ']': {
                        if (currentloop < 0)
                            error(env, "Loop close without begin", n);

                        if (array[pos])
                            n = looppos[currentloop] - 1;

                        currentloop--;

                        break;
                    }
                    default:
                        break;
                }
            }

            if (currentloop > -1)
                error(env, "Loop not closed", len);
        }

        env->CallVoidMethod(thiz, method_finish);

        env->DeleteGlobalRef(thiz);

        DetachThread
    }).detach();
}
