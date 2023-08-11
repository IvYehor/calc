struct TreeNode {
  int type = 0; // 0 - Digit 1 - Operator
  // 0-9 digits
  // 0-+ 1-- 2-* 3-/ 4-Negation
  int value = 0;
  struct TreeNode *leftNode;
  struct TreeNode *rightNode;
};

int calculate(String expression);
struct TreeNode* parseExpression();
struct TreeNode* parseTerm();
struct TreeNode* parseFactor();
void nextToken();
int evaluate(struct TreeNode* node);

//void nodeDescr(struct TreeNode* node);
//void printTree(struct TreeNode* node, int depth);
//int getDepth(struct TreeNode* node);
//void logD(String message);

int tValue = 0, tType = -1, nextTIndex = -1;
String currentExpression = "";

int calculate(String expression) {
  currentExpression = expression;
  nextTIndex = 0;
  nextToken();
  
  struct TreeNode* fullTree = parseExpression();
  
  return evaluate(fullTree);
}

int evaluate(struct TreeNode *node){
  if(node->type == 0) {
    return node->value;
  }
  else if(node->type == 1) {
    switch(node->value) {
      case 0: // +
        return evaluate(node->leftNode) + evaluate(node->rightNode);
      case 1: // -
        return evaluate(node->leftNode) - evaluate(node->rightNode);
      case 2: // *
        return evaluate(node->leftNode) * evaluate(node->rightNode);
      case 3: // /
        return evaluate(node->leftNode) / evaluate(node->rightNode);
      case 4: // Negation
        return -evaluate(node->leftNode);
    }
  }
}

struct TreeNode* parseExpression() {
  // When calling the first token of the expression should be set
  
  struct TreeNode* node = parseTerm();
  while (1) {
    //nextToken(); - not needed here
    
    if(tType == 1 && (tValue == 0 || tValue == 1)) { // + or -
      struct TreeNode* node2 = (struct TreeNode*) malloc(sizeof(struct TreeNode));
      node2->type = 1;
      node2->value = tValue; // 2-* 3-/
      node2->leftNode = node;
      nextToken();
      node2->rightNode = parseTerm();
      node = node2;
    }
    else{
      return node;
    }
  }

  return node;
}

struct TreeNode* parseTerm() {
  // When calling the first token of the term should be set
  // After calling the next token after the term will be set
  
  struct TreeNode* node = parseFactor();
  while (1) {
    nextToken();
    if(tType == 1 && (tValue == 2 || tValue == 3)) { // * or /
      struct TreeNode* node2 = (struct TreeNode*) malloc(sizeof(struct TreeNode));
      node2->type = 1;
      node2->value = tValue; // 2-* 3-/
      node2->leftNode = node;
      nextToken();
      node2->rightNode = parseFactor();
      node = node2;
    }
    else{
      return node;
    }
  }

  return node;
}

struct TreeNode* parseFactor() {
  // When calling the first token of the factor should be set
  // After calling the last token of the factor will be set
  if (tType == 0) {
    struct TreeNode* node = (struct TreeNode*) malloc(sizeof(struct TreeNode));
    node->type = 0;
    node->value = tValue;
    node->leftNode = NULL;
    node->rightNode = NULL;
    return node;
  }
  else if(tType == 1) {
    if (tValue == 1) { // '-'
      struct TreeNode* node = (struct TreeNode*) malloc(sizeof(struct TreeNode));
      node->type = 1;
      node->value = 4;
      nextToken();
      node->leftNode = parseFactor();
      node->rightNode = NULL;
      return node;
    }
    else if(tValue == 4) { // '('
      nextToken();
      struct TreeNode* node = parseExpression();
      nextToken(); // ')'
      return node;
    }
  }
}

void nextToken() {
  // tokenIndex - index of a token to be parsed
  if(nextTIndex >= currentExpression.length()) {
    tType = -1;
    return;
  }
  
  char c = currentExpression.charAt(nextTIndex);
  nextTIndex++;

  
  switch (c) {
    case '+':
      tValue = 0;
      tType = 1;
      return;
    case '-':
      tValue = 1;
      tType = 1;
      return;
    case '*':
      tValue = 2;
      tType = 1;
      return;
    case '/':
      tValue = 3;
      tType = 1;
      return;
    case '(':
      tValue = 4;
      tType = 1;
      return;
    case ')':
      tValue = 5;
      tType = 1;
      return;
  }

  if(!isDigit(c))
    return;
    
  int number = c - '0';
  
  while (nextTIndex < currentExpression.length() && isDigit(c = currentExpression.charAt(nextTIndex))) { // || c == '.'
    number *= 10;
    number += c - '0';
    nextTIndex++;
  }
  tType = 0;
  tValue = number;
}

/*void logD(String message){
  
  Serial.print(message);
  Serial.print(" ");
  Serial.print(tValue);
  Serial.print(" ");
  Serial.print(tType);
  Serial.print(" ");
  Serial.print(tokenIndex);
  Serial.print(" ");
  Serial.println(currentExpression);
}*/

/*void printTree(struct TreeNode *node, int depth){
  for(int i = 0; i < depth; i++) {
    Serial.print("|-");
  }
  if(node->type == 0){
    Serial.print("Number: ");
    Serial.println(node->value);
  }
  else if(node->type == 1) {
    Serial.print("Operator: ");
    switch(node->value) {
      case 0:
        Serial.println("+");
        break;
      case 1:
        Serial.println("-");
        break;
      case 2:
        Serial.println("*");
        break;
      case 3:
        Serial.println("/");
        break;
      case 4:
        Serial.println("Negation");
        break;
    }
  }
  if(node->leftNode != NULL)
    printTree(node->leftNode, depth+1);
    
  if(node->rightNode != NULL)
    printTree(node->rightNode, depth+1);
}*/

/*void nodeDescr(struct TreeNode* node)
{
  Serial.print("Type: ");
  Serial.print(node->type);
  Serial.print(" Value: ");
  Serial.print(node->value);
  if (node->leftNode != NULL) {
    Serial.print(" LN type: ");
    Serial.print((node->leftNode)->type);
    Serial.print(" LN value: ");
    Serial.print((node->leftNode)->value);
  }
  if (node->rightNode != NULL) {
    Serial.print(" RN type: ");
    Serial.print((node->rightNode)->type);
    Serial.print(" RN value: ");
    Serial.print((node->rightNode)->value);
  }
  Serial.println("");
}*/

/*int getDepth(struct TreeNode* node){
  int res = 1;
  if(node->leftNode != NULL)
    res+=getDepth(node->leftNode);
  if(node->rightNode != NULL)
    res+=getDepth(node->rightNode);
  return res;
}*/
