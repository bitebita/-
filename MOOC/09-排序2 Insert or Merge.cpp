#include <iostream>
#include <algorithm>
int main() {
	int n, i, count, length, a[100], b[100];
	int flag = 1;
	scanf("%d", &n);
	for (i = 0;i < n;i++)
		scanf("%d", &a[i]);
	for (i = 0;i < n;i++)
		scanf("%d", &b[i]);

	//i��0��n-1��������b[i+1]<b[i]�����������ж�a[i] == b[i]
	//����ȣ���Ϊ�鲢������Ϊ����
	for (i = 1; b[i] >= b[i - 1]; i++);
	count = i;
	for (;i < n && a[i] == b[i];i++);

	if (i == n) {
		printf("Insertion Sort\n");
		std::sort(b, b + count + 1);
		printf("%d", b[0]);
		for (i = 1;i < n;i++)
			printf(" %d", b[i]);
	}

	else {
		printf("Merge Sort\n");
		for (length = 2;flag;length *= 2) {
			//�ڳ���ΪlengthƬ������˳�����еĻ������ж�length*2
			for (i = length; i < n; i += length * 2) {
				if (b[i - 1] > b[i]) {
					flag = 0;break;
				}
			}
		}
		for (i = 0;i + length <= n;i += length)
			std::sort(b + i, b + i + length);
		std::sort(b + i, b + n);
		printf("%d", b[0]);
		for (i = 1;i < n;i++)
			printf(" %d", b[i]);
		//��һ�ֱȽ�ֱ�ӵ�������ģ��merge���̣�ÿһ�����ж��Ƿ���b���
	}
	return 0;
}


