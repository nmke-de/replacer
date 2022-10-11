# Replacer

The **replacer** is a small program which replaces the string "`<!-- CONTENT -->`" in a template file with the compiled contents of another file.

## But why?

As part of restructuring my website (yet again), I decided to switch to Markdown to write my content. After taking a look at already existing options (modifying my [current website backbone](https://www.nmke.de/index.php), [Github Pages](https://github.com/pages), [Bearblog](https://bearblog.dev), [Smol Pub](http://smol.pub), [ssg](https://romanzolotarev.com/ssg.html), [Hugo](https://gohugo.io/)), I decided to restart from scratch (yet again) and to do it myself (if for nothing else than the thrill of it) (and also yet again).

The (planned) backbone works as follows: A (pre-existing) **compiler** (i.e. smu (Link incoming) or [md2html](https://github.com/md4c/md4c)) converts Markdown to HTML. The result of this is then pasted together by the **replacer** (that's this program). Finally, a **builder** (this may be a Makefile or a shell script) writes the output into the targets for each input file.

## Dependencies

You will need a compiler as described above. While this is intended to be a Markdown compiler, another program for another language (for example AsciiDoc or `text/gemini`) will work perfectly fine as long as the compiler prints its output (in a way that can be piped).

Other than that, you will need

- a libc, preferrably statically linked.

### Build dependencies

- a C compiler (*not* the kind of compiler as described above)
- Make (i.e. GNU Make)

## Build

Type `make`.

## Usage

`./replace [filename] [-t template_filename] [-c compiler]`

Currently, the replacer does not search the `PATH` for the compiler executable.
