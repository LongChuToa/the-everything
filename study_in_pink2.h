/*
* Ho Chi Minh City University of Technology
* Faculty of Computer Science and Engineering
* Initial code for Assignment 1
* Programming Fundamentals Spring 2023
* Author: Vu Van Tien
* Date: 02.02.2023
*/

//The library here is concretely set, students are not allowed to include any other libraries.
#ifndef _H_STUDY_IN_PINK_2_H_
#define _H_STUDY_IN_PINK_2_H_

#include "main.h"

////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
/// Complete the following functions
/// DO NOT modify any parameters in the functions.
////////////////////////////////////////////////////////////////////////

// Forward declaration
class MovingObject;
class Position;
// class Configuration;
class Map;

// class Criminal;
// class RobotS;
// class RobotW;
// class RobotSW;
// class RobotC;

// class ArrayMovingObject;
// class StudyPinkProgram;

// class BaseItem;
// class BaseBag;
// class SherlockBag;
// class WatsonBag;

class TestStudyInPink;

enum ItemType { MAGIC_BOOK, ENERGY_DRINK, FIRST_AID, EXCEMPTION_CARD, PASSING_CARD };
enum ElementType { PATH, WALL, FAKE_WALL };
enum RobotType { C=0, S, W, SW };

class MapElement {
protected:
    ElementType type;
public:
    MapElement(ElementType in_type);
    MapElement() = default;
    virtual ~MapElement();
    virtual ElementType getType() const;
};

class Path : public MapElement {
public:
    Path();
};

class Wall : public MapElement {
public:
    Wall();
};

class FakeWall : public MapElement {
private:
    int req_exp;
public:
    FakeWall(int in_req_exp);
    int getReqExp() const;
};


class Map {
private:
    int num_rows, num_cols;
    MapElement** map;
public:
    Map (int num_rows, int num_cols, int num_walls, Position * array_walls, int num_fake_walls, Position * array_fake_walls);
    ~Map();

    //Get num_rows and num_cols
    int get_num_rows() const { return num_rows; };
    int get_num_cols() const { return num_cols; };

    bool isValid ( const Position & pos , MovingObject * mv_obj ) const ;
};

class Position {
private:
    int r, c;
public:
    static const Position npos;


    Position(int r=0, int c=0);

    Position(const string & str_pos);

    int getRow() const;
    int getCol() const;
    void setRow(int r);
    void setCol(int c);

    string str() const;

    bool isEqual(int in_r, int in_c) const;
};

class MovingObject {
protected:
    int index;
    Position pos;
    Map * map;
    string name;

public:
    //New here
    string getname() const;

    //Original here
    MovingObject(int index, const Position pos, Map * map, const string & name="");
    MovingObject() = default;
    virtual ~MovingObject();
    virtual Position getNextPosition() = 0;
    Position getCurrentPosition() const;
    virtual void move() = 0;
    virtual string str() const = 0;
};

class Sherlock: public MovingObject {
private:
    int index;
    string moving_rule;
    Position cur_pos;
    int init_hp, init_exp, k;
public:
    //Static variable (because this variable change very much in game)
    static int hp;
    static int exp;

    Sherlock(int index, const string& moving_rule, const Position& init_pos, Map* map, int init_hp, int init_exp);
    
    //Set and Get
    //For index
    int get_index() const;
    void set_index(int index);

    //For pos
    Position getpos() const;
    void setpos(Position cur_pos);

    //For k
    int getk() const;
    void setk(int k);
    
    //For moving_rule
    string get_moving_rule() const;
    void set_moving_rule(string moving_rule);

    // Get Next Position
    Position getNextPosition();

    // Moving
    void move();
    
    // str
    string str() const;
    // ...
};

class Watson :public MovingObject {
private:
    int index;
    string moving_rule;
    Position cur_pos;
    int init_hp, init_exp, k;
public:
    //Static variable (because this variable change very much in game)
    static int hp;
    static int exp;

    Watson(int index, const string& moving_rule, const Position& init_pos, Map* map, int init_hp, int init_exp);

    //Set and Get
    //For index
    int get_index() const;
    void set_index(int index);

    //For pos
    Position getpos() const;
    void setpos(Position cur_pos);

    //For k
    int getk() const;
    void setk(int k);

    //For moving_rule
    string get_moving_rule() const;
    void set_moving_rule(string moving_rule);

    // Get Next Position
    Position getNextPosition();

    // Moving
    void move();

    // str
    string str() const;

    // 
};

class Criminal : public MovingObject {
private:
    int index;
    Position cur_pos;
    Sherlock* sherlock;
    Watson* watson;
public: 
    Criminal(int index, const Position& init_pos, Map* map, Sherlock* sherlock, Watson* watson);

    //Get and Set
    //For pos
    Position getpos() const;
    void setpos(Position cur_pos);

    Sherlock* getsher() const;
    void setsher(Sherlock* sherlock);

    Watson* getwatson() const;
    void setwatson(Watson* watson);

    // Get Next Position
    Position getNextPosition();

    // move
    // str
    // ...
};

class ArrayMovingObject {
private:
    // TODO

public:
    ArrayMovingObject(int capacity);

    ~ArrayMovingObject() ;
    bool isFull() const;
    bool add(MovingObject * mv_obj);
    MovingObject * get(int index) const;
    int size() const; // return current number of elements in the array
    string str() const;
};

class Configuration {
    friend class StudyPinkProgram;

private:
    // TODO

public:
    Configuration(const string & filepath);
    ~Configuration();
    string str() const;
};

// Robot, BaseItem, BaseBag,...

class StudyPinkProgram {
private:
    // Sample attributes
    Configuration * config;
    
    Sherlock * sherlock;
    Watson * watson;
    Criminal * criminal;
    
    Map * map;
    ArrayMovingObject * arr_mv_objs;


public:
    StudyPinkProgram(const string & config_file_path);

    bool isStop() const;

    void printResult() const {
        if (sherlock->getCurrentPosition().isEqual(criminal->getCurrentPosition())) {
            cout << "Sherlock caught the criminal" << endl;
        }
        else if (watson->getCurrentPosition().isEqual(criminal->getCurrentPosition())) {
            cout << "Watson caught the criminal" << endl;
        }
        else {
            cout << "The criminal escaped" << endl;
        }
    }

    void printStep(int si) const {
        cout << "Step: " << setw(4) << setfill('0') << si
            << "--"
            << sherlock->str() << "--|--" << watson->str() << "--|--" << criminal->str() << endl;
    }

    void run(bool verbose) {
        // Note: This is a sample code. You can change the implementation as you like.
        // TODO
        for (int istep = 0; istep < config->num_steps; ++istep) {
            for (int i = 0; i < arr_mv_objs->size(); ++i) {
                arr_mv_objs->get(i)->move();
                if (isStop()) {
                    printStep(istep);
                    break;
                }
                if (verbose) {
                    printStep(istep);
                }
            }
        }
        printResult();
    }

    ~StudyPinkProgram();
};

////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////
#endif /* _H_STUDY_IN_PINK_2_H_ */
