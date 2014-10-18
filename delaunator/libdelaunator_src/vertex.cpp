#include "vertex.h"
#include "edge.h"

unsigned int Vertex::last_id = 1;


/***************************************************
 * CONSTRUCTOR
 ***************************************************/
/**
 * Creat a new Vertex.
 * @param x coordinate
 * @param y coordinate
 * @param edge reference to an Edge that have this new Vertex for origin
 * @return a new and well initialized Vertex
 */
Vertex::Vertex(const float x, const float y, Edge* edge) : Coordinates(x,y), edge(edge) {
        this->id = this->last_id++;
        if(this->edge != NULL)  this->edge->setOriginVertex(this);
}

/**
 * Creat a new Vertex.
 * @param c Coordinates that defined position of new Vertex
 * @param edge reference to an Edge that have this new Vertex for origin
 * @return a new and well initialized Vertex
 */
Vertex::Vertex(const Coordinates c, Edge* edge) : Coordinates(c), edge(edge) {
        this->id = this->last_id++;
}




/***************************************************
 * PUBLIC METHODS
 ***************************************************/
/*
 * @return number of neighbour directly connected by an edge
 */
unsigned int Vertex::neighbourCount() const {
        unsigned int counter = 0;
        Edge* edge = this->edge;
        do {
                counter++;
                edge = edge->rotLeftEdge();
        } while(edge != this->edge);
        return counter;
}



/**
 * @param v a Vertex
 * return true iff an edge have for origin this and for destination v
 */
bool Vertex::isNeighbourOf(Vertex* v) const {
        Edge* finder = this->edge;
        bool isNei = false;
        do {
                if(finder->destinVertex() == v)
                        isNei = true;
                finder = finder->rotLeftEdge();
        } while(!isNei && finder != this->edge);
        return isNei;
}



/**
 * @param v a Vertex that will receive all TrianguledObject of this
 */
void Vertex::giveTrianguledObjectsTo(Vertex* v) {
        std::list<TrianguledObject*>::iterator it = v->objects.begin();
        v->objects.insert(it, this->objects.begin(), this->objects.end());
}



/**
 * @param trob a TrianguledObject that will be added in list of this
 */
void Vertex::take(TrianguledObject* trob) {
        this->objects.push_back(trob);
}



/**
 * @param trob a TrianguledObject that will be forgeted by this
 * @note: if this have not trob in memory, nothing is done by that function.
 */
void Vertex::forget(TrianguledObject* trob) {
        this->objects.remove(trob);
}









/***************************************************
 * OPERATORS
 ***************************************************/
/**
 * @param othr other Vertex that equality is tested
 * @return true iff given vertex have the same coordinates of this
 */
bool Vertex::operator==(const Vertex& othr) {
        return Coordinates::operator==(othr) && this->edge == othr.edge;
}








/***************************************************
 * ACCESSORS
 ***************************************************/
/**
 * Access to all TrianguledObjects of this instance.
 * @return a std::list of TrianguledObject pointers
 */
std::list<TrianguledObject*> Vertex::getObjects() const {
        std::list<TrianguledObject*> l(this->objects); // recopy
        return l;
}


/**
 * Access to one TrianguledObject of this instance.
 * @return a TrianguledObject pointer, or NULL if no Object referenced
 */
TrianguledObject* Vertex::getFirstObject() const {
        return this->objects.front() != NULL ? this->objects.front() : NULL;
}



/**
 * @return an Edge that had this Vertex for origin
 */
Edge* Vertex::getEdge() const { return this->edge; }
/**
 * @param e the reference to an Edge, the new Edge that have this Vertex for origin
 */
void Vertex::setEdge(Edge* e) { this->edge = e; }


/**
 * @return true iff this is referenced by triangulation and is one of the four corner vertice
 */
bool Vertex::isACorner() const {
        return this->getID() <= 4;
}




/***************************************************
 * PRIVATE METHODS
 ***************************************************/



/***************************************************
 * EXTERNAL METHODS
 ***************************************************/
#if !SWIG
std::ostream& operator<<(std::ostream& flux, Coordinates const &c) {
        return flux << "(" << c.x() << ";" << c.y() << ")";
}
std::ostream& operator<<(std::ostream& flux, Vertex const &c) {
        return flux << "(" << c.x() << ";" << c.y() 
                << (c.getEdge() != NULL ? ";linked)" : ";free)");
}
#endif



