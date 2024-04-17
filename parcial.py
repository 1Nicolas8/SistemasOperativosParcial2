import time

class Process:
    def __init__(self, pid, burst_time):
        self.pid = pid
        self.burst_time = burst_time
        self.remaining_time = burst_time
        self.priority = 0

def execute_process(process, time_slice):
    start_time = time.time()
    while process.remaining_time > 0:
        if process.remaining_time > time_slice:
            process.remaining_time -= time_slice
            process.priority += 1
        else:
            process.remaining_time = 0
    end_time = time.time()
    return end_time - start_time

def mlfq(processes, time_slice):
    queues = [[] for _ in range(3)]
    for process in processes:
        queues[0].append(process)

    total_time = 0
    while any(queue for queue in queues):
        for queue_idx in range(3):
            if queues[queue_idx]:
                process = queues[queue_idx].pop(0)
                execution_time = execute_process(process, time_slice)
                total_time += execution_time
                if process.remaining_time > 0:
                    queues[min(queue_idx + 1, 2)].append(process)

    return total_time

processes = [Process(1, 5), Process(2, 3), Process(3, 7)]
time_slice = 2
start_time = time.time()
total_time = mlfq(processes, time_slice)
end_time = time.time()
print(f"Tiempo total de ejecución en Python: {total_time:.6f} segundos")
print(f"Tiempo real de ejecución en Python: {end_time - start_time:.6f} segundos")
