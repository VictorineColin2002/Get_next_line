*This project has been created as part of the 42 curriculum by vcolin.*

# get_next_line

42 common core project: a function that reads a file descriptor one line at
a time, regardless of the file's content or size, and regardless of the
`BUFFER_SIZE` value used at compile time.

## The problem the subject sets up

`read()` has no notion of a "line": it returns a fixed number of bytes
(`BUFFER_SIZE`) without any regard for where newlines fall. A line can
therefore:

- fit entirely within a single call to `read()`,
- be split across several calls if it's longer than `BUFFER_SIZE`,
- or several complete lines can arrive in a single call if `BUFFER_SIZE` is
  large.

`get_next_line` must therefore keep, from one call to the next, whatever was
read too far ahead and already belongs to the following line. That's the
core job of the function: making some state persist across successive
calls.

## Architecture choice: a static string

A static `char *rest` variable keeps,
between two calls to `get_next_line`, the "leftover": everything already
read from the file but not yet returned as a complete line. On each call:

- `rest` is topped up by reading in chunks of `BUFFER_SIZE` bytes as long as
  no `\n` is found and the end of the file hasn't been reached,
- the line to return is extracted (up to and including the first `\n`, or up
  to the end if the file doesn't end with a newline),
- `rest` is rebuilt from whatever was left after that `\n`, ready for the
  next call
The "problem" is that `rest` gets reallocated on every read (via
`join_rest`), which costs some memory copying but stays well within the
performance expectations for this project.

## How it works, function by function

### `get_next_line` (`get_next_line.c`)

The main entry point. It relies on a `static char *rest` variable that
survives from one call to the next:

1. Calls `read_and_store` to top up `rest` until a `\n` is found or the end
   of the file is reached.
2. If the read fails, or `rest` ends up empty or `NULL`, it frees `rest`,
   resets it to `NULL` and returns `NULL` (end of file or error).
3. Otherwise, it extracts the line to return with `extract_line`,
   recalculates the new leftover with `extract_rest`, frees the old `rest`,
   and returns the line.

### `read_and_store` (static, `get_next_line.c`)

The reading loop: as long as no `\n` is found in `rest`, it reads
`BUFFER_SIZE` bytes into a local buffer and appends that buffer to `rest`
via `join_rest`. It stops in three cases: a read error (`read` returns
`-1`), end of file (`read` returns `0`), or a `\n` is found in `rest`.

### `find_new_line` (`get_next_line_utils.c`)

Scans a string and returns a pointer to the first `\n` found, or `NULL` if
there isn't one. Used by `read_and_store` to know when to stop reading.

### `extract_line` (`get_next_line_utils.c`)

Splits off and allocates the line to return: copies every character from
`rest` up to and including the first `\n` (if there is one). This is what
naturally handles the case of a file that doesn't end with a newline: the
loop simply stops at the string's terminating `\0`, and no `\n` is ever
appended.

### `extract_rest` (`get_next_line_utils.c`)

Computes the new leftover: everything after the first `\n` in `rest`. If no
`\n` is found (the string was entirely consumed by `extract_line`), it
returns `NULL` rather than an empty string, avoiding an unnecessary `free`
and a potential leak on the next call.

### `join_rest` and `copy_str` (`get_next_line_utils.c`)

`join_rest` concatenates the existing leftover with the freshly-read buffer
into a newly allocated string, freeing the old leftover in the process.
`copy_str` is an internal helper that handles the character-by-character
copy, including when the source is `NULL` (first call, `rest` still empty).

## Bonus: handling several file descriptors

The subject's bonus part asks for `get_next_line` to work correctly even
when reading several files (or file descriptors) in parallel, without
their reads interfering with one another — while still only being allowed a
single static variable to manage everything.

### The problem

With a single static `char *rest`, as in the mandatory version, only one
leftover can be kept at a time. Alternating reads between two different
files would have each call overwrite the other file's leftover.

### The solution: a linked list of leftovers, indexed by fd

The bonus version (`get_next_line_bonus.c`, `get_next_line_bonus.h`,
`get_next_line_utils_bonus.c`) replaces the single `rest` variable with one
static `t_gnl_list *stock` variable: a linked list where each node
associates a file descriptor (`fd`) with its own leftover (`rest`).

```c
typedef struct s_gnl_list
{
	int					fd;
	char				*rest;
	struct s_gnl_list	*next;
}	t_gnl_list;
```

On each call to `get_next_line`:

1. `get_stock` looks through the list for the node matching the current
   `fd`. If none exists yet, it creates a new one (with `rest` initialized
   to `NULL`) and adds it to the head of the list.
2. Reading and line extraction happen exactly as in the mandatory version,
   but reading from and writing to `current->rest` (the leftover specific
   to that `fd`) instead of a single global variable.
3. Once the end of the file is reached (or on error), `delete_node` removes
   the corresponding node from the list and frees its memory: a descriptor
   leaves nothing behind once it has been fully read.

This design respects the single-static-variable constraint while fully
isolating each file descriptor's state: alternating reads across several
files causes no interference between their respective lines.

## Memory management

A particular point of care on this project, due to the use of a static
variable: it survives past the function call but can end up pointing to
memory that's already been freed if you're not careful (use-after-free).
Two rules were applied consistently throughout:

- every `free` of a pointer that still exists afterwards (the static
  variable, or a leftover kept inside a list node) is immediately followed
  by resetting it to `NULL`,
- at end of file, if the remaining leftover is an empty string (`""`)
  rather than truly `NULL`, it's still freed before returning `NULL` to the
  caller, to avoid a memory leak.

The project compiles without warnings under `-Wall -Wextra -Werror`, and has
been checked with Valgrind (zero memory errors, including on files
containing an empty line in the middle, with `BUFFER_SIZE=1`).

## Project files

| File | Role |
|---|---|
| `get_next_line.h` | Prototypes and `BUFFER_SIZE` macro (mandatory version) |
| `get_next_line.c` | `get_next_line` and `read_and_store` (mandatory version) |
| `get_next_line_utils.c` | Helper functions: `find_new_line`, `extract_line`, `extract_rest`, `join_rest`, `copy_str` |
| `get_next_line_bonus.h` | Prototypes, `BUFFER_SIZE` macro and the `t_gnl_list` structure (bonus version) |
| `get_next_line_bonus.c` | `get_next_line`, `get_stock`, `delete_node`, `read_and_store` (bonus version, multi-fd) |
| `get_next_line_utils_bonus.c` | Same helpers as the mandatory version, reused by the bonus |

## Ressources

https://www.youtube.com/watch?v=MCIwn7mY4jY : explicative video on the linked lists for beginners

https://www.youtube.com/watch?v=928Xhm_jByo : explicative video by the same creator (Bluefever Software)

https://www.youtube.com/watch?v=OngGUoENgWo : explicative video on the static variable

### On AI
For this project, we used the help of Claude Code and ChatGPT to give this project a clean structure. 
It also helped the understanding of the concept of static variable and to help with the debugging.
