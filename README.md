*This project has been created as part of the 42 curriculum by dsoto-ga.*

# get_next_line

## Description

`get_next_line` is a C function that reads a file descriptor **one line at a
time**, from start to finish, no matter how large the file is and no matter how
small the reading buffer is.

The goal of the project is to learn how to work with file descriptors, dynamic
memory management and **static variables**, which allow a function to preserve
state between successive calls. Every call to `get_next_line` returns the next
line read from the given file descriptor, including the terminating newline
character (`\n`) when present, and returns `NULL` when there is nothing left to
read or when an error occurs.

The function prototype is:

```c
char *get_next_line(int fd);
```

The size of the internal read buffer is defined at compile time through the
`BUFFER_SIZE` macro, so the implementation must behave correctly for any value
(1, 42, 9999, ...).

### Bonus

The bonus part extends the mandatory version so that `get_next_line` can manage
**multiple file descriptors at the same time** without losing the reading
context of each of them. In the code this is achieved with an array of static
buffers indexed by the file descriptor (`FD_MAX` entries), instead of a single
static buffer.

## Instructions

There is no `Makefile` in this project: `get_next_line` is meant to be compiled
together with the source files of whatever program uses it. `BUFFER_SIZE` can be
set at compile time with the `-D` flag; if it is not provided, the header falls
back to a default value of `42`.

### Files

| File | Description |
|------|-------------|
| `get_next_line.c` / `get_next_line.h` | Mandatory version (single fd) |
| `get_next_line_utils.c` | Helper functions for the mandatory version |
| `get_next_line_bonus.c` / `get_next_line_bonus.h` | Bonus version (multiple fds) |
| `get_next_line_utils_bonus.c` | Helper functions for the bonus version |

### Compiling the mandatory version

Compile the source files together with your own program (the one that calls
`get_next_line` from a `main`):

```sh
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 \
   get_next_line.c get_next_line_utils.c your_main.c -o gnl
./gnl
```

### Compiling the bonus version

```sh
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 \
   get_next_line_bonus.c get_next_line_utils_bonus.c your_main.c -o gnl_bonus
./gnl_bonus
```

> Note: include `get_next_line.h` in your program for the mandatory version, or
> `get_next_line_bonus.h` for the bonus version.

### Usage example

```c
#include "get_next_line.h"
#include <fcntl.h>
#include <stdio.h>

int main(void)
{
    int   fd = open("test.txt", O_RDONLY);
    char *line;

    while ((line = get_next_line(fd)) != NULL)
    {
        printf("%s", line);
        free(line);
    }
    close(fd);
    return (0);
}
```

## Algorithm

The core idea is to **read in fixed-size chunks and stitch them together** until
a newline is found, while keeping any leftover bytes for the next call.

### Data kept between calls

A `static` variable survives between calls to the function, which is exactly what
lets `get_next_line` remember where it stopped:

- **Mandatory:** a single `static char *buffer` holds whatever was read from the
  file descriptor but not yet returned to the caller.
- **Bonus:** a `static char *fd_table[FD_MAX]` holds one such buffer per file
  descriptor, so several files can be read in an interleaved way without mixing
  their contents.

### Step by step

For each call to `get_next_line(fd)`:

1. **Validate and initialize.** Reject invalid input (`fd < 0`,
   `BUFFER_SIZE <= 0`) and lazily allocate the persistent buffer with
   `init_buf` the first time it is used.
2. **Search the leftover buffer.** Use `ft_strchr` to look for a `\n` in the
   bytes that were kept from a previous call.
   - If a newline is found, `cut_line` appends everything up to and including
     that `\n` to `line`, and `shift` moves the remaining bytes to the front of
     the buffer so they are available next time. The finished line is returned.
3. **Read more data.** If no newline is present, append the current buffer
   content to `line` (via `ft_realloc`), reset the buffer, and `read`
   `BUFFER_SIZE` more bytes from the file descriptor.
   - `read` returning `0` means end of file; a negative value means an error.
4. **Repeat** from step 2 until a newline is found or the file is exhausted.
5. **Finish.** On a read error, or when the last line is empty, free `line` and
   return `NULL`. Otherwise return the accumulated line (the last line of a file
   without a trailing `\n` is returned as-is).

### Why this design

- **Chunked reading (`BUFFER_SIZE`)** decouples the size of the returned line
  from the size of the read, so arbitrarily long lines work even with a
  1-byte buffer.
- **Growing the line with `ft_realloc`** (a custom join-and-reallocate helper)
  concatenates the previous partial line with newly read data, then frees the
  old block, keeping memory usage proportional to the current line rather than
  the whole file.
- **Keeping the remainder in a static buffer** avoids re-reading or seeking:
  bytes read after a `\n` simply wait for the next call. This is what makes the
  function stateful and efficient.
- **`shift` instead of reallocating the leftover** keeps the helper logic simple
  and avoids an extra allocation on every returned line.
- **An array indexed by `fd` (bonus)** is the smallest change that isolates each
  file descriptor's state, satisfying the multi-fd requirement while reusing the
  exact same algorithm.

The helper functions (`ft_strchr`, `ft_strlen`, `ft_memcpy`, `ft_realloc`) are
re-implemented from scratch because the project only allows `read`, `malloc` and
`free`.

## Resources

- **The Norm** — the 42 coding standard every file must comply with. Official
  repository: <https://github.com/42School/norm>.
- **man pages**.
- <https://www.youtube.com/watch?v=-Mt2FdJjVno>
- <https://www.youtube.com/watch?v=8E9siq7apUU>
- **Peer help.**
- **gnlTester** (<https://github.com/Tripouille/gnlTester>) — community test
  suite used to validate edge cases and check for memory leaks with Valgrind.

### Use of AI

AI tools were used only to help draft and structure this `README.md`. The
algorithm design, the implementation and the debugging of the code were done by
the author.
