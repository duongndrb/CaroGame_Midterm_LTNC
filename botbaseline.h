#ifndef BOTBASELINE
#define BOTBASELINE

#include <iostream>
#include <stdlib.h>
#include "config.h"
#include <stdbool.h>
#include <algorithm>
//mảng điểm tấn công
int MangDiemTanCong[7] = {0, 9, 54, 162, 1458, 13112, 118008};
//mảng điểm phòng ngự
int MangDiemPhongThu[7] = {0, 3, 27, 99, 729, 6561, 59049 };

Point check_win(int board_game[][WIDTH], int player_id);
Point defend(int board_game[][WIDTH], int player_id);
Point attack(int board_game[][WIDTH], int player_id);
Point check_n_tile(int board_game[][WIDTH], int player_id, int n);
long DiemTanCong_DuyetDoc(int board_game[][WIDTH],int curDong, int curCot,int player_id);
long DiemTanCong_DuyetNgang(int board_game[][WIDTH],int curDong, int curCot,int player_id);
long DiemTanCong_DuyetCheoXuoi(int board_game[][WIDTH],int curDong, int curCot,int player_id);
long DiemTanCong_DuyetCheoNguoc(int board_game[][WIDTH],int curDong, int curCot,int player_id);
long DiemPhongNgu_DuyetDoc(int board_game[][WIDTH],int curDong, int curCot,int player_id);
long DiemPhongNgu_DuyetNgang(int board_game[][WIDTH],int curDong, int curCot,int player_id);
long DiemPhongNgu_DuyetCheoXuoi(int board_game[][WIDTH],int curDong, int curCot,int player_id);
long DiemPhongNgu_DuyetCheoNguoc(int board_game[][WIDTH],int curDong, int curCot,int player_id);

// player_id = 1 || -1
Point player_rand(int board_game[][WIDTH], int player_id){
    int row, col;
    row = rand() % HEIGHT;
    col = rand() % WIDTH;
    return Point(row, col);
}

Point player_baseline(int board_game[][WIDTH], int player_id){
    Point p = check_win(board_game, player_id);
    if(p.x != -1 && p.y != -1){
        return p;
    } else {
        p = defend(board_game, player_id);
        if(p.x != -1 && p.y != -1){
            return p;
        } else {
            return attack(board_game, player_id);
        }
    }
}


Point check_win(int board_game[][WIDTH], int player_id){
    return check_n_tile(board_game, player_id, 4);
}

Point defend(int board_game[][WIDTH], int player_id){
//    Pointp p = check_n_tile(board_game, -player_id, 4);
//    if(p.x != -1 || p.y != -1) return p;
//    else {
//        p = check_n_tile(board_game, -player_id, 3);
//        if(p.x != -1 || p.y != -1) return p;
//    }
//    return Point(-1, -1);
    return check_n_tile(board_game, -player_id,4);
}

Point attack(int board_game[][WIDTH], int player_id){
    Point p =  Point(HEIGHT/2,WIDTH/3);
    long diemMax = 0;
    for (int i = 0; i < HEIGHT; i++){
             for (int j = 0; j <WIDTH; j++){
                    // chỉ duyệt những ô chưa đánh
                    // tìm tổng điềm tấn công và phòng thủ
                    if (board_game[i][j] == 0)
                    {
                        long diemTanCong = DiemTanCong_DuyetDoc(board_game,i, j,player_id) + DiemTanCong_DuyetNgang(board_game,i, j,player_id) + DiemTanCong_DuyetCheoXuoi(board_game,i, j,player_id) + DiemTanCong_DuyetCheoNguoc(board_game,i, j,player_id);
                        long diemPhongThu = DiemPhongNgu_DuyetDoc(board_game,i, j,player_id) + DiemPhongNgu_DuyetNgang(board_game,i, j,player_id) + DiemPhongNgu_DuyetCheoXuoi(board_game,i, j,player_id) + DiemPhongNgu_DuyetCheoNguoc(board_game,i, j,player_id);
                        // Lấy điểm tạm bằng cách so sánh điểm tấn công, phòng thủ
                        long diemTam = diemTanCong > diemPhongThu ? diemTanCong : diemPhongThu;
                        // tìm điểm max
                        if (diemMax < diemTam)
                        {
                            diemMax = diemTam;
                            p.x=i;
                            p.y=j;
                        }
                    }
                }
            }

            return p;
}


Point check_n_tile(int board_game[][WIDTH], int player_id, int n){
    int check_6h = 1, check_3h = 1, check_5h = 1, check_1h = 1;
    Point temp(-1, -1);
    Point posible_moves[8];
    int p_moves = 0;
    for(int i=0; i < HEIGHT; i++){
        for(int j=0; j < WIDTH; j++){
            if(board_game[i][j] != player_id) continue;

            check_6h = 1, check_3h = 1, check_5h = 1, check_1h = 1;
            for(int k = 1; k < n; k++){
                if(board_game[i][j] == board_game[i+k][j]) check_6h++;
                if(board_game[i][j] == board_game[i][j+k]) check_3h++;
                if(board_game[i][j] == board_game[i+k][j+k]) check_5h++;
                if(board_game[i][j] == board_game[i-k][j+k]) check_1h++;
            }

            if(check_6h == n){
                if(n == 3){
                    if(board_game[i-1][j] == 0 && board_game[i+n][j] == 0) return Point(i-1, j);
                }
                if(board_game[i-1][j] == 0) {
                    posible_moves[p_moves] = Point(i-1, j);
                    p_moves++;
                }
                if(board_game[i+n][j] == 0) {
                    posible_moves[p_moves] = Point(i+n, j);
                    p_moves++;
                }

            }
            if(check_3h == n){
                if(n == 3){
                    if(board_game[i][j-1] == 0 && board_game[i][j+n] == 0) return Point(i, j-1);
                }

                if(board_game[i][j-1] == 0) {
                    posible_moves[p_moves] = Point(i, j-1);
                    p_moves++;
                }
                if(board_game[i][j+n] == 0) {
                    posible_moves[p_moves] = Point(i, j+n);
                    p_moves++;
                }

            }
            if(check_5h == n){
                if(n == 3){
                    if(board_game[i-1][j-1] == 0 && board_game[i+n][j+n] == 0) return Point(i-1, j-1);
                }

                if(board_game[i-1][j-1] == 0) {
                    posible_moves[p_moves] = Point(i-1, j-1);
                    p_moves++;
                }
                if(board_game[i+n][j+n] == 0) {
                    posible_moves[p_moves] = Point(i+n, j+n);
                    p_moves++;
                }

            }
            if(check_1h == n){
                if(n == 3){
                    if(board_game[i+1][j-1] == 0 && board_game[i-n][j+n] == 0) return Point(i+1, j-1);
                }
                if(board_game[i+1][j-1] == 0) {
                    posible_moves[p_moves] = Point(i+1, j-1);
                    p_moves++;
                }
                if(board_game[i-n][j+n] == 0) {
                    posible_moves[p_moves] = Point(i-n, j+n);
                    p_moves++;
                }

            }

            if(p_moves > 0){
                return posible_moves[rand()%p_moves];
            }
        }
    }
    return Point(-1, -1);
}

long DiemTanCong_DuyetDoc(int board_game[][WIDTH],int curDong, int curCot,int player_id)
        {
            long DiemTemp = 0;
            long diemTong = 0;
            int soQuanTa = 0; // máy
            int soQuanDich = 0; // người

            // duyệt từ dòng trên xuống dưới
            // duyệt 5 con để biết ô tiếp theo(thứ 6) bị chặn thì ta xử lí phù hợp
            for (int dem = 1; dem < 6 && curDong + dem < HEIGHT; dem++)
            {


                if (board_game[curDong + dem][curCot] == player_id)
                    soQuanTa++;
                else if (board_game[curDong + dem] [curCot] == -player_id) // quân địch(player 2)
                {
                    soQuanDich++;
                   // DiemTemp -= 9;
                    break; // nếu gặp quân địch(bị chặn) => thoát vòng lặp
                }
                else // nếu gặp ô trống => thoát
                {
                    break;
                }
            }

            // duyệt từ dòng dưới ngược lên trên
            for (int dem = 1; dem < 6 && curDong - dem >= 0; dem++)
            {

                if (board_game[curDong - dem][curCot] == player_id)
                    soQuanTa++;
                else if (board_game[curDong - dem ][curCot] == -player_id) // quân địch(player 2)
                {
                   // DiemTemp -= 9;
                    soQuanDich++;
                    break; // nếu gặp quân địch(bị chặn) => thoát vòng lặp
                }
                else // nếu gặp ô trống => thoát
                {
                    break;
                }
            }

            // nếu bị chặn cả 2 đầu thì nước đang xét sẽ không còn giá trị nữa
            if (soQuanDich == 2)
                return 0;
            // Giảm diemTong dựa trên số quân địch
            // Do số quân địch tối đa chỉ có thể là 1, nên ta +1 để tăng số điểm bị trừ
            diemTong -= MangDiemPhongThu[soQuanDich];
            // tăng diemTong dựa trên số quân ta
            diemTong += MangDiemTanCong[soQuanTa];
           // diemTong += DiemTemp;
             if (board_game[curDong-1][curCot] == player_id&&board_game[curDong+1][curCot] == player_id&&board_game[curDong][curCot] == 0&&board_game[curDong+2][curCot] == player_id){
                diemTong+= MangDiemTanCong[5];
             }
             if (board_game[curDong+1][curCot] == player_id&&board_game[curDong-1][curCot] == player_id&&board_game[curDong][curCot] == 0&&board_game[curDong-2][curCot] == player_id){
                diemTong+= MangDiemTanCong[5];
             }

            return diemTong;
        }
        // Duyệt Ngang
long DiemTanCong_DuyetNgang(int board_game[][WIDTH],int curDong, int curCot,int player_id)
        {
            long DiemTemp = 0;
            long diemTong = 0;
            int soQuanTa = 0;
            int soQuanDich = 0;

            // duyệt từ dòng trên xuống dưới
            // duyệt 5 con để biết ô tiếp theo(thứ 6) bị chặn thì ta xử lí phù hợp
            for (int dem = 1; dem < 6 && curCot + dem < WIDTH; dem++)
            {


                if (board_game[curDong][curCot + dem] == player_id)
                    soQuanTa++;
                else if (board_game[curDong][curCot + dem] == -player_id)
                {
                   // DiemTemp -= 9;
                    soQuanDich++;
                    break; // nếu gặp quân địch(bị chặn) => thoát vòng lặp
                }
                else // nếu gặp ô trống => thoát
                {
                    break;
                }
            }

            // duyệt từ dòng dưới ngược lên trên
            for (int dem = 1; dem < 6 && curCot - dem >= 0; dem++)
            {

                if (board_game[curDong][curCot - dem] == player_id)
                    soQuanTa++;
                else if (board_game[curDong][curCot - dem] == -player_id) // quân địch(player 2)
                {
                  //  DiemTemp -= 9;
                    soQuanDich++;
                    break; // nếu gặp quân địch(bị chặn) => thoát vòng lặp
                }
                else // nếu gặp ô trống => thoát
                {
                    break;
                }
            }

            // nếu bị chặn cả 2 đầu thì nước đang xét sẽ không còn giá trị nữa
            if (soQuanDich == 2)
                return 0;
            // Giảm diemTong dựa trên số quân địch
            // Do số quân địch tối đa chỉ có thể là 1, nên ta +1 để tăng số điểm bị trừ
            diemTong -= MangDiemPhongThu[soQuanDich];
            // tăng diemTong dựa trên số quân ta
            diemTong += MangDiemTanCong[soQuanTa];
           // diemTong += DiemTemp;
           if (board_game[curDong][curCot-1] == player_id&&board_game[curDong][curCot+1] == player_id&&board_game[curDong][curCot] == 0&&board_game[curDong][curCot+2] == player_id){
                diemTong+= MangDiemTanCong[5];
             }
             if (board_game[curDong][curCot+1] == player_id&&board_game[curDong][curCot-1] == player_id&&board_game[curDong][curCot] == 0&&board_game[curDong][curCot-2] == player_id){
                diemTong+= MangDiemTanCong[5];
             }

            return diemTong;
        }
        // Chéo xuôi
long DiemTanCong_DuyetCheoXuoi(int board_game[][WIDTH],int curDong, int curCot,int player_id)
        {
            long DiemTemp = 0;
            long diemTong = 0;
            int soQuanTa = 0; // máy
            int soQuanDich = 0; // người

            // duyệt từ dòng trên xuống dưới
            // duyệt 5 con để biết ô tiếp theo(thứ 6) bị chặn thì ta xử lí phù hợp
            for (int dem = 1; dem < 6 && curDong + dem < HEIGHT && curCot + dem < WIDTH; dem++)
            {


                if (board_game[curDong + dem][curCot + dem] == player_id)
                    soQuanTa++;
                else if (board_game[curDong + dem][curCot + dem] == player_id) // quân địch(player 2)
                {
                   // DiemTemp -= 9;
                    soQuanDich++;
                    break; // nếu gặp quân địch(bị chặn) => thoát vòng lặp
                }
                else // nếu gặp ô trống => thoát
                {
                    break;
                }
            }

            // duyệt từ dòng dưới ngược lên trên
            for (int dem = 1; dem < 6 && curDong - dem >= 0 && curCot - dem >= 0; dem++)
            {

                if (board_game[curDong - dem][curCot - dem] == player_id)
                    soQuanTa++;
                else if (board_game[curDong - dem][curCot - dem] == -player_id)
                {
                   // DiemTemp -= 9;
                    soQuanDich++;
                    break; // nếu gặp quân địch(bị chặn) => thoát vòng lặp
                }
                else // nếu gặp ô trống => thoát
                {
                    break;
                }
            }

            // nếu bị chặn cả 2 đầu thì nước đang xét sẽ không còn giá trị nữa
            if (soQuanDich == 2)
                return 0;
            // Giảm diemTong dựa trên số quân địch
            // Do số quân địch tối đa chỉ có thể là 1, nên ta +1 để tăng số điểm bị trừ
            diemTong -= MangDiemPhongThu[soQuanDich] ;
            // tăng diemTong dựa trên số quân ta
            diemTong += MangDiemTanCong[soQuanTa];
           // diemTong += DiemTemp;
            if (board_game[curDong-1][curCot-1] == player_id&&board_game[curDong+2][curCot+2] == player_id&&board_game[curDong][curCot] == 0&&board_game[curDong+1][curCot+1] == player_id){
                diemTong+= MangDiemTanCong[5];
             }
             if (board_game[curDong+1][curCot+1] == player_id&&board_game[curDong-2][curCot-2] == player_id&&board_game[curDong][curCot] == 0&&board_game[curDong-1][curCot-1] == player_id){
                diemTong+= MangDiemTanCong[5];
             }
            return diemTong;
        }
        // Duyệt chéo ngược
long DiemTanCong_DuyetCheoNguoc(int board_game[][WIDTH],int curDong, int curCot,int player_id)
        {
            long DiemTemp = 0;
            long diemTong = 0;
            int soQuanTa = 0;
            int soQuanDich = 0;

            // duyệt từ dòng trên xuống dưới
            // duyệt 5 con để biết ô tiếp theo(thứ 6) bị chặn thì ta xử lí phù hợp         // đang duyệt chéo ngược lại lên trên
            for (int dem = 1; dem < 6 && curCot + dem < WIDTH && curDong - dem >= 0; dem++)
            {


                if (board_game[curDong - dem][curCot + dem] == player_id)
                    soQuanTa++;
                else if (board_game[curDong - dem][curCot + dem] == -player_id)
                {
                 //   DiemTemp -= 9;
                    soQuanDich++;
                    break; // nếu gặp quân địch(bị chặn) => thoát vòng lặp
                }
                else // nếu gặp ô trống => thoát
                {
                    break;
                }
            }

            // duyệt từ trên chéo ngược xuống dưới
            for (int dem = 1; dem < 6 && curCot - dem >= 0 && curDong + dem < HEIGHT; dem++)
            {


                if (board_game[curDong + dem][curCot - dem] == player_id)
                    soQuanTa++;
                else if (board_game[curDong + dem][curCot - dem] == -player_id) // quân địch(player 2)
                {
                   // DiemTemp -= 9;
                    soQuanDich++;
                    break; // nếu gặp quân địch(bị chặn) => thoát vòng lặp
                }
                else // nếu gặp ô trống => thoát
                {
                    break;
                }
            }

            // nếu bị chặn cả 2 đầu thì nước đang xét sẽ không còn giá trị nữa
            if (soQuanDich == 2)
                return 0;
            // Giảm diemTong dựa trên số quân địch
            // Do số quân địch tối đa chỉ có thể là 1, nên ta +1 để tăng số điểm bị trừ
            diemTong -= MangDiemPhongThu[soQuanDich];
            // tăng diemTong dựa trên số quân ta
            diemTong += MangDiemTanCong[soQuanTa];
           // diemTong += DiemTemp;
           if (board_game[curDong-1][curCot+1] == player_id&&board_game[curDong+1][curCot-1] == player_id&&board_game[curDong][curCot] == 0&&board_game[curDong+2][curCot-2] == player_id){
                diemTong+= MangDiemTanCong[5];
             }
             if (board_game[curDong+1][curCot-1] == player_id&&board_game[curDong-1][curCot+1] == player_id&&board_game[curDong][curCot] == 0&&board_game[curDong-2][curCot+2] == player_id){
                diemTong+= MangDiemTanCong[5];
             }

            return diemTong;
        }
long DiemPhongNgu_DuyetDoc(int board_game[][WIDTH],int curDong, int curCot,int player_id)
        {
            long DiemTemp = 0;
            long diemTong = 0;
            int soQuanTa = 0;
            int soQuanDich = 0;

            // duyệt từ dòng trên xuống dưới
            // duyệt 5 con để biết ô tiếp theo(thứ 6) bị chặn thì ta xử lí phù hợp
            for (int dem = 1; dem < 6 && curDong + dem < HEIGHT; dem++)
            {


                if (board_game[curDong + dem][curCot] == player_id)
                {
                    soQuanTa++;
                    break;
                }
                else if (board_game[curDong + dem][curCot] == -player_id) // quân địch(player 2)
                {
                    soQuanDich++;
                }
                else // nếu gặp ô trống => thoát
                {
                    break;
                }
            }

            // duyệt từ dòng dưới ngược lên trên
            for (int dem = 1; dem < 6 && curDong - dem >= WIDTH; dem++)
            {
                // Do Player1 luôn là máy(quân ta) đánh nên SoHuu = 1
                if (board_game[curDong - dem][curCot]== player_id)
                {
                    soQuanTa++;
                    break;
                }
                else if (board_game[curDong - dem][curCot]== -player_id)
                    // quân địch(player 2)
                {
                    soQuanDich++;
                }
                else // nếu gặp ô trống => thoát
                {
                    break;
                }
            }

            // quân ta đã chặn 2 đầu, nên k cần xét nữa
            if (soQuanTa == 2)
                return 0;
            //
            //

            // tăng điểm phòng ngự dựa trên số quân địch
            diemTong += MangDiemPhongThu[soQuanDich];
//            diemTong -= MangTanCong[soQuanTa];
            if (soQuanDich > 0)
            {
                diemTong -= MangDiemTanCong[soQuanTa] ;
            }
            if(soQuanDich==3&&soQuanTa==0){
                diemTong+=MangDiemPhongThu[5];
            }
              if (board_game[curDong-1][curCot] == -player_id&&board_game[curDong+1][curCot] == -player_id&&board_game[curDong][curCot] == 0&&board_game[curDong+2][curCot] == -player_id){
                diemTong+= MangDiemPhongThu[5];
             }
             if (board_game[curDong+1][curCot] == -player_id&&board_game[curDong-1][curCot] == -player_id&&board_game[curDong][curCot] == 0&&board_game[curDong-2][curCot] == -player_id){
                diemTong+= MangDiemPhongThu[5];
             }

            return diemTong;
        }
        // Duyệt Ngang
long DiemPhongNgu_DuyetNgang(int board_game[][WIDTH],int curDong, int curCot,int player_id)
        {
            long diemTong = 0;
            int soQuanTa = 0; // máy
            int soQuanDich = 0; // người

            // duyệt từ dòng trên xuống dưới
            // duyệt 5 con để biết ô tiếp theo(thứ 6) bị chặn thì ta xử lí phù hợp
            for (int dem = 1; dem < 6 && curCot + dem < HEIGHT; dem++)
            {


                if (board_game[curDong][curCot + dem] == player_id)
                {
                    soQuanTa++;
                    break;
                }
                else if (board_game[curDong][curCot + dem] == -player_id) // quân địch(player 2)
                {
                    soQuanDich++;
                }
                else // nếu gặp ô trống => thoát
                {
                    break;
                }
            }

            // duyệt từ dòng dưới ngược lên trên
            for (int dem = 1; dem < 6 && curCot - dem >= 0; dem++)
            {

                if (board_game[curDong][curCot - dem] == player_id)
                {
                    soQuanTa++;
                    break;
                }
                else if (board_game[curDong][curCot - dem] == -player_id) // quân địch(player 2)
                {
                    soQuanDich++;
                }
                else // nếu gặp ô trống => thoát
                {
                    break;
                }
            }

            if (soQuanTa == 2)
                return 0;


            // tăng điểm phòng ngự
            diemTong += MangDiemPhongThu[soQuanDich];
            if (soQuanDich > 0)
            {
                diemTong -= MangDiemTanCong[soQuanTa] ;
            }
            if(soQuanDich==3&&soQuanTa==0){
                diemTong+=MangDiemPhongThu[5];
            }
            if (board_game[curDong][curCot-1] == -player_id&&board_game[curDong][curCot+1] ==- player_id&&board_game[curDong][curCot] == 0&&board_game[curDong][curCot+2] == -player_id){
                diemTong+= MangDiemPhongThu[5];
             }
             if (board_game[curDong][curCot+1] ==- player_id&&board_game[curDong][curCot-1] == -player_id&&board_game[curDong][curCot] == 0&&board_game[curDong][curCot-2] == -player_id){
                diemTong+= MangDiemPhongThu[5];
             }


            return diemTong;
        }
        // Chéo xuôi
long DiemPhongNgu_DuyetCheoXuoi(int board_game[][WIDTH],int curDong, int curCot,int player_id)
        {
            long diemTong = 0;
            int soQuanTa = 0;
            int soQuanDich = 0;

            // duyệt từ dòng trên xuống dưới
            // duyệt 5 con để biết ô tiếp theo(thứ 6) bị chặn thì ta xử lí phù hợp
            for (int dem = 1; dem < 6 && curDong + dem < HEIGHT && curCot + dem < WIDTH; dem++)
            {


                if (board_game[curDong + dem][curCot + dem]== player_id)
                {
                    soQuanTa++;
                    break;
                }
                else if (board_game[curDong + dem][curCot + dem] == -player_id) // quân địch(player 2)
                {
                    soQuanDich++;
                }
                else // nếu gặp ô trống => thoát
                {
                    break;
                }
            }

            // duyệt từ dòng dưới ngược lên trên
            for (int dem = 1; dem < 6 && curDong - dem >= 0 && curCot - dem >= 0; dem++)
            {

                if (board_game[curDong - dem][curCot - dem] == player_id)
                {
                    soQuanTa++;
                    break;
                }
                else if (board_game[curDong - dem][curCot - dem] == player_id) // quân địch(player 2)
                {
                    soQuanDich++;
                }
                else // nếu gặp ô trống => thoát
                {
                    break;
                }
            }


            if (soQuanTa == 2)
                return 0;
            // tăng diemTong dựa trên số quân ta
            diemTong += MangDiemPhongThu[soQuanDich];
            if (soQuanDich > 0)
            {
                diemTong -= MangDiemTanCong[soQuanTa] ;
            }
            if(soQuanDich==3&&soQuanTa==0){
                diemTong+=MangDiemPhongThu[5];
            }
            if (board_game[curDong-1][curCot-1] == -player_id&&board_game[curDong+2][curCot+2] == -player_id&&board_game[curDong][curCot] == 0&&board_game[curDong+1][curCot+1] == -player_id){
                diemTong+= MangDiemPhongThu[5];
             }
             if (board_game[curDong+1][curCot+1] == -player_id&&board_game[curDong-2][curCot-2] == -player_id&&board_game[curDong][curCot] == 0&&board_game[curDong-1][curCot-1] == -player_id){
                diemTong+= MangDiemPhongThu[5];
             }


            return diemTong;
        }
        // Duyệt chéo ngược
long DiemPhongNgu_DuyetCheoNguoc(int board_game[][WIDTH],int curDong, int curCot,int player_id)
        {
            long diemTong = 0;
            int soQuanTa = 0;
            int soQuanDich = 0;

            // duyệt từ dòng trên xuống dưới
            // duyệt 5 con để biết ô tiếp theo(thứ 6) bị chặn thì ta xử lí phù hợp         // đang duyệt chéo ngược lại lên trên
            for (int dem = 1; dem < 6 && curCot + dem < WIDTH && curDong - dem >= 0; dem++)
            {


                if (board_game[curDong - dem][curCot + dem] == player_id)
                {
                    soQuanTa++;
                    break;
                }
                else if (board_game[curDong - dem][curCot + dem] == -player_id) // quân địch(player 2)
                {
                    soQuanDich++;
                }
                else // nếu gặp ô trống => thoát
                {
                    break;
                }
            }

            // duyệt từ trên chéo ngược xuống dưới
            for (int dem = 1; dem < 6 && curCot - dem >= 0 && curDong + dem < HEIGHT; dem++)
            {

                if (board_game[curDong + dem][curCot - dem] == player_id)
                {
                    soQuanTa++;
                    break;
                }
                else if (board_game[curDong + dem][curCot - dem] == -player_id) // quân địch(player 2)
                {
                    soQuanDich++;
                }
                else // nếu gặp ô trống => thoát
                {
                    break;
                }
            }


            if (soQuanTa == 2)
                return 0;

            diemTong += MangDiemPhongThu[soQuanDich];
            if (soQuanDich > 0)
            {
                diemTong -= MangDiemTanCong[soQuanTa] ;
            }
            if(soQuanDich==3 && soQuanTa==0){
                diemTong+=MangDiemPhongThu[5];
            }
            if (board_game[curDong-1][curCot+1] ==-player_id&&board_game[curDong+1][curCot-1] == -player_id&&board_game[curDong][curCot] == 0&&board_game[curDong+2][curCot-2] == -player_id){
                diemTong+= MangDiemPhongThu[5];
             }
             if (board_game[curDong+1][curCot-1] == -player_id&&board_game[curDong-1][curCot+1] == -player_id&&board_game[curDong][curCot] == 0&&board_game[curDong-2][curCot+2] == -player_id){
                diemTong+= MangDiemPhongThu[5];
             }


            return diemTong;
        }


#endif // BOTBASELINE
