

public class Gallery {
	final int[][] d = {{0,0,0},{0,0,0},{0,0,-1},{0,-1,0},{0,0,1},{0,1,0}};
	int position[][];//机器人位置
	int supervised[][];//监控状态
	int bestPos[][];//最优布局
	int m,n;
	int best;//当前最优哨位数
	int k;//当前哨位数
	int t;//当前监视陈列室数
	int t1,t2,more;
	boolean p;
	
	public void search(int i,int j){
		do {
			j++;//同一行右移
			if(j>n){//超出列数
				i++;//换行
				j=1;//第一列
			}
		} while (supervised[i][j]!=0&&i<=n);//已被监视并且未超范围
		if(i>n){//遍历到底
			if(k<best){//更优
				best = k;
				bestPos = (int[][]) position.clone();
			}
			return;
		}
		if(k+(t1-t)/5>=best) return;
		if((i<n-1)&&(k+(t2-t)/5>=best)) return;
		if(i<n){
			change(i+1, j);
			search(i, j);
			restore(i+1, j);
		}
		if((j<n)&&(supervised[i][j+1]==0||supervised[i][j+2]==0)){
			change(i, j+1);
			search(i, j);
			restore(i, j+1);
		}
		if((supervised[i+1][j]==0)&&(supervised[i][j+1]==0)){
			change(i, j);
			search(i, j);
			restore(i, j);
		}
	}
	public void change(int i,int j){
		position[i][j] = 1;
		k++;
		for(int a=1;a<=5;a++){
			int p = i+d[a][1];
			int q = j+d[a][2];
			supervised[p][q]++;
			if(supervised[p][q]==1){
				t++;
			}
		}
	}
	public void restore(int i,int j){
		position[i][j] = 0;
		k--;
		for(int a=1;a<=5;a++){
			int p = i+d[a][1];
			int q = j+d[a][2];
			supervised[p][q]--;
			if(supervised[p][q]==0){
				t--;
			}
		}
	}
	public void calculate(){
		more = m/4+1;
		if(m%4==3){
			more++;
		}else if(m%4==2){
			more += 2;
		}
		t1 = m*n + 4;
		t2 = m*n + more + 4;
		best = Integer.MAX_VALUE;
		for(int a=0;a<101;a++){
			for(int b=0;b<101;b++){
				position[a][b] = 0;
				supervised[a][b] = 0;
			}
		}
		if(m==1&&n==1){
			System.out.println("1\n1\n");
			return;
		}
		for(int i=0;i<m+1;i++){
			supervised[0][i] = 1;
			supervised[n+1][i] = 1;
		}
		for(int i=0;i<=n+1;i++){
			supervised[i][0] = 1;
			supervised[i][m+1] = 1;
		}
		search(1, 0);
	}
	public static void main(String[] args) {
		Gallery gallery = new Gallery();
		gallery.position = new int[101][101];
		gallery.supervised = new int[101][101];
		gallery.bestPos = new int[101][101];
		gallery.m = 2;
		gallery.n = 2;
		gallery.calculate();
		for(int a=1;a<=gallery.m;a++){
			for(int b=1;b<=gallery.n;b++){
				System.out.print(gallery.supervised[a][b]+"\t");
			}
			System.out.println("");
		}
	}
}
