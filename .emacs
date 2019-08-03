(custom-set-variables
 ;; custom-set-variables was added by Custom.
 ;; If you edit it by hand, you could mess it up, so be careful.
 ;; Your init file should contain only one such instance.
 ;; If there is more than one, they won't work right.
 '(custom-enabled-themes (quote (wheatgrass))))
(custom-set-faces
 ;; custom-set-faces was added by Custom.
 ;; If you edit it by hand, you could mess it up, so be careful.
 ;; Your init file should contain only one such instance.
 ;; If there is more than one, they won't work right.
 )

(setq inhibit-startup-screen t)
(setq-default tab-width 4)
(setq-default indent-tabs-mode nil)
(setq next-line-add-newlines nil)
(setq column-number-mode t)
(show-paren-mode)
(set-cursor-color "Yellow")
(global-set-key "\M-g" 'goto-line)
(global-set-key [delete] 'delete-char)

(setq c-basic-offset 4)
(add-hook 'c-mode-hook (lambda () (c-set-style style "cc-mode")))
(add-hook 'c++-mode-hook (lambda () (c-set-style style "cc-mode")))
(add-to-list 'auto-mode-alist '("\\.h\\'" . c++-mode))
(c-set-offset 'inline-open '0)
(setq compile-command "make")
(global-set-key [f6] 'compile)

(add-to-list 'load-path "~/.emacs.d")
(require 'tabbar)
(tabbar-mode t)

