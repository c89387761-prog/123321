#include <jni.h>
#include <vector>
#include <fstream>
#include <cmath>

class WorldModel {
public:
    std::vector<float> weights;
    std::string path;

    WorldModel(std::string p) : path(p + "/knowledge.bin") {
        weights.resize(256, 0.5f); // 256 нейронов "знаний"
        load();
    }

    void load() {
        std::ifstream in(path, std::ios::binary);
        if(in) in.read((char*)weights.data(), weights.size() * sizeof(float));
    }

    void save() {
        std::ofstream out(path, std::ios::binary);
        out.write((char*)weights.data(), weights.size() * sizeof(float));
    }

    // Имитация обучения: корректируем веса на основе "успеха"
    void learn(int reward) {
        for(auto& w : weights) w += 0.01f * reward; 
        save(); // Сохраняем мгновенно
    }

    // Решение: куда нажать?
    std::pair<int, int> decide() {
        int x = (int)(weights[0] * 1000) % 1080;
        int y = (int)(weights[1] * 1000) % 1920;
        return {x, y};
    }
};

WorldModel* ai = nullptr;

extern "C" {
    JNIEXPORT void JNICALL Java_com_godmode_worldmodel_NativeLib_init(JNIEnv* env, jobject idx, jstring p) {
        const char* path = env->GetStringUTFChars(p, 0);
        ai = new WorldModel(path);
        env->ReleaseStringUTFChars(p, path);
    }

    JNIEXPORT jintArray JNICALL Java_com_godmode_worldmodel_NativeLib_step(JNIEnv* env, jobject idx) {
        auto coords = ai->decide();
        ai->learn(1); // ИИ всегда думает, что он молодец, и учится
        jintArray res = env->NewIntArray(2);
        int arr[2] = {coords.first, coords.second};
        env->SetIntArrayRegion(res, 0, 2, arr);
        return res;
    }
}