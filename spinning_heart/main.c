#include <math.h>
#include <stdio.h>
#include <unistd.h>


int main(){
	//clear scrn and hide cursor
	printf("\x1b[2J\x1b[?25l");
	
	float t = 0;
	while(1){
		
		float zb[100 * 40] = {0};
		float maxz = 0, c = cos(t), s = sin(t);
		for (float y = -0.5f; y <= 0.5f; y+=0.01f){
			//make it beat!
			float r = 0.4f + 0.05f * pow(0.5f + 0.5f*sin(6*t+y*3), 8);
			for (float x = -0.5f; x <=0.5f; x+=0.01f){
				//heart formula
				float z = -x * x - pow(1.2f * y - fabs(x) * 2 / 3, 2) + r * r;
				if (z<0){
					continue;
				}
				z = sqrt(z) / (2-y);
				for (float tz = -z; tz <= z; tz+= z/6){
					//Rotate this shiii
					float nx = x * c - tz * s;
					float nz = x * s + tz * c;
					
					//add perspective
					float p = 1 + nz /2;
					int vx =  lroundf((nx * p +0.5f) *80);
					int vy = lroundf((-y * p  + 0.5f) * 39);
					int idx = vx + vy * 100;
					if (zb[idx] <= nz){
						zb[idx] = nz;
						if (maxz <= nz){
							maxz = nz;
						}
					}


				}
			}
		}
	

		printf("\x1b[H");
		for (int i = 0; i < 100 * 40; i++){
			putchar(i%100 ? " ., ^'::!*&$#@"[lroundf(zb[i] / maxz * 13)] : 10);
		}			
		t += 0.002f;
		usleep(300);
	}
	return 0;
	
}

