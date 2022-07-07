bool Insert( List L, ElementType X )
{
	if (L->Last == MAXSIZE - 1) {
		return false; 
	}
	Position i = 0, tag;
    while (L->Data[i] > X && i <= L->Last) i++;   //�ж���������Ҫ������Ϊ<=���Ա�֤������Ϊ��Сֵ��ʱ��ָ������� 
    tag = i;                                      	
    if (L->Data[tag] == X) {
        return false;
    } else {
        i = L->Last;
        while (i >= tag) {
            L->Data[i + 1] = L->Data[i];
            i--;
        } 
        L->Data[tag] = X;
        L->Last++;
        return true;
    }
}
