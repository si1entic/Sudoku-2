# Sudoku-2
Pair programming project

<table style="table-layout: fixed; width: 100%;">
<thead>
<tr class="header">
<th>参数名字</th>
<th>参数意义</th>
<th>范围限制</th>
<th>用法示例</th>
</tr>
</thead>
<tbody>
<tr class="odd">
<td>-c</td>
<td>需要的数独终盘数量</td>
<td>1-1000000</td>
<td>示例：sudoku.exe -c 20 [表示生成20个数独终盘]</td>
</tr>
<tr class="even">
<td>-s</td>
<td>需要解的数独棋盘文件路径</td>
<td>绝对或相对路径</td>
<td>示例： sudoku.exe -s game.txt [表示从game.txt读取若干个数独游戏，并给出其解答，生成到sudoku.txt中]</td>
</tr>
<tr class="odd">
<td>-n</td>
<td>需要的游戏数量</td>
<td>1-10000</td>
<td>示例：sudoku.exe -n 1000 [表示生成1000个数独游戏]</td>
</tr>
<tr class="even">
<td>-m</td>
<td>生成游戏的难度</td>
<td>1-3</td>
<td>示例：sudoku.exe -n 1000 -m 1 [表示生成1000个简单数独游戏，只有m和n一起使用才认为参数无误，否则报错]</td>
</tr>
<tr class="odd">
<td>-r</td>
<td>生成游戏中挖空的数量范围</td>
<td>20-55</td>
<td>示例：sudoku.exe -n 20 -r 20~55 [表示生成20个挖空数在20到55之间的数独游戏，只有r和n一起使用才认为参数无误，否则报错]</td>
</tr>
<tr class="even">
<td>-u</td>
<td>生成游戏的解唯一</td>
<td></td>
<td>示例：sudoku.exe -n 20 -u [表示生成20个解唯一的数独游戏，只有u和n一起使用才认为参数无误，否则报错]</td>
</tr>
</tbody>
</table>

并拥有难度选择、提示、计时功能。
