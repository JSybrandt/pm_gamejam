#pragma once

struct Controls{
	UCHAR up,down,left,right,walk,use;
	Controls(UCHAR U, UCHAR D, UCHAR L, UCHAR R,UCHAR W,UCHAR USE){
		up = U;
		down = D;
		left = L;
		right = R;
		walk = W;
		use = USE;
	}
	Controls(){};
};