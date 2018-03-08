import java.awt.* ;

public class VAdresse extends Frame
   {
   private String    temp ;
   private Menu      datei ;
   private MenuBar   menubar  ;
   private TextField tname ;
   private TextField tvorname ;
   private TextField tstrasse ;

   public static void main (String args [])
      {
      VAdresse vadresse = new VAdresse () ;
      }

   public boolean handleEvent (Event event)
      {
      switch (event.id)
         {
         case Event.ACTION_EVENT:
              if   (event.target instanceof Button)
                   {
                   if   ("Ok".equals (event.arg))
                        {
                        temp = tname.getText () ;
                        tname.setText (tvorname.getText ()) ;
                        tvorname.setText (tstrasse.getText ()) ;
                        tstrasse.setText (temp) ;
                        return true ;
                        }
                   }
              if   (event.target instanceof MenuItem)
                   {
                   if   ("Verlassen".equals (event.arg))
                        {
                        System.exit (0) ;
                        return true ;
                        }
                   }
              return true ;
         case Event.WINDOW_DESTROY:
              System.exit (0) ;
              return true ;
         default:
              return false ;
         }
      }

   public VAdresse ()
      {
      super ("VAdresse") ;
      menubar = new MenuBar () ;
      datei = new Menu ("Datei") ;
      datei.add (new MenuItem ("Verlassen")) ;
      menubar.add (datei) ;
      setMenuBar (menubar) ;
      setLayout (new GridLayout (3, 1)) ;
      Panel panels [][] = new Panel [3][1] ;
      panels[0][0] = new Panel () ;
      panels[0][0].setLayout (new FlowLayout (FlowLayout.LEFT)) ;
      panels[1][0] = new Panel () ;
      panels[1][0].setLayout (new GridLayout (3, 2)) ;
      panels[2][0] = new Panel () ;
      panels[2][0].setLayout (new FlowLayout (FlowLayout.LEFT)) ;
      panels[0][0].add (new Button ("1")) ;
      panels[0][0].add (new Button ("2")) ;
      panels[0][0].add (new Button ("3")) ;
      panels[2][0].add (new Button ("Ok")) ;
      panels[2][0].add (new Button ("Verlassen")) ;
      panels[2][0].add (new Button ("Hilfe")) ;
      Panel centerpanels [][] = new Panel [3][2] ;
      centerpanels[0][0] = new Panel () ;
      centerpanels[0][1] = new Panel () ;
      centerpanels[1][0] = new Panel () ;
      centerpanels[1][1] = new Panel () ;
      centerpanels[2][0] = new Panel () ;
      centerpanels[2][1] = new Panel () ;
      panels[1][0].add (centerpanels [0][0]) ;
      panels[1][0].add (centerpanels [0][1]) ;
      panels[1][0].add (centerpanels [1][0]) ;
      panels[1][0].add (centerpanels [1][1]) ;
      panels[1][0].add (centerpanels [2][0]) ;
      panels[1][0].add (centerpanels [2][1]) ;
      centerpanels[0][0].add (new Label ("Name", Label.LEFT)) ;
      centerpanels[1][0].add (new Label ("Vorname", Label.LEFT)) ;
      centerpanels[2][0].add (new Label ("Strasse", Label.LEFT)) ;
      tname = new TextField (30) ;
      tvorname = new TextField (30) ;
      tstrasse = new TextField (30) ;
      centerpanels[0][1].add (tname) ;
      centerpanels[1][1].add (tvorname) ;
      centerpanels[2][1].add (tstrasse) ;
      add (panels [0][0]) ;
      add (panels [1][0]) ;
      add (panels [2][0]) ;
      pack () ;
      resize (400, 400) ;
      show () ;
      }
   }

