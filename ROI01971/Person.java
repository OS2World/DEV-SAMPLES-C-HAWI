import java.lang.System ;

import Adresse ;

class Person
   {
   private int        Adresse ;
   private int        Nummer ;
   private String     Name ;
   private String     Vorname ;
   static private int Anzahl = 0 ;

   public static void main (String args [])
      {
      Adresse adresse1 = new Adresse ("Adresse 1", "Bezeichnung") ;
      Person  person1 = new Person (adresse1, "Person 1", "") ;
      adresse1.show () ;
      person1.show () ;
      }

   public int Adresse ()
      {
      return Adresse ;
      }

   public int Anzahl ()
      {
      return Anzahl ;
      }

   public String Name ()
      {
      return Name ;
      }

   public int Nummer ()
      {
      return Nummer ;
      }

   public Person (Adresse adresse)
      {
      Adresse = adresse.Nummer () ;
      Nummer = ++Anzahl ;
      Name = "" ;
      Vorname = "" ;
      }

   public Person (Adresse adresse, String Name, String Vorname)
      {
      Adresse = adresse.Nummer () ;
      Nummer = ++Anzahl ;
      this.Name = Name ;
      this.Vorname = Vorname ;
      }

   public void setName (String Name)
      {
      this.Name = Name ;
      }

   public void setVorname (String Vorname)
      {
      this.Vorname = Vorname ;
      }

   public void show ()
      {
      System.out.println ("Adresse: "+Adresse) ;
      System.out.println ("Anzahl : "+Anzahl) ;
      System.out.println ("Nummer : "+Nummer) ;
      System.out.println ("Name   : "+Name) ;
      System.out.println ("Vorname: "+Vorname) ;
      }

   public String Vorname ()
      {
      return Vorname ;
      }
   }

