# RA2024
2024 年の RA における作成物

## popular arborescence
###
- Input: 有向グラフと、各辺の優先度（ハッセ図）
- Output: 有向全域木であって popular である物
### 定義: popular
条件を満たす物の内、二つの有向全域木に対して以下の順序を考える
根以外の各頂点について自身に入る辺が一本存在するが、辺の優先度が高い全域木に各頂点が一票ずつ入れる．優先度が比較不能の場合どちらにも入れない．
より多くの票が入った全域木を大きいとする．この時、どの全域木に対しても小さくない様な全域木を popular と定義する．

### アルゴリズム
Kavitha, T., Makino, K., Schlotter, I., & Yokoi, Y. (2024). Arborescences, colorful forests, and popularity. In Proceedings of the 2024 Annual ACM-SIAM Symposium on Discrete Algorithms (SODA) (pp. 3724-3746). Society for Industrial and Applied Mathematics.
にて提案されたアルゴリズムを実装する

### ビジュアライズ
赤いほど辺の優先度が高いとしている
dashed edge は $E(C)$ に属する辺、 solid edge は $I$ に属する辺、辺の太さは $lev_C(e)$ の大きさを表す．
<div><video controls src="https://github.com/hotman78/RA2024/assets/10681598/d7c30b57-7542-44b3-a62d-63c47fc9a4fb" muted="false"></video></div>



