#include <stdio.h>

void GetAndPrintSequence(int a[], int n)
{
	a[n] = -99999;        //����������ڱ���ʹ�����һ��һ������£� 
	int i, len, maxLen, start, isStart;
	start = isStart = 0;
	len = maxLen = 1;
	
	for (i = 0; i < n; i++) {
		if (a[i] < a[i + 1]) {
			len++;
		} else {
			if (len > maxLen) {
				maxLen = len;
				start = isStart;
			}
			isStart = i + 1;
			len = 1;
		}
	}
	
	printf("%d", a[start]);
	for (i = 1; i < maxLen; i++) {
		printf(" %d", a[start + i]);
	}	
} 

int main()
{
	int a[100001];  //�࿪��һ���ռ�����ڱ� 
	int n, i;
	scanf("%d", &n);
	for (i = 0; i < n; i++) {
		scanf("%d", &a[i]);
	}
	GetAndPrintSequence(a, n);
	
	return 0;
}
