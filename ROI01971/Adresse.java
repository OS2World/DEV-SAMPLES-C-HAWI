import java.lang.System ;

public class Adresse
   {
   private int        Nummer ;
   private String     Bezeichnung1 ;
   private String     Bezeichnung2 ;
   static private int Anzahl = 0 ;

   public static void main (String args [])
      {
      Adresse adresse1 = new Adresse () ;
      adresse1.Bezeichnung1 = "Adresse 1" ;
      adresse1.Bezeichnung2 = "Bezeichnung" ;
      adresse1.show () ;
      Adresse adresse2 = new Adresse ("Adresse 2", "Bezeichnung") ;
      adresse2.show () ;
      }

   public Adresse ()
      {
      Nummer = ++Anzahl ;
      Bezeichnung1 = "" ;
      Bezeichnung2 = "" ;
      }

   public Adresse (String Bezeichnung1, String Bezeichnung2)
      {
      Nummer = ++Anzahl ;
      this.Bezeichnung1 = Bezeichnung1 ;
      this.Bezeichnung2 = Bezeichnung2 ;
      }

   public int Anzahl ()
      {
      return Anzahl ;
      }

   public String Bezeichnung1 ()
      {
      return Bezeichnung1 ;
      }

   public String Bezeichnung2 ()
      {
      return Bezeichnung2 ;
      }

   public int Nummer ()
      {
      return Nummer ;
      }

   public void setBezeichnung1 (String Bezeichnung1)
      {
      this.Bezeichnung1 = Bezeichnung1 ;
      }

   public void setBezeichnung2 (String Bezeichnung2)
      {
      this.Bezeichnung2 = Bezeichnung2 ;
      }

   public void show ()
      {
      System.out.println ("Anzahl      : "+Anzahl) ;
      System.out.println ("Nummer      : "+Nummer) ;
      System.out.println ("Bezeichnung1: "+Bezeichnung1) ;
      System.out.println ("Bezeichnung2: "+Bezeichnung2) ;
      }
   }

