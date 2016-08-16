/*
 * 已知随机函数rand()，以p的概率产生0，以1-p的概率产生1，
 * 现在要求设计一个新的随机函数new_rand()，
 * 使其以1/n的等概率产生1~n之间的任意一个数。
 */

/*
 * 解决思路：可以通过已知随机函数rand()产生可以等概率生成0和1的新随机函数rand_ex，
 * 然后调用k(k为整数n的二进制表示的位数)次rand_ex()函数，得到一个长度为k的0和1序列，
 * 以此序列所形成的整数即为1-n之间的数字。注意：从产生序列得到的整数有可能大于n，
 * 如果大于n的话，则重新产生直至得到的整数不大于n。
 */

//第一步：由rand()函数产生rand_ex()函数，rand_ex()函数等概率产生0和1。
int rand_ex()
{
	int rand_num1 = rand();
	int rand_num2 = rand();
	if (rand_num1 == 0 && rand_num2 == 1)
		return 1;
	else if (rand_num1 == 1 && rand_num2 == 0)
		return 0;
	
	return rand_ex();
}
//关于rand_ex()说明：只有当rand_num1和rand_num2两个值互不相等时，概率才相等。
//两者都为1的概率为(1-p)*(1-p)，两者都为0的概率为p*p，所以互不相等的概率是rate=1-p*p-(1-p)(1-p)。
//因为两个值中其中一个为0，另一个为1的概率都是rate*1/2。所以取互不相等的情况作为产生0和1的结果。

//第二步：计算整数n的二进制表示所拥有的位数k，k = 1 +log2n(2为底，n为值)
//第三步：调用k次rand_ex()产生随机数。
int new_rand()
{
	int result = 0;
	for (int i = 0; i < k; ++i)
	{
		if (rand_ex() == 1)
			result |= (1 << i);
	}

	if (result > n)
		return new_rand();
	
	return result;
}


//给定rand5()能随机等概率生成整数1-5的函数，写出能随机等概率生成整数1-7的函数rand7()
//思路：很多人的第一反应是利用rand5()+rand5()%3来实现rand7()函数，
//这个方法确实可以产生1-7之间的随机数，但是仔细想想可以发现数据生成的概率是不相等的。
//rand() % 3产生0的概率是1/5，而产生1和2的概率都是2/5，所以这个方法产生6和7的概率大于产生5的概率。
//正确的方法是利用rand5()函数生成1-25之间的数字(批注1)，然后将其中的1-21映射成1-7，丢弃22-25。
//例如两次rand5()分别生成(1, 1)，则看成rand7()中的2，如果出现22-25，则丢弃重新生成。
//批注1：
//讲解为什么(rand5()-1)*5+rand5()可以构造出均匀分布在1-25的随机数。
//首先rand5()-1得到一个离散整数集合{0, 1, 3, 4}，其中每个整数出现的概率都是1/5。
//那么(rand5()-1)*5得到一个离散数据集合A={0, 5, 10, 15, 20}，其中每个整数出现的概率也都是1/5。
//而rand5()得到的集合B={1, 2, 3, 4, 5}中，每个整数出现的概率也都是1/5。
//显然集合A和B中任何两个元素组合可以与1-25之间的一个整数一一对应，也就是说1-25之间的任何一个数，
//可以唯一确定A和B中两个元素的一种组合方式，反正过来亦成立。由于A和B中元素可以看成是独立事件，
//根据独立事件的概率公式P(AB)=P(A)P(B)，得到的每个组合的概率是1/5*1/5=1/25。
//因此(rand5()-1)*5+rand5()生成的整数均匀分布在1-25之间，每个数的概率都是1/25。
//注：有人提出rand7()实现中为什么用while (x > 21)而不用while (x > 7)呢？
//原因是如何用while (x > 7)，则有12/21的概率需要循环while。有可能造成死循环。
int rand7()
{
	int x = 0;
	do
	{
		x = 5 * (rand5() - 1) + rand5();
	} while (x > 21);

	return 1 + x % 7;
}
//备注：
//这种思想是基于rand()产生[0, n-1]，把rand()视为n进制的一位数产生器，
//那么可以使用rand()*n+rand()来产生2位的n进制数，以此类推，可以产生3位，4位...的n进制数。
//这种按构造n进制数的方式生成的随机数，必定能保证等概率。相反，
//借助其他方式来使用rand()产生随机数(如rand5()+rand5()%3)都不能保证概率平均。
//以题中n=5为例，可以使用rand5()*5+rand5()来产生2位的5进制数，范围就是1-25。
//再去掉22-25，剩余的除3，以此作为rand7()的产生器。

//给定rand()，能产生0到n-1之间的等概率随机数，问如何产生0到m-1之间等概率的随机数？
int random(int m, int n)
{
	int k = rand();
	int max = n - 1;
	while (k < m)
	{
		k = k * n + rand();
		max = max * n + n - 1;
	}

	return k / (max / n);
}

//如何产生如下概率的随机数？0出1次，1出2次，2出3次，n-1出n次？
int random(int size)
{
	while (1)
	{
		int m = rand(size);
		int n = rand(size);
		if (m + n < size)
			return m + n;
	}
}
//如何利用rand7()实现等概率的rand5()？
int rand5()
{
	int x = 0;
	do
	{
		x = rand7();
	} while (x > 5);

	return x;
}
