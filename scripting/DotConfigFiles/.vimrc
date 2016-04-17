set autoindent
set smartindent
set hlsearch
set cindent
set expandtab
set shiftwidth=2
set softtabstop=2
set ic
set complete+=kspell
set spell spelllang=en_us
map Q :q
map N :n
se nospell
"This is to comment a vimrc file
set mouse=a
map <ScrollWheelUp> <C-Y>
map <ScrollWheelDown> <C-E>

":set formatoptions+=w
":set tw=80
map fa gggqG
map fp gqip
map fl gqq

"This is for llvm syntax highlighting
"copy `src/utils/vim/syntax/llvm.vim' to
" ~/.vim/syntax and add this code to your ~/.vimrc :
augroup filetype
  au! BufRead,BufNewFile *.ll     set filetype=llvm
augroup END

