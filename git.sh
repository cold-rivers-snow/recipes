#!/bin/bash

# 设置严格模式：遇到错误立即退出，未定义变量报错
set -euo pipefail

# 可选：设置颜色输出（便于阅读）
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

log() {
    echo -e "${GREEN}[INFO]${NC} $1"
}

warn() {
    echo -e "${YELLOW}[WARN]${NC} $1"
}

error() {
    echo -e "${RED}[ERROR]${NC} $1" >&2
    exit 1
}

# 检查是否在 Git 仓库中
if ! git rev-parse --git-dir > /dev/null 2>&1; then
    error "当前目录不是 Git 仓库！"
fi

# 获取当前分支名
current_branch=$(git symbolic-ref --short HEAD 2>/dev/null) || {
    error "无法获取当前分支名（可能处于 detached HEAD 状态）"
}

log "当前分支: $current_branch"

# 检查工作区和暂存区是否有未提交的更改
if ! git diff-index --quiet HEAD --; then
    error "工作区有未提交的更改，请先提交或 stash 后再运行此脚本。"
fi

# 检查是否有未暂存的文件（可选，根据需求决定是否严格）
# if ! git diff-files --quiet; then
#     error "有未暂存的修改，请先 add 或 stash。"
# fi

log "开始拉取远端更新并 rebase..."

# 拉取远端变更并 rebase 到当前分支
if git pull --rebase origin "$current_branch"; then
    log "成功拉取并 rebase 远端更新。"
else
    error "拉取或 rebase 失败！请手动解决冲突后重试。"
fi

log "正在推送本地更新到远端..."

# 推送当前分支到远端
if git push origin "$current_branch"; then
    log "推送成功！"
else
    error "推送失败！请检查权限或网络问题。"
fi

log "操作完成：本地已同步远端并成功推送。"