#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <cmath>
#include <string>
const int N = 200;
const int W = 2560;
const int H = 1440;

class circle{
public:
	float x,y,r,dx,dy;
	sf::Color col;
	std::string status;
	
	circle(){
		r = (rand() % 20) + 20;
		x = rand() % int (W-r*2) + r;
		y = rand() % int (H-r*2) + r;
		dx = ((rand() % 50) + 1)/10;
		dy = ((rand() % 50) + 1)/10;
		if (rand() % 2 == 0){
			dx=-dx;
		}
		if (rand() % 2 == 0){
			dy=-dy;
		}
		col = sf::Color(rand() % 256,rand() % 256,rand() % 256);
		status = "Asteroid";
	};
	newcircle(){
		r = (rand() % 20) + 20;
		x = rand() % int (W-r*2) + r;
		y = rand() % int (H-r*2) + r;
		dx = ((rand() % 50) + 1)/10;
		dy = ((rand() % 50) + 1)/10;
		if (rand() % 2 == 0){
			dx=-dx;
		}
		if (rand() % 2 == 0){
			dy=-dy;
		}
		col = sf::Color(rand() % 256,rand() % 256,rand() % 256);
		status = "Asteroid";
	}
	
	re(float x1,float y1,float r1,float dx1,float dy1,sf::Color col1,std::string status1){
		x = x1; y = y1; r = r1; dx = dx1; dy = dy1; col = col1; status = status1;
	};
	
	void move(){
		x+=dx;
		y+=dy;
		if ((x+r>W)||(x-r<0)){
			dx=-dx;
		}
		if ((y+r>H)||(y-r<0)){
			dy=-dy;
		}
		//dy+=0.1f;
	}
	
	void del(){
		x = 0;
		y = 0;
		r = 0;
		dx = 0;
		dy = 0;
		col = sf::Color(0,0,0);
		status = "None";
	}
};

circle cir[N];

class sprite{
public:
	int frame;
	int Wsize, Hsize, Wpsize, Hpsize;
	sf::Image image;
	sf::Texture texture;
	sf::Sprite sprit;
	float x,y;
	
	sprite(std::string path, float Wsize1, float Hsize1, float Wpsize1, float Hpsize1){
		image.loadFromFile(path);
		texture.loadFromImage(image);
		sprit.setTexture(texture);
		Wsize = Wsize1;
		Hsize = Hsize1;
		Wpsize = Wpsize1;
		Hpsize = Hpsize1;
		frame = 0;
	}
	void anim(){
		frame++;
		if (frame > Wsize*Hsize) frame = 0;
		sprit.setTextureRect(sf::IntRect(Wpsize * int(frame) % Wsize, Hpsize * (int(frame)-int(frame) % Wsize) / Hsize, Wpsize, Hpsize));
	}
	void move(){
		sprit.setPosition(x,y);
	}
	void move(float x1, float y1){
		x = x1;
		y = y1;
		sprit.setPosition(x,y);
	}
	
	//--------------
};
sprite vzriv("images\\vzriv.png", 9, 9, 100, 100);


void stolk();
void stolk(int i, int j){
	float Dx = cir[j].x - cir[i].x;
	float Dy = cir[j].y - cir[i].y;
	float d = sqrt(Dx*Dx+Dy*Dy); if (d == 0) d = 0.01;
	float s = Dx/d; // sin
	float e = Dy/d; // cos
	if (d < cir[j].r+cir[i].r){
		float Vn1 = cir[i].dx*s + cir[i].dy*e;
		float Vn2 = cir[j].dx*s + cir[j].dy*e;
		float dt = (cir[i].r+cir[j].r-d)/(Vn1-Vn2);
		if (dt > 0.6) dt = 0.6;
		if (dt < -0.6) dt = -0.6;
		cir[j].x-=cir[j].dx*dt;
		cir[j].y-=cir[j].dy*dt;
		cir[i].x-=cir[i].dx*dt;
		cir[i].y-=cir[i].dy*dt;
		Dx = cir[j].x - cir[i].x;
		Dy = cir[j].y - cir[i].y;
		d = sqrt(Dx*Dx+Dy*Dy); if (d == 0) d = 0.01;
		s = Dx/d; // sin
		e = Dy/d; // cos
		Vn1 = cir[i].dx*s + cir[i].dy*e;
		Vn2 = cir[j].dx*s + cir[j].dy*e;
		float Vt1 = -cir[i].dx*e+cir[i].dy*s;
		float Vt2 = -cir[j].dx*e+cir[j].dy*s;
		
		if ((cir[i].status == "Asteroid")&&(cir[j].status == "Asteroid")){
			float o = Vn2;
			Vn2 = Vn1;
			Vn1 = o;
		}
		if ((cir[i].status == "Asteroid")&&(cir[j].status == "Hero")){
			vzriv.move(cir[i].x-vzriv.Wpsize / 2,cir[i].y-vzriv.Hpsize / 2);
			cir[i].del();
			//vzrivSprite.setPosition(cir[i].x,cir[i].y);
			//cir[j].r++;
		}
		if ((cir[i].status == "Hero")&&(cir[j].status == "Asteroid")){
			vzriv.move(cir[j].x-vzriv.Wpsize / 2,cir[j].y-vzriv.Hpsize / 2);
			cir[j].del();
			//vzriv.move(1000,1000);
			//vzrivSprite.setPosition(cir[j].x,cir[j].y);
			//cir[i].r++;
		}
		
		cir[j].dx = Vn2*s-Vt2*e;
		cir[j].dy = Vn2*e+Vt2*s;
		cir[i].dx = Vn1*s-Vt1*e;
		cir[i].dy = Vn1*e+Vt1*s;
		cir[j].x+=cir[j].dx*dt;
		cir[j].y+=cir[j].dy*dt;
		cir[i].x+=cir[i].dx*dt;
		cir[i].y+=cir[i].dy*dt;
	}
	
}
//   |---------------------------------------|--|
int vzrivFrame = 0;
int main()
{	
	cir[0].re(W/2,H/2,100, 0, 0, sf::Color(255,255,255), "Hero");
	
	sf::RenderWindow window(sf::VideoMode(W, H), "My SFML WORKS!!!", sf::Style::Fullscreen);
	
//	sf::Image vzrivImage;
//	vzrivImage.loadFromFile("images\\vzriv.png");
//	
//	sf::Texture vzrivTexture;
//	vzrivTexture.loadFromImage(vzrivImage);
//	
//	sf::Sprite vzrivSprite;
//	vzrivSprite.setTexture(vzrivTexture);
//	vzrivSprite.setTextureRect(sf::IntRect(0,0,100,100));
//	vzrivSprite.setPosition(200,200);
	
	
	
	
	sf::CircleShape shape(0);
	
	
	
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		{
		    window.close();
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
		    for (int i = 0; i < N; i++)
				if (cir[i].status == "Hero"){
					cir[i].dx-=0.1;
				}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
		    for (int i = 0; i < N; i++)
				if (cir[i].status == "Hero"){
					cir[i].dx+=0.1f;
				}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		{
		    for (int i = 0; i < N; i++)
				if (cir[i].status == "Hero"){
					cir[i].dy-=0.1f;
				}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
		    for (int i = 0; i < N; i++)
				if (cir[i].status == "Hero"){
					cir[i].dy+=0.1f;
				}
		}
		sf::Vector2i mp = sf::Mouse::getPosition();
		
		if (rand() % 10 == 1){
			for (int i = 0; i < N; i++)
				if (cir[i].status == "None"){
					cir[i].newcircle();
					break;
				}
		}


		for (int i = 0; i < N; i++)
			for(int j = i; j < N; j++)
				if (i != j){
					stolk(i,j);
				}
	
		window.clear();
		for (int i = 0; i < N; i++) {
			shape.setRadius(cir[i].r);
			shape.setOrigin(cir[i].r, cir[i].r);
			shape.setPosition(cir[i].x,cir[i].y);
			shape.setFillColor(cir[i].col);
	        window.draw(shape);
	        cir[i].move();
		}
		
		
		float d = sqrt((mp.x-cir[0].x)*(mp.x-cir[0].x)+(mp.y-cir[0].y)*(mp.y-cir[0].y)); if (d == 0) d = 0.001f;
		float r = cir[0].r*(mp.y-cir[0].y)/d;
		float r1 = cir[0].r*(mp.x-cir[0].x)/d;
		sf::Vertex line[] =
		{
		    sf::Vertex(sf::Vector2f(cir[0].x, cir[0].y), sf::Color(255,0,0)),
		    sf::Vertex(sf::Vector2f(cir[0].x+r1, cir[0].y+r), sf::Color(255,0,0))
		};
		
		window.draw(line, 2, sf::Lines);
		
		
		
		
		//vzrivFrame++;
		//if (vzrivFrame > 64) vzrivFrame = 0;
		//vzrivSprite.setTextureRect(sf::IntRect(100 * int(vzrivFrame) % 9, 100 * (int(vzrivFrame)-int(vzrivFrame) % 9) / 9, 100, 100));
		vzriv.anim();
		window.draw(vzriv.sprit);
		
		window.display();
		//sf::sleep(sf::milliseconds(int(3000)));
		sf::sleep(sf::milliseconds(int(1000/120)));
    }

    return 0;
}
/*
#include <iostream>
int main(){
	setlocale(LC_ALL, "ru");
	
	int a[]{45,234,65,14,65,1,454,234,45};
	
	printf ("%d \n\n", sizeof(a)/sizeof(a[0]));
	for (int i = 0;i<100;i++){
		printf ("%d\n", a[i]);
	}
	
	
	return 0;
}*/
