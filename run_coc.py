Import("env")
import os
import subprocess

# Đường dẫn tuyệt đối tới thư mục berry
berry_dir = os.path.join(env.subst("$PROJECT_DIR"), "lib", "libesp32", "berry")

# Thư mục chứa script coc
coc_script = os.path.join(berry_dir, "tools", "coc", "coc")

# Các thư mục input để generate
input_dirs = [
    "src",
    "default",
    "../re1.5",
    "../berry_mapping/src",
    "../berry_int64/src",
    "../berry_matter/src",
    "../berry_animate/src"
]

# Nối thành lệnh gọi
command = ["python", coc_script, "-o", "generate"] + input_dirs + ["-c", "default/berry_conf.h"]

print("[run_coc.py] Generating Berry constant tables...")
subprocess.run(command, cwd=berry_dir, check=True)
print("[run_coc.py] Done generating.")
