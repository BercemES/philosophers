# Philosophers (philo)

A C project that simulates the **Dining Philosophers** problem using multithreading. Each philosopher runs in its own thread; fork sharing is protected with `pthread_mutex`, and an observer thread monitors death and meal-count conditions.

## Problem Overview

N philosophers sit around a circular table. Each philosopher has one fork in front of them — N forks in total. Philosophers repeat the following cycle:

1. Pick up the left and right forks
2. Eat
3. Put down the forks
4. Sleep
5. Think

The simulation ends when:

- A philosopher **dies** if they do not eat within `time_to_die` milliseconds
- *(Optional)* The simulation **ends successfully** when every philosopher has eaten `must_eat` times

## Requirements

- **GCC/Clang** (`cc`)
- **POSIX threads** support (`pthread`)
- **Linux**, **macOS**, or **WSL** (does not compile natively on Windows)

## Build

```bash
make
```

Cleanup:

```bash
make clean    # remove object files
make fclean   # remove object files and binary
make re       # rebuild from scratch
```

Compiler flags: `-Wall -Wextra -Werror -g -fsanitize=thread -pthread`

## Usage

```bash
./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]
```

| Argument | Description |
|----------|-------------|
| `number_of_philosophers` | Number of philosophers (cannot be 0) |
| `time_to_die` | Maximum time without eating before death (ms) |
| `time_to_eat` | Time spent eating (ms) |
| `time_to_sleep` | Time spent sleeping (ms) |
| `number_of_times_each_philosopher_must_eat` | *(optional)* Minimum number of meals each philosopher must eat |

All arguments must be positive integers.

### Examples

```bash
# 5 philosophers, die after 800 ms without eating, eat for 200 ms, sleep for 200 ms
./philo 5 800 200 200

# Each philosopher must eat at least 7 times
./philo 5 800 200 200 7

# Single philosopher — picks up a fork but dies because there is no second fork
./philo 1 800 200 200
```

## Output Format

Each line shows the elapsed time since the start of the simulation (ms), the philosopher ID, and the action:

```
timestamp philosopher_id action
```

Possible actions:

| Action | Meaning |
|--------|---------|
| `has taken a fork` | Picked up a fork |
| `is eating` | Eating |
| `is sleeping` | Sleeping |
| `is thinking` | Thinking |
| `died` | Died of starvation |

Death messages are printed in red; eating messages are printed in green.

## Architecture

```
main.c           → Argument validation and simulation startup
init.c           → Philosopher, fork, and mutex initialization; thread creation
routine.c        → Philosopher loop and observer (monitor) thread
utils.c          → ft_atoi, digit_control, current_time
print_and_free.c → Thread-safe printing and memory cleanup
philo.h          → Data structures and function prototypes
```

### Data Structures

- **`t_philo`**: Philosopher ID, last meal time, meals eaten, left/right fork mutexes
- **`t_simulator`**: Simulation parameters, fork array, stop flag, observer thread

### Key Design Decisions

- **Deadlock prevention**: Philosophers with odd IDs pick up the right fork first; even IDs pick up the left fork first. Even-ID philosophers start with a 200 µs delay
- **Observer thread** (`check_philo`): Periodically checks for starvation; if `must_eat` is set, verifies whether all philosophers have reached their target
- **Mutexes**: Forks, printing, the death flag, and each philosopher's meal data are protected by separate locks
- **Single philosopher**: Dies after `time_to_die` because there is no second fork available

## License

This project is part of the [42 School](https://www.42.fr/) curriculum.
