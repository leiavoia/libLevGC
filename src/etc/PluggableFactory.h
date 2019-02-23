#ifndef PLUGGABLEFACTORY_H
#define PLUGGABLEFACTORY_H

#include <map>
#include <string>
#include <iostream>


namespace LGC {

// Pluggable Factory Template:
template <class OBJECT> class PluggableFactory {
	public:
		virtual ~PluggableFactory() { }
		
		static OBJECT* GetNew( const std::string& s ) {
			FactoryMapIter i = registry->find(s);
			if ( i == registry->end() ) { return NULL; }
			OBJECT* obj = i->second->CreateNew();
			if (!obj) { return NULL; }
			return obj;
			}
		
	protected:
		// this should only be used by a static instance of the inherited class
		PluggableFactory( const std::string& s ) {
			// make secure from static initialization order problems of the registry (needed?):
			static bool inited __attribute__ ((unused)) = Init();
			registry->insert(  std::make_pair( s, this )  );
			}
		
		// the factory's personal create-object function
		virtual OBJECT* CreateNew()=0;
			
	private:
		typedef std::map< std::string, PluggableFactory<OBJECT>* > FactoryMap;
		typedef typename FactoryMap::iterator FactoryMapIter;
			
		static FactoryMap* registry;
		
		static bool Init() {
			registry = new FactoryMap();
			return true;
			}
	};


template <class OBJECT> std::map<std::string,PluggableFactory<OBJECT>*>* PluggableFactory<OBJECT>::registry;




/*
MAKING IT WORK: ==========================================================


// CREATE A FACTORY FOR EACH OBJECT TYPE YOU WANT TO MANUFACTURE:

	class CircleFactory : public PluggableFactory<Shape> {
		public:
			~CircleFactory() {}
		protected:
			CircleFactory() : PluggableFactory<Shape>("Circle") {}
			Shape* CreateNew() { return new Circle(); }
			static const CircleFactory auto_register_me;  
		};
	
	const CircleFactory CircleFactory::auto_register_me;


// NOW USE IT:

	Shape* s = PluggableFactory<Shape>::GetNew("Circle");


// FEEL FREE TO TYPEDEF IT TOO:

	typedef PluggableFactory<Shape> ShapeFactory;
	
	
============================================================================
*/






//
// THIS IS THE 'ANOTATED' VERSION WITH DEBUGGING:
//
//Pluggable Factory Template:
// template <class OBJECT> class PluggableFactory {
// 	public:
// 		typedef std::map< std::string, PluggableFactory<OBJECT>* > FactoryMap;
// 		typedef typename FactoryMap::iterator FactoryMapIter;
// 		
// 		virtual ~PluggableFactory() { }
// 		
// 		static OBJECT* GetNew( const std::string& s ) {
// 			std::cout << "PluggableFactory::GetNew(): registry now contains [" << registry->size() << "] entries:" << std::endl;
// 			FactoryMapIter i = registry->find(s);
// 			if ( i == registry->end() ) { 
// 				std::cout << "PluggableFactory:: Can't find a Factory to make a " << s << std::endl;
// 				return NULL;
// 				}
// 			OBJECT* obj = i->second->CreateNew();
// 			if (!obj) { return NULL; }
// 			return obj;
// 			}
// 		
// 	protected:
// 
// 		// this should only be used by a static instance of the inherited class
// 		PluggableFactory( const std::string& s ) {
// 			// make secure from static initialization order problems of the registry (needed?):
// 			static bool inited __attribute__ ((unused)) = Init();
// 			std::cout << "PluggableFactory:: Attempting to register a new factory: " << s << std::endl;
// 			registry->insert(  std::make_pair( s, this )  );
// 			std::cout << "PluggableFactory:: registry now contains [" << registry->size() << "] entries:" << std::endl;
// 			// spit out all names
// 			for ( FactoryMapIter i = registry->begin(); i != registry->end(); i++ ) {
// 				std::cout << "     " << i->first << std::endl;
// 				}
// 			}
// 		
// 		// the factory's personal create-object function
// 		virtual OBJECT* CreateNew()=0;
// 			
// 	private:
// 	
// 		static FactoryMap* registry;
// 		
// 		static bool Init() {
// 			registry = new FactoryMap();
// 			return true;
// 			}
// 	};
// 
// 
// template <class OBJECT> std::map<std::string,PluggableFactory<OBJECT>*>* PluggableFactory<OBJECT>::registry;



} // end namespace LGC 

#endif



