#include<SFML/Graphics.hpp>
#include<bits/stdc++.h>
using namespace std;
void Replay();
void BuildWall();
void create_fig();
void move_and_display_current();
void set_matrix();
void applycolor(int x,int y);
void display_matrix();
bool present_next();
void applyfallingcolor();
void rotate();
bool no_hindarance();
void check_game_status();
void check_for_deletion();
void rearrange_matrix(int x);
class Block
{
public:
  pair<int,int> coordinate;
  sf::Color c;
  Block()
  {
    coordinate.first=-20;coordinate.second=-20;
    c=sf::Color(204,204,205);
  }
};
sf::Font font;
sf::Text score_text;
sf::Text game_over_text;
string score_string = "Score: ";
string header = "TETRIS!";
sf::RenderWindow window(sf::VideoMode(440,880), "Tetris!");
sf::RectangleShape wall(sf::Vector2f(20,20));
sf::RectangleShape b(sf::Vector2f(20,20));
sf::RectangleShape b2(sf::Vector2f(20,20));// for falling color
sf::Time t2 = sf::milliseconds(125);
char rot = ' ';
int game_play=1;
int min_x=999;int max_x=-999;
int min_y=999;int max_y=-999;
int total_blocks=0;
int in_progress=0;
int y_speed=1;
int x_speed=0;
int current_color=-1;
pair<int,int> current[4]={{-1,-1},{-1,-1},{-1,-1},{-1,-1}};
pair<int,int> figures[7][4]={
                             {{0,1},{1,1},{1,2},{1,0}} //T
                            ,{{0,0},{0,1},{0,2},{0,3}} //I
                            ,{{0,0},{1,0},{1,1},{2,1}} //Z
                            ,{{0,1},{1,1},{1,0},{2,0}} //S
                            ,{{0,0},{1,0},{1,1},{1,2}} //L
                            ,{{0,0},{1,0},{0,1},{1,1}} //O
                            ,{{1,0},{0,0},{0,1},{0,2}} //J
                            };
Block matrix[30][20];
int main()
{
  if(!font.loadFromFile("OpenSans-ExtraBold.ttf"))
  {
    std::cout<<"Error in loading the font"<<std::endl;
  }
  window.setFramerateLimit(60);
  for(int i=0;i<30;i++)
  {
    for(int j=0;j<20;j++)
    {
      matrix[i][j] = Block();
    }
  }
  while (window.isOpen())
  {
      sf::Event event;
      while (window.pollEvent(event))
      {
          if (event.type == sf::Event::Closed)
              window.close();
          else if (event.type == sf::Event::KeyPressed)
          {
            if (event.key.code == sf::Keyboard::Escape)
            {
              window.close();
            }
            else if(event.key.code == sf::Keyboard::Down)
            {
              y_speed=1;
            }
            else if(event.key.code == sf::Keyboard::Left)
            {
              x_speed=-1;
            }
            else if(event.key.code == sf::Keyboard::Right)
            {
              x_speed=1;
            }
            else if(event.key.code == sf::Keyboard::X)
            {
              rot='C';
            }
            else if(event.key.code == sf::Keyboard::Z)
            {
              rot='A';
            }
            else if(event.key.code == sf::Keyboard::R)
            {
              Replay();
            }
          }
      }
      window.clear(sf::Color::Black);
      if(game_play==1)
      {
        BuildWall();
        if(in_progress==0)
        {
          create_fig();
        }
        if(rot!=' ')
        {
          rotate();
        }
        move_and_display_current();
        check_game_status();
        display_matrix();
        window.display();
        sf::Clock clock;
        while(clock.getElapsedTime()<(t2)){}
      }
      else
      {
        window.clear(sf::Color::Red);
        game_over_text.setFont(font);
        game_over_text.setString("Game Over!");
        game_over_text.setCharacterSize(50);
        game_over_text.setPosition(75,300);
        window.draw(game_over_text);
        game_over_text.setString("Replay : r");
        game_over_text.setCharacterSize(25);
        game_over_text.setPosition(175,375);
        window.draw(game_over_text);
        window.display();
      }
  }
  return 0;
}
void BuildWall()
{
  score_text.setFont(font);
  score_text.setFillColor(sf::Color::White);
  score_text.setPosition(125,175);
  score_text.setCharacterSize(50);
  score_text.setString(score_string);
  window.draw(score_text);
  score_text.setString(header);
  score_text.setCharacterSize(75);
  score_text.setPosition(75,0);
  score_text.setFillColor(sf::Color::Yellow);
  window.draw(score_text);
  //score_string="Score: ";
  wall.setFillColor(sf::Color(160,160,160));
  wall.setOutlineThickness(2);
  wall.setOutlineColor(sf::Color(64,64,64));
  for(int i=12;i<=43;i++)
  {
    wall.setPosition(0,20*i);
    window.draw(wall);
    wall.setPosition(420,20*i);
    window.draw(wall);
  }
  for(int i=0;i<=20;i++)
  {
    wall.setPosition(20*i,860);
    window.draw(wall);
    wall.setPosition(20*i,240);
    window.draw(wall);
  }
  //window.display();
}

void create_fig()
{
  in_progress=1;
  total_blocks+=4;
  score_string="Score: ";
  score_string += to_string(total_blocks/4);
  srand(time(0));
  int a = rand()%7;
  current_color = a;
  for(int i=0;i<4;i++)
  {
    current[i] = figures[a][i];
  }
  int b = rand()%15 + 2;
  for(int i=0;i<4;i++)
  {
    current[i].first+=b;
    current[i].second+=12;
  }
}

void move_and_display_current()
{
  min_x=999;min_y=999;max_x=-999;max_y=-999;
  for(int i=0;i<4;i++)
  {
    if(current[i].first<min_x)
      min_x=current[i].first;
    if(current[i].second<min_y)
      min_y=current[i].second;
    if(current[i].first>max_x)
      max_x=current[i].first;
    if(current[i].second>max_y)
      max_y=current[i].second;
  }
  if(present_next() || max_y>41)
  {
    in_progress=0;min_x=999;min_y=999;max_x=-999;max_y=-999;
    set_matrix();
  }
  for(int i=0;i<4;i++)
  {
    if(x_speed==1 && max_x<20 && no_hindarance())
    {
      current[i].first+=x_speed;
    }
    else if(x_speed==-1 && min_x>1 && no_hindarance())
    {
      current[i].first+=x_speed;
    }
    else
    {
      if(max_y<42)
        current[i].second+=min(y_speed,42-current[i].second);
      else
      {
        min_x=999;min_y=999;max_x=-999;max_y=-999;
        in_progress=0;
        set_matrix();
      }
    }
  }
  x_speed=0;
  y_speed=1;

  //display now.
  //sf::RectangleShape b(sf::Vector2f(20,20));
  applyfallingcolor();
  //b.setFillColor(sf::Color(45,45,45));
  for(int i=0;i<4;i++)
  {
    b2.setPosition(current[i].first * 20,current[i].second * 20);
    window.draw(b2);
  }
}

void set_matrix()
{
  for(int i=0;i<4;i++)
  {
    matrix[current[i].second - 13][current[i].first - 1].coordinate = current[i];
    applycolor(current[i].second - 13 , current[i].first - 1);
    //matrix[current[i].second - 13][current[i].first - 1].c =current_color;// depends
  }
  check_for_deletion();
  //cout<<"ran check_for_deletion"<<endl;
}
void applycolor(int x,int y)
{
  if(current_color==0)//T Color
  {
    matrix[x][y].c=sf::Color(255,255,0);
  }
  else if(current_color==1)//I Color
  {
    matrix[x][y].c=sf::Color(0,0,255);
  }
  else if(current_color==2)//Z Color
  {
    matrix[x][y].c=sf::Color(255,0,0);
  }
  else if(current_color==3)//S Color
  {
    matrix[x][y].c=sf::Color(255,128,0);
  }
  else if(current_color==4)//L Color
  {
    matrix[x][y].c=sf::Color(128,255,0);
  }
  else if(current_color==5)//O Color
  {
    matrix[x][y].c=sf::Color(0,255,255);
  }
  else if(current_color==6)//J Color
  {
    matrix[x][y].c=sf::Color(255,51,255);
  }
}
void display_matrix()
{
  for(int i=0;i<30;i++)
  {
    for(int j=0;j<20;j++)
    {
      b.setPosition(matrix[i][j].coordinate.first * 20,matrix[i][j].coordinate.second * 20);
      b.setFillColor(matrix[i][j].c);
      b.setOutlineThickness(2);
      b.setOutlineColor(sf::Color(64,64,64));
      window.draw(b);
    }
  }
}
bool present_next()
{
  for(int i=0;i<4;i++)
  {
    if(matrix[min(current[i].second-13 + 1,29)][current[i].first-1].coordinate.first!=-20 && matrix[min(current[i].second-13 + 1,29)][current[i].first-1].coordinate.second!=-20)
    {
      return true;
    }
  }
  return false;
}
void applyfallingcolor()
{
  if(current_color==0)
  {
    b2.setFillColor(sf::Color(255,255,0));
  }
  else if(current_color==1)
  {
    b2.setFillColor(sf::Color(0,0,255));
  }
  else if(current_color==2)
  {
    b2.setFillColor(sf::Color(255,0,0));
  }
  else if(current_color==3)
  {
    b2.setFillColor(sf::Color(255,128,0));
  }
  else if(current_color==4)
  {
    b2.setFillColor(sf::Color(128,255,0));
  }
  else if(current_color==5)
  {
    b2.setFillColor(sf::Color(0,255,255));
  }
  else if(current_color==6)
  {
    b2.setFillColor(sf::Color(255,51,255));
  }
  b2.setOutlineThickness(2);
  b2.setOutlineColor(sf::Color(64,64,64));
}

void rotate()
{
  if(rot == 'C')
  {
    for(int i=0;i<4;i++)
    {
      if(i!=1)
      {
        int tmp_x = current[i].first;
        int tmp_y = current[i].second;
        current[i].first = current[1].first - (tmp_y) + current[1].second;
        current[i].second= current[1].second + (tmp_x) - current[1].first;
      }
    }

  }
  else if(rot == 'A')
  {
    for(int i=0;i<4;i++)
    {
      if(i!=1)
      {
        int tmp_x = current[i].first;
        int tmp_y = current[i].second;
        current[i].first = current[1].first + (tmp_y) - current[1].second;
        current[i].second= current[1].second - (tmp_x) + current[1].first;
      }
    }
  }
  rot=' ';
}
bool no_hindarance()
{
  int flag=0;
  for(int i=0;i<4;i++)
  {
    if(x_speed==1)
    {
      if(matrix[current[i].second-13][min(current[i].first-1 + 1,20)].coordinate.first!=-20 && matrix[current[i].second-13][min(current[i].first-1 + 1,20)].coordinate.second!=-20)
      {
        return false;
      }
    }
    else if(x_speed==-1)
    {
      if(matrix[current[i].second-13][max(current[i].first-1-1,0)].coordinate.first!=-20 && matrix[current[i].second-13][max(current[i].first-1-1,0)].coordinate.second!=-20)
      {
        return false;
      }
    }
  }
  return true;
}
void check_game_status()
{
  int flag=1;
  for(int i=0;i<20;i++)
  {
    if(matrix[0][i].coordinate.first!=-20 && matrix[0][i].coordinate.second!=-20)
    {
      flag=0;
      break;
    }
  }
  if(flag==0)
    game_play=0;
}

void Replay()
{
  in_progress=0;
  current_color=-1;
  total_blocks=0;
  for(int i=0;i<30;i++)
  {
    for(int j=0;j<20;j++)
    {
      matrix[i][j]=Block();
    }
  }
  game_play=1;
}

void check_for_deletion()
{
  int f=0;
  int k=0;
  for(int i=0;i<30;i++)
  {
    for(int j=0;j<20;j++)
    {
      if(matrix[i][j].coordinate.first==-20 && matrix[i][j].coordinate.second==-20)
      {
        //cout<<"no deletion: "<<k++<<endl;
        f=1;
        break;
      }
      // else
      // {
      //   //cout<<"( "<<matrix[i][j].coordinate.first<<" , "<<matrix[i][j].coordinate.second<<" )"<<endl;
      // }
    }
    if(f==0)
    {
      //cout<<"deletion required"<<endl;
      rearrange_matrix(i);
      break;
    }
    f=0;
  }
}
void rearrange_matrix(int x)
{
  for(int i=x-1;i>=0;i--)
  {
    for(int j=0;j<20;j++)
    {
      //cout<<"original : ( "<<matrix[i+1][j].coordinate.first<<" , "<<matrix[i+1][j].coordinate.second<<" )"<<" : ";
      matrix[i+1][j].coordinate.first = matrix[i][j].coordinate.first;
      matrix[i+1][j].coordinate.second = matrix[i][j].coordinate.second+1;
      matrix[i+1][j].c = matrix[i][j].c;
      //cout<<"updated : ( "<<matrix[i+1][j].coordinate.first<<" , "<<matrix[i+1][j].coordinate.second<<" )"<<endl;
    }
  }
  for(int i=0;i<20;i++)
  {
    matrix[0][i] = Block();
  }
}
