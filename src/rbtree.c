#include "rbtree.h"

#include <stdlib.h>

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  // TODO: initialize struct if needed
  // NIL은 NULL고 다르게 실제로 메모리에 존재하는 노드

  //NIL 노드 생성, 모든 노드가 같은 NIL 포인터를 참조
  p->nil = (node_t *) malloc ( sizeof(node_t));
  p->nil->color = RBTREE_BLACK;
  p->nil->left = p->nil->right = p->nil->parent= p->nil;

  p->root=p->nil;
  

  return p; //RBT 포인터 반환
}

// 트리 좌회전용 함수
void left_rotate(rbtree *t,node_t *new)
{

}

//트리 우회전용 함수
void right_rotate(rbtree *t,node_t *new)
{


}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  free(t);
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


  return t->root;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  return t->root;
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  return 0;
}
