syntax on
set tabstop=4 softtabstop=4 shiftwidth=4 expandtab
set fileformat=unix
if has("autocmd")
    autocmd FileType make   set noexpandtab
endif
