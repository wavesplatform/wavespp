au FileType,BufRead c,cpp setl cindent cinoptions=N-sp0t0s ts=4 sts=4 sw=4 fdm=marker fdm=marker cms=//%s

let g:ale_cpp_clang_options = '-std=c++14 -Wall -isystem /usr/include/c++/7 -isystem /usr/include/c++/7/backward -isystem /usr/include/mysqlslave -isystem /usr/local/include/waves -isystem /usr/local/include/coda -isystem /usr/local/include/blizzard -isystem /usr/include/mysql -isystem /usr/local/include -Isrc'
let g:ale_enabled = 0
