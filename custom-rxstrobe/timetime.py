import time
from datetime import datetime, timedelta

def sleep_until_next_minute():
    # Get the current time
    now = datetime.now()
    
    # Calculate the time for the next minute
    next_minute = (now + timedelta(minutes=1)).replace(second=0, microsecond=0)
    
    # Calculate how many seconds to sleep
    sleep_seconds = (next_minute - now).total_seconds()
    
    # Sleep until the next minute
    time.sleep(sleep_seconds)

    print(f"Slept for {sleep_seconds:.2f} seconds. It's now exactly {datetime.now().strftime('%H:%M:%S')}")

if __name__ == "__main__":
    print(f"Current time: {datetime.now().strftime('%H:%M:%S')}")
    sleep_until_next_minute()
    print("Task starting now!")

