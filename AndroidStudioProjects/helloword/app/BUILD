android_library(
    name = "basic_lib",
    srcs = glob(["*.java"]),
    manifest = "AndroidManifest.xml",
    resource_files = glob(["res/**"]),
    deps = [
        "//mediapipe/java/com/google/mediapipe/components:android_components//third_party:android_constraint_layout",
        "//third_party:androidx_appcompat",
        "//mediapipe/java/com/google/mediapipe/components:android_components",
    ],
)

android_binary(
    name = "helloworld",
    assets = [
        "//mediapipe/graphs/edge_detection:mobile_gpu.binarypb",
    ],
    manifest = "AndroidManifest.xml",
    manifest_values = {
        "applicationId": "com.google.mediapipe.apps.basic",
        "appName": "Hello World",
        "mainActivity": ".MainActivity",
    },
    multidex = "native",
    deps = [
        ":basic_lib",
    ],
)
