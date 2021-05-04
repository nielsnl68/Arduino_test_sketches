/*
 * the following sketch is a proto of a chainable class system.
 * with on function in the Impl class you can hook that class 
 * to his predecessor class.  
 * This system will be used to create a new I2S library where  
 * you can link (chain) different proccesses together to receive
 * sound from different sources decode/encode them end send the
 * result out again.
 * This example shows also a way to interact with each other with
 * the use of an interface class.
 */


class Interface
{
  public:
  virtual void callback() = 0;
};

class Chain {
    private:
       Interface& myCallback;
       Chain *m_prev= nullptr;
       Chain *m_next= nullptr;

    public: 
      
      Chain (Interface& newCallback) : myCallback(newCallback)  { }      
      void __prev(Chain *prev) { 
         m_prev = prev; 
      }
      void __next(Chain *next) { 
        if (m_next) {
           m_next->__prev(nullptr);
        }
        m_next = next; 
        if (m_next) {
           m_next->__prev(this);
        }

      }
      void clear() {
        if (m_prev){
          m_prev->__next(NULL);
        }
      }
      void display() { myCallback.callback();}
      void displayAll() {
        if (m_prev == nullptr) {Serial.printf("%8s","");} else m_prev->display();
        display();
        if (m_next == nullptr) {Serial.printf("%8s","");} else m_next->display();
        Serial.println();
      }
};


class Impl : public Interface
{
  private:
  Chain *m_impl= nullptr;
  char * m_name="";
  public:
  Impl(char* name) {
    m_name = name;
    m_impl =new Chain((*this));
  }
  virtual void callback() 
  {
    Serial.printf("%8s",m_name); 
  }
  void display() {
    m_impl->displayAll();
  }
  Chain *testing(Chain *next = nullptr){
    if (next != nullptr)
        m_impl->__next(next);
    return m_impl;
  }
};

void setup() {
  
  Serial.begin(115200);
  delay(1000);
  Serial.println("_---_"); 
  Impl a("anton");
   a.display();
   Serial.println("add Bernard:"); 
   
   Impl b("Bernard");
   a.testing(b.testing());
   a.display();
   b.display();
   
   Serial.println("add Coranel:"); 
   Impl c("Coranel");
   b.testing(c.testing());
   a.display();   
   b.display();
   c.display();
   
   Serial.println("clear test:"); 
   c.testing()->clear();
   a.display();   
   b.display();
   c.display();

   Serial.println("replace Daniel with Bernard:"); 
   Impl d("Daniel");
   a.testing(d.testing());
   a.display();   
   b.display();
   c.display();
   d.display();

/*
  // put your setup code here, to run once:
    Base a("a"); // a
    a.displayAll();
    Base b("b"); // a <- b
    b.displayAll();
    a.set_next(&b); // a <-> b
    a.displayAll();
    Base c("c"); // a <-> b, a <- c -> b
    c.set_prev(&a); // a <-> b
    c.set_next(&b); // a <-> b
    c.displayAll();
   
    a.set_next(&c); // a <- b, a <-> c -> b
    a.displayAll();
    b.set_prev(&c); // a <-> c <-> b
    b.displayAll();
    */
    
}

void loop() {
  // put your main code here, to run repeatedly:

}
