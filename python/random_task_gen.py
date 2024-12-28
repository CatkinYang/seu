import random
import json


def generate_random_tasks(num_tasks):
    tasks = []
    for task_id in range(1, num_tasks + 1):  # 从 id 1 开始
        clb = random.randint(400, 4000)  # CLB 范围：400-4000
        dsp = random.randint(int(clb * 0.10), int(clb * 0.15))  # DSP 是 CLB 的 10%-15%
        bram = random.randint(
            int(clb * 0.10), int(clb * 0.15)
        )  # BRAM 是 CLB 的 10%-15%
        exectime = random.randint(5, 20)  # 执行时间范围：5-20

        task = {
            "id": task_id,
            "clb": clb,
            "dsp": dsp,
            "bram": bram,
            "exectime": exectime,
        }
        tasks.append(task)
    return tasks


# 生成 10 个随机任务
num_tasks = 100
random_tasks = generate_random_tasks(num_tasks)

# 将数据保存为 JSON 文件
output_data = {"tasks": random_tasks}
with open("random_tasks.json", "w") as file:
    json.dump(output_data, file, indent=2)

print(f"已生成 {num_tasks} 个任务并保存到 random_tasks.json 文件中。")
