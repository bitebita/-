void hanoiRecursive(char a, char b, char c, int n)  //��ŵ���ݹ�ʵ��, a,b,c�ֱ�Ϊ��ʼ���������յ�, nΪ������ 
{
	if (n > 0) {
		hanoiRecursive(a, c, b, n - 1);
		printf("%c->%c\n", a, c);                  //n = 1, ��ʱa������һ������ֱ������c��Բ�� 
		hanoiRecursive(b, a, c, n - 1);	
	}
}
