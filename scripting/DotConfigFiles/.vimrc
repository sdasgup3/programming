set background=dark

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
se nospell
se number

" Enable mouse scrolling
set mouse=a
map <ScrollWheelUp> <C-Y>
map <ScrollWheelDown> <C-E>

":set formatoptions+=w
":set tw=80

" Map commands for line wrap
" granularity
" fa: full file
" fp: paragraph
" fl: line
map fa gggqG
map fp gqip
map fl gqq

" This is for llvm syntax highlighting
" copy `src/utils/vim/syntax/llvm.vim' to
" ~/.vim/syntax and add this code to your ~/.vimrc :
"augroup filetype
"  au! BufRead,BufNewFile *.ll     set filetype=llvm
"augroup END
augroup filetype
  au! BufRead,BufNewFile *.swift     set filetype=swift
augroup END

" Markdown syntax highlighting
"filetype on
"au BufNewFile,BufRead *.{md,mdown,mkd,mkdn,markdown,mdwn} set     filetype=markdown

" Set not compatible mode to enable Vim features.
"set nocp

" File access patterns
map Q :q
map N :n

" Do git clone https://github.com/VundleVim/Vundle.vim.git ~/.vim/bundle/Vundle.vim
" set the runtime path to include Vundle and initialize
set rtp+=~/.vim/bundle/Vundle.vim
call vundle#begin()
Plugin 'ctrlpvim/ctrlp.vim'
Plugin 'vim-scripts/Vimchant'
" Plugin 'Valloric/YouCompleteMe'
" All of your Plugins must be added before the following line
call vundle#end()         " required
filetype plugin indent on

"Start from the git root of the project
let g:ctrlp_working_path_mode = 'ra'

let g:ctrlp_cmd = 'CtrlPBuffer'

