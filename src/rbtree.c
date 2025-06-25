#include "rbtree.h"
#include <stdlib.h>

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  // TODO: initialize struct if needed
  // NIL은 NULL와 다르게 실제로 메모리에 존재하는 노드

  //NIL 노드 생성, 모든 노드가 같은 NIL 포인터를 참조
  p->nil = (node_t *) malloc ( sizeof(node_t));
  p->nil->color = RBTREE_BLACK;
  p->nil->left = p->nil->right = p->nil->parent= p->nil;

  p->root=p->nil;
  return p; //RBT 포인터 반환
}

// 트리 좌회전용 함수
void left_rotate(rbtree *t,node_t *x)
{
  //new 를 중심축으로 회전 예정
  node_t *y = x->right;
  x->right = y->left;
  
  if (y->left != t->nil) //Y 의 왼쪽 서브트리가 비어있지 않을때
  {
    y->left->parent = x; // x는 서브트리 루트의 부모가 됨
  }

  // new의 부모 (Z) 가 y의 부모가 된다.
  y->parent = x->parent;

  if (x->parent == t->nil) //new가 루트인 경우
  {
    t->root = y; //y가 루트가 됨
  }

  else if ( x == x->parent->left) //x가 왼쪽 자식인 경우
  {
    x->parent->left = y; //왼쪽 회전되면서 y가 왼쪽 자식이됨
  }

  else // x가 오른쪽 자식이었던 경우
  {
    x->parent->right = y; 
  }

  y->left = x;
  x->parent = y; 

}

//트리 우회전용 함수 - left_rotate 에서 (left,right)만 바꿈
void right_rotate(rbtree *t,node_t *x)
{
  // x 를 중심축으로 회전 예정
  node_t *y = x->left;            // (1) 축 노드 x의 왼쪽 자식 y 확보
  x->left = y->right;             // (2) y의 오른쪽 서브트리(β)를 x의 왼쪽으로 이동

  if (y->right != t->nil)         // (3) y의 오른쪽 자식이 존재할 경우
  {
    y->right->parent = x;         // β의 부모를 x로 연결
  }

  y->parent = x->parent;          // (4) x의 부모를 y가 대신하게 함

  if (x->parent == t->nil)        // (5) x가 루트였던 경우
  {
    t->root = y;                  // y가 새로운 루트가 됨
  }

  else if (x == x->parent->right) // (6) x가 오른쪽 자식이었다면
  {
    x->parent->right = y;         // 부모의 오른쪽에 y 연결
  }

  else                            // (7) x가 왼쪽 자식이었다면
  {
    x->parent->left = y;          // 부모의 왼쪽에 y 연결
  }

  y->right = x;                   // (8) x는 y의 오른쪽 자식이 됨
  x->parent = y;                  // (9) x의 부모를 y로 설정

}
void post_order(rbtree * t,node_t *cur)
{ 
  //후위순회 함수
  if (cur == t->nil) // 베이스 케이스
    return;

  post_order(t,cur->left);
  post_order(t,cur->right);
  free(cur);
}

void delete_rbtree(rbtree *t) {
  //free(t);
  // 부모노드부터 free하면 자식노드에 접근할 수 없으니 자식부터 free
  // 후위순회로 모든 노드를 돌기 (왼쪽 - 오른쪽 - 부모)
  node_t *cur = t->root;
  
  if (t->root != t->nil) // 빈 트리가 아닐때
    post_order(t,cur); //후위 순회
  
  free(t->nil); // NIL노드 해제
  free(t); //모든 노드 free후 마지막으로 트리 free
}

//삽입후 트리 FIX용 함수
void rbtree_insert_fixup(rbtree *t, node_t *new)
{
  node_t * uncle; //삼촌 노드포인터 uncle

while (new->parent != t->nil &&
       new->parent->color == RBTREE_RED)
      
      // 부모의 색 == Red인 동안
{
  if (new->parent == new->parent->parent->left) //부모가 할아버지의 왼쪽자식일때
    {
      uncle = new->parent->parent->right;  //삼촌의 위치는 오른쪽노드

       // 삼촌의 색이 Red라면 CASE. 1 에 해당!
      if (uncle->color ==RBTREE_RED) 
      {
        //부모, 삼촌, 할아버지 색 반전 해주기
        new->parent->color = RBTREE_BLACK;  //부모
        new->parent->parent->color = RBTREE_RED; //할아버지
        uncle->color = RBTREE_BLACK; // 삼촌

        //할아버지부터 다시 탐색
        new = new->parent->parent; 
      }

      else //삼촌의 색이 Black 일때 CASE 2, CASE3에 해당!
      {
        
        if (new == new->parent->right)
        {
          /*
          CASE 2 검사 : 
          부모의 색이 red 이고, 부모가 할아버지의 왼쪽 자식이면서,
          삼촌은 오른쪽 Black일때,
          그리고 자기(삽입한 위치)는 "오른쪽 자식"일때
          */
          // 또한 new의 parent를 "중심 축"으로 회전하므로 인자는 new->parent
          new = new-> parent;
          left_rotate(t,new);
        
        }

        //CASE 3 검사 :  부모와 할아버지 색 반전후 오른쪽회전 
        new->parent->parent->color=RBTREE_RED;
        new->parent->color = RBTREE_BLACK;
        
        // '할아버지' 를 중심축으로 회전
        right_rotate(t,new->parent->parent);
      }
    }

    // 위와 같지만, 이제 '왼쪽' 과 '오른쪽'이 바뀜
    else
    {
      uncle = new->parent->parent->left;

      // 삼촌의 색이 Red라면 CASE. 1 에 해당!
      if (uncle->color ==RBTREE_RED ) 
      {
        //부모, 삼촌, 할아버지 색 반전 해주기
        new->parent->color = RBTREE_BLACK;  //부모
        new->parent->parent->color = RBTREE_RED; //할아버지
        uncle->color = RBTREE_BLACK; // 삼촌
      
        //할아버지부터 다시 탐색
        new = new->parent->parent;
      }

      else //삼촌의 색이 Black 일때 CASE 2, CASE3에 해당!
      {
        
        if (new == new->parent->left)
        {
          /*
          CASE 2 검사 : <반전인 경우이므로 new는 '왼쪽'자식>
           자기(삽입한 위치)는 "왼쪽 자식"일때,
          */

          // 또한 new의 parent를 "중심 축"으로 회전하므로 인자는 new->parent
          new = new->parent;
          right_rotate(t,new);
        }

        //CASE 3 검사 :  부모와 할아버지 색 반전후 오른쪽회전 
        new->parent->parent->color=RBTREE_RED;
        new->parent->color = RBTREE_BLACK;
        
        // 균형을 잡기 위해 , '할아버지' 를 중심축으로 회전
        left_rotate(t,new->parent->parent);
      }

    }
  
}
// 루트는 항상 BLACK으로 Fix
t->root->color = RBTREE_BLACK;
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // KEY 는 int 형 Costant
  // 새 노드생성 , 색은 일단 red(루트 제외)
  node_t* new = malloc(sizeof(node_t)); 

    if (new == NULL) return NULL; //예외처리
  new->color = RBTREE_RED; 
  new->key = key; 
  new->parent=  new->left=  new->right= t->nil;


  // x : 삽입위치 , y: 삽입위치의 부모 
  node_t * x = t->root; 
  node_t * y = t->nil;

  //루트부터 탐색시작
  while ( x != t->nil)
  {
    y= x;
    if (key < x->key) //key가 더 작을때 
      x = x->left;

    else
      x = x->right;  //key가 더 클때
  }

  new->parent = y; // 새노드의 부모를 y로 링크

  //트리가 비어있을때 
  if (y == t->nil) // y,t->nil 모두 node_t*
  {
    t->root= new; 
    new->color = RBTREE_BLACK; // 루트면 무조건 Black
  }

  //key 가 y보다 작을때 연결방법
  else if (key < y->key)
  {
    y->left = new;
  }

  else // (key > y->key)
  {
    y->right = new;
  }

  // 레드블랙트리 속성 위반 수정
  rbtree_insert_fixup(t,new);
  //return t->root; 반환값 new로 해야함
  return new;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  node_t* cur = t->root; //루트부터 탐색시작
  while(cur != t-> nil ) //key를 찾을때까지 탐색
  { //cur == nil 이거나 cur == key 면 종료
    if (key == cur->key)
      return cur;

    else if ( key < cur->key) //찾는 값이 더 작으면 왼쪽 이동
      cur = cur ->left;

    else // 찾는값이 더 크다면 오른쪽 이동
      cur = cur ->right; 
  }
  //return t->nil; //찾는 값이 없으면 NIL 반환
  return NULL;
}

node_t *rbtree_min(const rbtree *t) {
  //가장 왼쪽으로 이동
  node_t* cur = t->root; //루트부터 탐색시작
  while ( cur != t->nil) //NIL이 아닌동안
  {
    if (cur->left == t->nil) //다음노드가 NIL 이면 현재노드반환
      return cur;

    cur = cur->left; //왼쪽으로 이동
  }
  return NULL; 
}

node_t *rbtree_max(const rbtree *t) {
  //가장 오른쪽으로 이동
  node_t* cur = t->root; //루트부터 탐색시작
  while ( cur != t->nil) //NIL이 아닌동안
  {
    if (cur->right == t->nil) //다음노드가 NIL 이면 현재노드반환
      return cur;

    cur = cur->right; //왼쪽으로 이동
  }
  return NULL;
}
/////////////////////////////////////////////////////////////////////

//erase 후 규칙 위반 수정 함수
void rbtree_erase_fixup(rbtree *t, node_t *new)
{
  while (new != t->root && new->color == RBTREE_BLACK) {
    if (new == new->parent->left) {
      node_t *w = new->parent->right; // 형제 노드
      if (w->color == RBTREE_RED) {
        // Case 1: 형제가 RED
        w->color = RBTREE_BLACK;
        new->parent->color = RBTREE_RED;
        left_rotate(t, new->parent);
        w = new->parent->right;
      }
      if (w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK) {
        // Case 2: 형제와 형제의 자식들이 모두 BLACK
        w->color = RBTREE_RED;
        new = new->parent;
      } else {
        if (w->right->color == RBTREE_BLACK) {
          // Case 3: 형제의 오른쪽 자식이 BLACK, 왼쪽 자식이 RED
          w->left->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          right_rotate(t, w);
          w = new->parent->right;
        }
        // Case 4: 형제의 오른쪽 자식이 RED
        w->color = new->parent->color;
        new->parent->color = RBTREE_BLACK;
        w->right->color = RBTREE_BLACK;
        left_rotate(t, new->parent);
        new = t->root;
      }
    } 
    else 
    {
      // new가 오른쪽 자식일 때 (대칭)
      node_t *w = new->parent->left;
      if (w->color == RBTREE_RED) {
        // Case 1
        w->color = RBTREE_BLACK;
        new->parent->color = RBTREE_RED;
        right_rotate(t, new->parent);
        w = new->parent->left;
      }
      if (w->right->color == RBTREE_BLACK && w->left->color == RBTREE_BLACK) {
        // Case 2
        w->color = RBTREE_RED;
        new = new->parent;
      } else {
        if (w->left->color == RBTREE_BLACK) {
        // Case 3
          w->right->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          left_rotate(t, w);
          w = new->parent->left;
        }
        // Case 4
        w->color = new->parent->color;
        new->parent->color = RBTREE_BLACK;
        w->left->color = RBTREE_BLACK;
        right_rotate(t, new->parent);
        new = t->root;
      }
    }
  }
  new->color = RBTREE_BLACK;

}

node_t *rbtree_min_sub(const rbtree *t, node_t *start) 
{
  node_t *x = start;
  // 왼쪽 자식이 없을때까지 이동
  while (x->left != t->nil) 
  {
    x = x->left;
  }
  return x;
}

void rbtree_transplant(rbtree *t, node_t *u, node_t *v)
{ // u 노드를 v노드로 이식하는 함수 p.370 참고
   if (u->parent == t->nil) // u == 루트 노드일때 (루트 노드의 parent = nil)
    t->root = v;

  else if (u == u->parent->left) // u == 왼쪽 자식일때
    u->parent->left = v;
  else 
    u->parent->right = v; // u == 오른쪽 자식일때

  v->parent = u->parent; // u의 부모와 v 연결 
}

//노드 제거 함수
// x : fixup 함수의 인자로 들어감
// y : 실제로 트리에서 삭제되는 노드
// z : "삭제하고자 하는" 노드
// 만약 z 의 자식이 2개일때는 successor가 y가 된다.
int rbtree_erase(rbtree *t, node_t *z) {
  node_t *y = z;
  color_t y_original_color = y->color;
  node_t *x;

  if (z->left == t->nil) 
  {
    x = z->right;
    // z를 z->right로 이식
    rbtree_transplant(t, z, z->right);
  }
  else if (z->right == t->nil) 
  {
    x = z->left;
    // z를 z->left로 이식
    rbtree_transplant(t, z, z->left);
  } 
  else 
  {
    y = rbtree_min_sub(t, z->right); // 오른쪽 서브트리에서 최소값
    y_original_color = y->color;
    x = y->right;

    if (y->parent != z) 
    {
      rbtree_transplant(t, y, y->right);
      y->right = z->right;
      y->right->parent = y;
    } 
    else 
    {
      if (x != t->nil) {
        x->parent = y;
      }
    }

    rbtree_transplant(t, z, y);
    y->left = z->left;
    y->left->parent = y;
    y->color = z->color;
  }

  if (y_original_color == RBTREE_BLACK) 
  {
    rbtree_erase_fixup(t, x);
  }
  
// 안전하게 메모리 해제
  if (y != z) {
    free(z);
    free(y);
  } else {
    free(z);
  }



  return 0;
}

//rbtree_to_array에 쓰인 중위순회 함수
void inorder(const rbtree *t, const node_t *node, key_t *arr, size_t n, size_t *idx) 
{
    // nil 일때 base case,  idx>=n 일때 예외처리 
    if (node == t->nil || *idx >= n) 
        return;

    inorder(t, node->left, arr, n, idx);

    //포인터가 쓰인 이유 : 함수내에서 idx 증가시키고, 그 값을 다시 인자로 전달위해
    //포인터변수가 아니었다면 함수가 끝나면 증가된 값이 사라짐 
    if (*idx < n) //인덱스 out of range 체크
    {
        arr[*idx] = node->key;
        (*idx)++;
    }

    inorder(t, node->right, arr, n, idx);
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) 
{
  // 중위순회를 이용하여 트리의 값을 오름차순으로 arr에 저장

  if (t == NULL || arr == NULL || n == 0) 
    return 0;
      
  size_t idx = 0;

  inorder(t, t->root, arr, n, &idx);

  return 0;  // 또는 실제 저장된 요소 개수를반환
}
