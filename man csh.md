CSH(1)                             BSD General Commands Manual                             CSH(1)

NAME
     csh — a shell (command interpreter) with C-like syntax

SYNOPSIS
     csh [-bcefimnstVvXx] [argument ...]
     csh [-l]

DESCRIPTION
     csh is a command language interpreter incorporating a history mechanism (see History
     substitutions), job control facilities (see Jobs), interactive file name and user name com‐
     pletion (see File name completion), and a C-like syntax.  It is used both as an interactive
     login shell and a shell script command processor.

Argument list processing
If the first argument (argument 0) to the shell is a dash (‘-’), then this is a login shell.
A login shell also can be specified by invoking the shell with the -l flag as the only argu‐
ment.

     The rest of the flag arguments are interpreted as follows:
    
     -b     This flag forces a “break” from option processing, causing any further shell argu‐
            ments to be treated as non-option arguments.  The remaining arguments will not be in‐
            terpreted as shell options.  This may be used to pass options to a shell script with‐
            out confusion or possible subterfuge.  The shell will not run a set-user-ID script
            without this option.
    
     -c     Commands are read from the (single) following argument which must be present.  Any
            remaining arguments are placed in argv.
    
     -e     The shell exits if any invoked command terminates abnormally or yields a non-zero
            exit status.
    
     -f     The shell will start faster, because it will neither search for nor execute commands
            from the file .cshrc in the invoker's home directory.  Note:   if the environment vari‐
            able HOME is not set, fast startup is the default.
    
     -i     The shell is interactive and prompts for its top-level input, even if it appears not
            to be a terminal.  Shells are interactive without this option if their inputs and
            outputs are terminals.
    
     -l     The shell is a login shell (only applicable if -l is the only flag specified).
    
     -m     Read .cshrc, regardless of its owner and group.  This option is dangerous and should
            only be used by su(1).
    
     -n     Commands are parsed, but not executed.  This aids in syntactic checking of shell
            scripts.  When used interactively, the shell can be terminated by pressing control-D
            (end-of-file character), since exit will not work.
    
     -s     Command input is taken from the standard input.
    
     -t     A single line of input is read and executed.  A backslash (‘\’) may be used to escape
            the newline at the end of this line and continue onto another line.
    
     -V     Causes the verbose variable to be set even before .cshrc is executed.
    
     -v     Causes the verbose variable to be set, with the effect that command input is echoed
            after history substitution.
    
     -X     Causes the echo variable to be set even before .cshrc is executed.
    
     -x     Causes the echo variable to be set, so that commands are echoed immediately before
            execution.
    
     After processing of flag arguments, if arguments remain but none of the -c, -i, -s, or -t
     options were given, the first argument is taken as the name of a file of commands to be exe‐
     cuted.  The shell opens this file, and saves its name for possible resubstitution by ‘$0’.
     Since many systems use either the standard version 6 or version 7 shells whose shell scripts
     are not compatible with this shell, the shell will execute such a “standard” shell if the
     first character of a script is not a hash mark (‘#’); i.e., if the script does not start
     with a comment.  Remaining arguments initialize the variable argv.
    
     An instance of csh begins by executing commands from the file /etc/csh.cshrc and, if this is
     a login shell, /etc/csh.login.  It then executes commands from .cshrc in the home directory
     of the invoker, and, if this is a login shell, the file .login in the same location.  It is
     typical for users on CRTs to put the command stty crt in their .login file, and to also in‐
     voke tset(1) there.
    
     In the normal case, the shell will begin reading commands from the terminal, prompting with
     ‘% .’ Processing of arguments and the use of the shell to process files containing command
     scripts will be described later.
    
     The shell repeatedly performs the following actions: a line of command input is read and
     broken into “words”.  This sequence of words is placed on the command history list and
     parsed.  Finally each command in the current line is executed.
    
     When a login shell terminates it executes commands from the files .logout in the user's home
     directory and /etc/csh.logout.

   Lexical structure
     The shell splits input lines into words at blanks and tabs with the following exceptions.
     The characters ‘&’, ‘|’, ‘;’, ‘<’, ‘>’, ‘(’, and ‘)’ form separate words.  If doubled in
     ‘&&’, ‘||’, ‘<<’, or ‘>>’, these pairs form single words.  These parser metacharacters may
     be made part of other words, or have their special meaning prevented, by preceding them with
     a backslash (‘\’).  A newline preceded by a ‘\’ is equivalent to a blank.

     Strings enclosed in matched pairs of quotations, ‘'’, ‘`’, or ‘"’, form parts of a word;
     metacharacters in these strings, including blanks and tabs, do not form separate words.
     These quotations have semantics to be described later.  Within pairs of ‘'’ or ‘"’ charac‐
     ters, a newline preceded by a ‘\’ gives a true newline character.
    
     When the shell's input is not a terminal, the character ‘#’ introduces a comment that con‐
     tinues to the end of the input line.  This special meaning is prevented when preceded by ‘\’
     and in quotations using ‘`’, ‘'’, and ‘"’.

   Commands
     A simple command is a sequence of words, the first of which specifies the command to be exe‐
     cuted.  A simple command or a sequence of simple commands separated by ‘|’ characters forms
     a pipeline.  The output of each command in a pipeline is connected to the input of the next.
     Sequences of pipelines may be separated by ‘;’, and are then executed sequentially.  A se‐
     quence of pipelines may be executed without immediately waiting for it to terminate by fol‐
     lowing it with a ‘&’.