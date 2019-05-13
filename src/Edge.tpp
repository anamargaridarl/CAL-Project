template <class T>
Edge<T>::Edge(Vertex<T> *d, Vertex<T>* s, double w): dest(d), weight(w) , src(s){}
template <class T>
Edge<T>::Edge(Vertex<T> *d, double w): dest(d), weight(w) {}
template <class T>
Vertex<T>* Edge<T>::getDest() {
    return this->dest;
}
