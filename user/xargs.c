#include "kernel/types.h"
#include "kernel/param.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  // argv[1..] chứa lệnh cần chạy (ví dụ: "echo bye")
  // stdin chứa các dòng sẽ thêm vào lệnh

  char buf[512]; // bộ đệm đọc dòng
  char *args[MAXARG]; // mảng chứa đối số khi gọi exec
  int n = 0; // chỉ số đối số
  int i = 0; // index trong buffer
  char c;

  // Copy các đối số ban đầu vào args
  for (int j = 1; j < argc; j++) {
    args[n++] = argv[j];
  }

  // Đọc stdin từng ký tự
  while (read(0, &c, 1) == 1) {
    if (c == '\n') {
      buf[i] = '\0'; // kết thúc chuỗi
      args[n] = buf; // thêm dòng đọc được vào args
      args[n+1] = 0; // kết thúc mảng argv bằng NULL

      if (fork() == 0) {
        exec(argv[1], args); // chạy command
        exit(1); // chỉ chạy khi exec lỗi
      } else {
        wait(0); // đợi child xong
      }

      i = 0; // reset bộ đệm
    } else {
      buf[i++] = c;
    }
  }

  exit(0);
}
