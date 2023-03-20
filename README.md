# FingerGame
小时候玩过的“1+1”碰数游戏

**游戏规则：** 两个人四只手，分别从1开始，轮番碰对方的手指得到两数相加的个位值，碰得10的手就收回，先双手都碰到10的人赢。

本程序旨在寻找必胜策略，实现人机对决。

***

**操作说明**

*本程序需要在Unix环境中编译运行*

开始游戏后首先选择先手、后手

程序会输出每一轮的状态，上方两个数分别代表对方两个手所代表的数字（0表示手收回），下方两个数代表己方两只手所代表的数字。

每次轮到己方时，输入两个数字，第一个数字为己方某个手上的数字，第二个数字为对手某个手上的数字，表示用己方对应的手去碰对方对应的手。
