#include "kernel/types.h"
#include "user/user.h"

// Báo cho compiler biết: hàm này sẽ không return (nó sẽ exit() trong mọi nhánh)
void primes(int left_fd) __attribute__((noreturn));

void primes(int left_fd) {
  int p;

  // Đọc số đầu tiên từ pipe bên trái -> đây là prime tiếp theo
  if (read(left_fd, &p, sizeof(p)) == 0) {
    // Nếu không còn dữ liệu (EOF), đóng fd và thoát
    close(left_fd);
    exit(0);
  }

  // In prime ra (ghi trực tiếp vào fd=1)
  // Dùng fprintf(1, ...) giảm nguy cơ dữ liệu bị trộn do nhiều process ghi cùng stdout.
  fprintf(1, "prime %d\n", p);

  // Tạo pipe bên phải, để gửi số lọc sang tiến trình tiếp theo
  int right[2];
  if (pipe(right) < 0) {
    // Không đủ tài nguyên -> đóng và thoát
    close(left_fd);
    exit(0);
  }

  int pid = fork();
  if (pid < 0) {
    // fork lỗi
    close(left_fd);
    close(right[0]);
    close(right[1]);
    exit(0);
  }

  if (pid == 0) {
    // =========== child (bên phải) ===========
    // Child sẽ đọc từ right[0]
    // Đóng các fd không cần: child không cần đầu ghi bên phải và không cần left_fd.
    close(right[1]);
    close(left_fd);
    // Gọi đệ quy: child trở thành bộ lọc tiếp theo (và sẽ exit khi xong)
    primes(right[0]);
    // không bao giờ tới đây vì primes được mark noreturn
  } else {
    // =========== parent (current filter) ===========
    // Parent sẽ đọc từ left_fd, lọc bội số của p, và ghi vào right[1]
    close(right[0]); // đóng đầu đọc của pipe mới (không cần)
    int num;
    while (read(left_fd, &num, sizeof(num)) != 0) {
      if (num % p != 0) {
        write(right[1], &num, sizeof(num));
      }
    }

    close(left_fd);
    close(right[1]);

    wait(0);
    // Sau khi chờ xong, thoát
    exit(0);
  }
}

int
main(void)
{
  int p[2];

  if (pipe(p) < 0) {
    exit(0);
  }

  int pid = fork();
  if (pid < 0) {
    // fork lỗi
    close(p[0]);
    close(p[1]);
    exit(0);
  }

  if (pid == 0) {
    // Child: sẽ là đầu đọc pipeline (bắt đầu sàng)
    close(p[1]);    // child không ghi vào pipe đầu
    primes(p[0]);   // không return
  } else {
    // Parent: gửi các số 2..280 vào pipe đầu và chờ pipeline hoàn tất
    close(p[0]);    // parent chỉ ghi
    for (int i = 2; i <= 280; i++) {
      write(p[1], &i, sizeof(i));
    }
    close(p[1]); 

    
    wait(0);
    exit(0);
  }
}
