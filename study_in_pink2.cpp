#include "study_in_pink2.h"

////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
/// Complete the following functions
/// DO NOT modify any parameters in the functions.
////////////////////////////////////////////////////////////////////////

///Function
//This function for check the stat
static int statcheck(int num_check, int max_stat) {
    return (num_check > max_stat) ? max_stat : ((num_check < 0) ? 0 : num_check);
}



///Forward Declaration for Static variables
//Static variable declaration of Sherlock
int Sherlock::hp = 0;
int Sherlock::exp = 0;

//Static variable declaration of Watson
int Watson::hp = 0;
int Watson::exp = 0;



/// Map Element
MapElement::MapElement(ElementType in_type) {
    type = in_type;
}

MapElement::~MapElement(){}

ElementType MapElement::getType() const
{
    return type;
}

Path::Path() :MapElement(PATH) {}

Wall::Wall() :MapElement(WALL) {}

int FakeWall::getReqExp() const {
    return req_exp;
}

FakeWall::FakeWall(int in_req_exp) :MapElement(FAKE_WALL) {
    req_exp = in_req_exp;
}



/// Map
Map::Map(int num_rows, int num_cols, int num_walls, Position* array_walls, int num_fake_walls, Position* array_fake_walls): num_rows(num_rows), num_cols(num_cols)
{
    //Khởi tạo mảng 2 chiều without constant variables
    MapElement** map = new MapElement* [num_rows];
    for (int i = 0; i < num_rows; i++) {
        map[i] = new MapElement[num_cols];
        for (int j = 0; j < num_cols; j++) {
            map[i][j] = Path();
        }
    }
    for (int i = 0; i < num_walls; i++) {
        Position wall = array_walls[i];
        map[wall.getRow()][wall.getCol()] = Wall();
    }
    for (int i = 0; i < num_fake_walls; i++) {
        Position fw = array_fake_walls[i];
        int r = fw.getRow(), c = fw.getCol();
        map[r][c] = FakeWall((r * 257 + c * 139 + 89) % 900 + 1);
    }
}

Map::~Map(){
    for (int i = 0; i < num_rows; i++)
        delete[] map[i];
    delete[] map;
}

bool Map::isValid(const Position& pos, MovingObject* mv_obj) const
{
    string name = (*mv_obj).getname();
    bool valid = true;
    int r = pos.getRow(), c = pos.getCol(), 
        req_exp = (r * 257 + c * 139 + 89) % 900 + 1;
    if ((name == "Watson") && (map[r][c].getType() == FAKE_WALL)) {
        valid = Watson::exp > req_exp ? true : false;
    }
    else {
        valid = ((map[r][c].getType() == WALL) || (r > num_rows) || (r < 0) || (c > num_cols) || (c < 0)) ? false : true;
    }
    return valid;
}



/// Position
Position::Position(int r, int c)
{
    setRow(r);
    setCol(c);
}

const Position Position::npos = Position(-1, -1);

Position::Position(const string& str_pos) {
    setRow(stoi(str_pos.substr(1, str_pos.find(","))));
    setCol(stoi(str_pos.substr(str_pos.find(",") + 1)));
}
int Position::getRow() const{
    return r;
}

int Position::getCol() const{
    return c;
}

void Position::setRow(int r){
    this->r = r;
}

void Position::setCol(int c){
    this->c = c;
}

string Position::str() const{
    return "(" + to_string(r) + "," + to_string(c) + ")";
}

bool Position::isEqual(int in_r, int in_c) const
{
    return ((r == in_r) && (c == in_c));
}



/// Moving Object
MovingObject::MovingObject(int index, const Position pos, Map * map, const string& name)
{
    this->index = index;
    this->pos = pos;
    this->map = map;
    this->name = name;
}

string MovingObject::getname() const{
    return name;
}

MovingObject::~MovingObject(){}

Position MovingObject::getCurrentPosition() const {
    return pos;
}



/// Sherlock
Sherlock::Sherlock(int index, const string& moving_rule, const Position& init_pos, Map* map, int init_hp, int init_exp) : MovingObject(index, init_pos, map, "Sherlock")
{
    set_moving_rule(moving_rule);
    setpos(init_pos);
    this->init_hp = statcheck(init_hp, 500);
    this->init_exp = statcheck(init_exp, 900);
    hp = statcheck(init_hp, 500);
    exp = statcheck(init_exp, 900);
}


//Set and Get
int Sherlock::get_index() const
{
    return index;
}

void Sherlock::set_index(int index)
{
    this->index = index;
}

Position Sherlock::getpos() const
{
    return cur_pos;
}

void Sherlock::setpos(Position cur_pos)
{
    this->cur_pos = cur_pos;
}

int Sherlock::getk() const {
    return k;
}

void Sherlock::setk(int k) {
    this->k = k;
}

string Sherlock::get_moving_rule() const {
    return moving_rule;
}

void Sherlock::set_moving_rule(string moving_rule) {
    this->moving_rule = moving_rule;
}




//Method
Position Sherlock::getNextPosition()
{
    Position next_pos;
    int n = get_moving_rule().length();
    string moves = get_moving_rule().substr(getk() % n);
    int r = getpos().getRow(), c = getpos().getCol();
    setk(getk() + 1);
    //For check what move is it
    if (moves == "L") {
        next_pos = Position(r, c - 1);
    }
    else if (moves == "R") {
        next_pos = Position(r, c + 1);
    }
    else if (moves == "U") {
        next_pos = Position(r - 1, c);
    }
    else if (moves == "D") {
        next_pos = Position(r + 1, c);
    }

    //For check the position is wall or not
    MovingObject* mv_obj = new Sherlock(get_index(), get_moving_rule(), next_pos, map, Sherlock::hp, Sherlock::exp);
    if (!map->isValid(next_pos, mv_obj)) {
        next_pos = Position::npos;
    }
    delete mv_obj;
    return next_pos;
}

void Sherlock::move() {
    Position next_pos = getNextPosition();
    int r = next_pos.getRow(), c = next_pos.getCol();
    if ((r == -1) && (c == -1)) {
        setpos(getpos());
    }
    else {
        setpos(next_pos);
    }
}

string Sherlock::str() const {
    return "Sherlock[index=" + to_string(get_index()) + "];pos=" +
        getpos().str() + "];moving_rule=" + get_moving_rule() + "]";
}



///Watson
Watson::Watson(int index, const string& moving_rule, const Position& init_pos, Map* map, int init_hp, int init_exp) :MovingObject(index, init_pos, map, "Watson")
{
    set_moving_rule(moving_rule);
    setpos(init_pos);
    this->init_hp = statcheck(init_hp, 500);
    this->init_exp = statcheck(init_exp, 900);
    hp = statcheck(init_hp, 500);
    exp = statcheck(init_exp, 900);
}


//Set and Get
int Watson::get_index() const
{
    return index;
}

void Watson::set_index(int index)
{
    this->index = index;
}

Position Watson::getpos() const
{
    return cur_pos;
}

void Watson::setpos(Position cur_pos)
{
    this->cur_pos = cur_pos;
}

int Watson::getk() const {
    return k;
}

void Watson::setk(int k) {
    this->k = k;
}

string Watson::get_moving_rule() const {
    return moving_rule;
}

void Watson::set_moving_rule(string moving_rule) {
    this->moving_rule = moving_rule;
}




//Method
Position Watson::getNextPosition()
{
    Position next_pos;
    int n = get_moving_rule().length();
    string moves = get_moving_rule().substr(getk() % n);
    int r = getpos().getRow(), c = getpos().getCol();
    setk(getk() + 1);
    //For check what move is it
    if (moves == "L") {
        next_pos = Position(r, c - 1);
    }
    else if (moves == "R") {
        next_pos = Position(r, c + 1);
    }
    else if (moves == "U") {
        next_pos = Position(r - 1, c);
    }
    else if (moves == "D") {
        next_pos = Position(r + 1, c);
    }

    //For check the position is wall or not
    MovingObject* mv_obj = new Watson(get_index(), get_moving_rule(), next_pos, map, Watson::hp, Watson::exp);
    if (!map->isValid(next_pos, mv_obj)) {
        next_pos = Position::npos;
    }
    delete mv_obj;
    return next_pos;
}

void Watson::move() {
    Position next_pos = getNextPosition();
    int r = next_pos.getRow(), c = next_pos.getCol();
    if ((r == -1) && (c == -1)) {
        setpos(getpos());
    }
    else {
        setpos(next_pos);
    }
}

string Watson::str() const {
    return "Watson[index=" + to_string(get_index()) + "];pos=" +
        getpos().str() + "];moving_rule=" + get_moving_rule() + "]";
}



///Criminal
Criminal::Criminal(int index, const Position& init_pos, Map* map, Sherlock* sherlock, Watson* watson) :MovingObject(index, init_pos, map, "Criminal") {
    this->sherlock = sherlock;
    this->watson = watson;
    setpos(init_pos);
}

//Get and Set
Position Criminal::getpos() const {
    return cur_pos;
}

void Criminal::setpos(Position cur_pos) {
    this->cur_pos = cur_pos;
}

Sherlock* Criminal::getsher() const
{
    return sherlock;
}

void Criminal::setsher(Sherlock* sherlock)
{
    this->sherlock = sherlock;
}

Watson* Criminal::getwatson() const
{
    return watson;
}

void Criminal::setwatson(Watson* watson)
{
    this->watson = watson;
}




//Method
Position Criminal::getNextPosition() {

}


////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////

