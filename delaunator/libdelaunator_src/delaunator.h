#ifndef DELAUNATOR_TRIANGULATION_H_INCLUDED
#define DELAUNATOR_TRIANGULATION_H_INCLUDED



/*
 * LIBRARIES
 */
// LOCAL MODULES
#include "commons.h"
#include "geometry.h"
#include "vertex.h"
#include "face.h"
#include "edge.h"
#include "iterators.h"



/*
 * DEFINES
 */



/*
 * PREDECLARATIONS
 */

/*
 * For some explanations on quad-edge implementation :
 * http://totologic.blogspot.fr/2013/11/core-quad-edge-implementation-explained.html
*/





/**
 * Delaunay class definition.  
 * Main object manipulated by user/wrapper.  
 * Provide iterators, access to Vertices, Faces, Edges,...  
 * Have all methods for add, delete and move Vertices.
 */
class Delaunator {
	public:
        // INTERNAL CLASS TYPES
        /**
         * Finder mode for when triangulation looking for the Face that contain Coordinates 
         *   (notabily for Vertex adding).  
         * Random: random Edge in all existing in Delaunator instance
         * First: the first added Edge
         * Middle: the Edge in the middle of the Edge list
         * Last: the last added Edge
         * @note Last mode is certainly the better, especially in case where added Vertex are close to previous one
         */
        enum FinderInitial {
                FINDER_INITIAL_RANDOM,
                FINDER_INITIAL_FIRST,
                FINDER_INITIAL_MIDDLE,
                FINDER_INITIAL_LAST
        };

	// CONSTRUCTOR
		Delaunator(const float, const float, 
                           const float, const float, Delaunator::FinderInitial = Delaunator::FINDER_INITIAL_LAST);
		~Delaunator();
	// PUBLIC METHODS
                Vertex* addVertexAt(Coordinates);
                Vertex* addVertexAt(float x, float y) { return this->addVertexAt(Coordinates(x, y)); }
                Vertex* vertexAt(float, float, float) const;
                Vertex* vertexAt(Coordinates c, float p) const { return this->vertexAt(c.x(), c.y(), p); }
                void moveVertex(Vertex* v, float x, float y);
                void moveVertexTo(Vertex* v, Coordinates c);
                void delVertex(Vertex* v);
#ifdef DEBUG // some tests with assertions
                void DEBUG_tests() const;
#endif
	// ACCESSORS
                unsigned int getIndexOf(Vertex*) const;
                std::vector<Edge*> getEdges()  const { return this->edges; }
                unsigned int getVerticeCount() const { return this->vertices.size(); }
                float getXmin() const { return this->xmin; }
                float getXmax() const { return this->xmax; }
                float getYmin() const { return this->ymin; }
                float getYmax() const { return this->ymax; }
                float epsilon() const { return EPSILON; }

        // PREDICATS
                bool haveVertex(Vertex*) const;
                bool isCornerVertex(Vertex*) const;
                bool isExternalEdge(Edge*) const;
                bool collideAt(Coordinates) const;
#ifdef DEBUG
                bool opt_isdebug()       const { return true; }
#else
                bool opt_isdebug()       const { return false; }
#endif
#ifdef FOLLOW_SEARCH
                bool opt_follow_search() const { return true; }
#else
                bool opt_follow_search() const { return false; }
#endif


        // ITERATORS
                IteratorVertexToNeighbourVertices getNeighbors(Vertex*);
                IteratorOnAllEdges allEdges()           { return IteratorOnAllEdges(&this->edges); }
                IteratorOnAllEdges_read allEdges_read() const
                                                        { return IteratorOnAllEdges_read(&this->edges); }
                IteratorOnAllFaces allFaces()           { return IteratorOnAllFaces(&this->faces); }
                IteratorOnAllFaces_read allFaces_read() const
                                                        { return IteratorOnAllFaces_read(&this->faces); }
                IteratorOnAllVertices allVertices()     { return IteratorOnAllVertices(&this->vertices); }
                IteratorOnAllVertices_read allVertices_read() const 
                                                        { return IteratorOnAllVertices_read(&this->vertices); }




// PRIVATE
	private:
	// ATTRIBUTES
                float xmin, xmax, ymin, ymax;
                FinderInitial finder_mode;
                std::vector<Vertex*> vertices;
                std::vector<Edge*> edges;
                std::vector<Face*> faces;
	// PRIVATE METHODS
                Face* findContainerOf(Coordinates) const;
#ifdef DEBUG
                bool applyDelaunayCondition(Face*, unsigned int ttl = 0);
#else
                bool applyDelaunayCondition(Face*);
#endif
                void operateFlip(Edge*);
                // Methods for manipulate lists of components
                inline void removeVertexFromVertices(Vertex* v) {
                        for(std::vector<Vertex*>::iterator it = this->vertices.begin(); 
                                        it != this->vertices.end(); it++) {
                                if((*it) == v) {
                                        this->vertices.erase(it);
                                        delete v;
                                        it = this->vertices.end()-1;
                                }
                        }
                }
                inline void removeEdgeFromEdges(Edge* e) {
                        for(std::vector<Edge*>::iterator it = this->edges.begin(); 
                                        it != this->edges.end(); it++) {
                                if((*it) == e) {
                                        this->edges.erase(it);
                                        delete e;
                                        it = this->edges.end()-1;
                                }
                        }
                }
                inline void removeFaceFromFaces(Face* f) {
                        for(std::vector<Face*>::iterator it = this->faces.begin(); 
                                        it != this->faces.end(); it++) {
                                if((*it) == f) {
                                        this->faces.erase(it);
                                        delete f;
                                        it = this->faces.end()-1;
                                }
                        }
                }
                /*
                 * Replace given vertex coords by given values.
                 */
                inline void moveVertex_pure(Vertex* v, Coordinates new_p) {
                // Move the vertex
                        v->setX(new_p.x());
                        v->setY(new_p.y());
                // Apply Delaunay Condition
                        std::vector<Face*> nei_faces;
                        Edge* edge = v->getEdge();
                        do {
                                nei_faces.push_back(edge->leftFace());
                                edge = edge->rotLeftEdge();
                        } while(edge != v->getEdge());

                        for(Face* f : nei_faces) {
#ifdef DEBUG
                                assert(f != NULL);
#endif
                                this->applyDelaunayCondition(f);
                        }
                }
};



#endif
