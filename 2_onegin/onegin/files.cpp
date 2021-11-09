#include "files.h"

void process_cmd_arg (char **f_in_name, char **f_out_name, int argc, char **argv) {

    assert (f_in_name != NULL);
    assert (f_out_name != NULL);
    assert (argv != NULL);

    Print_Debug ("Command line args are processing");

    int opt;
    while ( (opt = getopt (argc, argv, "hi:o:")) != -1) {
        switch (opt) {
            case 'h':
                Print_Debug ("Help mode on");
                printf ("\n\nUsage: -h -i <input_file_name> -o <output_file_name>\n\n");
                system ("pause");
                break;

                case 'i':
                    Print_Debug ("Name of input");
                    *f_in_name = optarg;
                    break;

                    case 'o':
                        Print_Debug ("Name of output");
                        *f_out_name = optarg;
                        break;

                        default:
                            printf ("No special arguments\n\n");
        }
    }
}

void open_files (FILE *in_f, FILE *out_f, char *in_name, char *out_name) {
//void open_files (Files* theFiles) {

   // assert (theFiles);
  /*//  assert (in_f != NULL);
  //  assert (out_f != NULL);
    assert (in_name != (char *) 1);
    assert (out_name != (char *) 1);*/

    PD_Value("in_f name is %s\n", in_name);
    if (in_f  = fopen (in_name, "rb")) {
        Print_Debug("input fie is opened");

       // PD_Value("#####first character is %c#####\n", fgetc(out_f));
    }

    else {
        Print_Debug("!!! failed to open input file");
    }
    if (out_f = fopen (out_name, "wb")) Print_Debug("output fie is opened");


    char c = ' ';

    while (c = fgetc(in_f) != EOF) {
        fprintf(out_f, "%c", c);
    }

    assert (in_f != NULL);
    assert (out_f != NULL);

    Print_Debug ("Files opened");
    PD_Value ("input file name  - %s\n", in_name);
    PD_Value ("output file name - %s\n\n", out_name);
}

unsigned long get_file_size_fstat (char *f_in_name) {

    assert (f_in_name != NULL);

    Print_Debug ("Get file size");
    struct stat file;
    assert (stat (f_in_name, &file) != -1);

    return file.st_size;
}

void Files::Ctor (int argc, char **argv) {

    Print_Debug ("File Ctor: start");

    process_cmd_arg (&this->f_in_name, &this->f_out_name, argc, argv);

    open_files (this->in_f, this->out_f, this->f_in_name, this->f_out_name);
  //  open_files (this);

 //   this->out_f = fopen (this->f_out_name, "w");
 //   assert (this->out_f != NULL);

    PD_Value ("In_file_d = %d, out_file_d = %d \n\n", this->in_fd, this->out_fd);

    this->fileSize = get_file_size_fstat (this->f_in_name);
    assert (this->fileSize > 0 && this->fileSize < ULONG_MAX);
    Print_Debug("file size returned");
    PD_Value ("file size: %lu\n\n", this->fileSize);
}

void Files::Dtor () {
    fclose(this->in_f);
    fclose(this->out_f);
}
