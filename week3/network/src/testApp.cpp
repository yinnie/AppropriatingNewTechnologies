 # i n c l u d e   " t e s t A p p . h " 
 
 v o i d   a d d F a c e ( o f M e s h &   m e s h ,   o f V e c 3 f   a ,   o f V e c 3 f   b ,   o f V e c 3 f   c )   { 
 	 m e s h . a d d V e r t e x ( a ) ; 
 	 m e s h . a d d V e r t e x ( b ) ; 
 	 m e s h . a d d V e r t e x ( c ) ; 
 } 
 
 v o i d   a d d F a c e ( o f M e s h &   m e s h ,   o f V e c 3 f   a ,   o f V e c 3 f   b ,   o f V e c 3 f   c ,   o f V e c 3 f   d )   { 
 	 a d d F a c e ( m e s h ,   a ,   b ,   c ) ; 
 	 a d d F a c e ( m e s h ,   a ,   c ,   d ) ; 
 	 / / a d d F a c e ( m e s h ,   b ,   c ,   d ) ; 
 } 
 
 o f V e c 3 f   g e t V e r t e x F r o m I m g ( o f I m a g e &   i m g ,   i n t   x ,   i n t   y )   { 
 	 o f C o l o r   c o l o r   =   i m g . g e t C o l o r ( x ,   y ) ; 
 	 i f ( c o l o r . a   >   2 0 0 )   {     / / a d j u s t   t o   g e t   a   g o o d   i m a g e 
 	 	 f l o a t   z   =   o f M a p ( c o l o r . a ,   2 0 0 ,   2 5 5 ,   - 4 8 0 ,   4 8 0 ) ;   / / m a p p i n g   a l p h a   t o   a   g o o d   r a n g e 
 	 	 r e t u r n   o f V e c 3 f ( x   -   i m g . g e t W i d t h ( )   / 2 ,   y   -   i m g . g e t H e i g h t ( )   /   2 ,   z ) ; 
 	 }   e l s e   { 
 	 	 r e t u r n   o f V e c 3 f ( 0 ,   0 ,   0 ) ; 
 	 } 
 } 
 
 
 v o i d   t e s t A p p : : s e t u p ( ) { 
 	 
 	 / / s e t   u p   i m g   m e s h 
 	 o f S e t V e r t i c a l S y n c ( t r u e ) ;   
 	 o f I m a g e   i m g ; 
 	 i m g . l o a d I m a g e ( " o u t 8 . p n g " ) ; 
 	 m e s h I m g . s e t M o d e ( O F _ P R I M I T I V E _ T R I A N G L E S ) ; 
 	 
 	 i n t   s k i p   =   3 ; 	 
 	 i n t   w i d t h   =   i m g . g e t W i d t h ( ) ; 
 	 i n t   h e i g h t   =   i m g . g e t H e i g h t ( ) ; 
 	 o f V e c 3 f   z e r o ( 0 ,   0 ,   0 ) ; 
 	 f o r ( i n t   y   =   0 ;   y   <   h e i g h t   -   s k i p ;   y   + =   s k i p )   { 
 	 	 f o r ( i n t   x   =   0 ;   x   <   w i d t h   -   s k i p ;   x   + =   s k i p )   { 
 	 	 	 o f V e c 3 f   n w   =   g e t V e r t e x F r o m I m g ( i m g ,   x ,   y ) ; 
 	 	 	 o f V e c 3 f   n e   =   g e t V e r t e x F r o m I m g ( i m g ,   x   +   s k i p ,   y ) ; 
 	 	 	 o f V e c 3 f   s w   =   g e t V e r t e x F r o m I m g ( i m g ,   x ,   y   +   s k i p ) ; 
 	 	 	 o f V e c 3 f   s e   =   g e t V e r t e x F r o m I m g ( i m g ,   x   +   s k i p ,   y   +   s k i p ) ; 
 	 	 	 i f ( n w   ! =   z e r o   & &   n e   ! =   z e r o   & &   s w   ! =   z e r o   & &   s e   ! =   z e r o )   { 
 	 	 	 	 a d d F a c e ( m e s h I m g ,   n w ,   n e ,   s e ,   s w ) ; 
 	 	 	 } 
 	 	 } 
 	 } 	 
 	 / /   e v e n   p o i n t s   c a n   o v e r l a p   w i t h   e a c h   o t h e r ,   l e t ' s   a v o i d   t h a t 
 	 g l E n a b l e ( G L _ D E P T H _ T E S T ) ; 
 	 
 	 
 	 / / - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
 	 / / s e t   u p   n e t w o r k   m e s h 
 	 m e s h N e t w o r k . s e t M o d e ( O F _ P R I M I T I V E _ P O I N T S ) ; 
 	 n u m P o i n t s = 1 0 0 ; 
 	 m o v e m e n t   =   o f V e c 3 f   ( 0 . 0 0 1 ,   0 . 0 0 1 ,   0 . 0 0 1 ) ; 
 	 n u m L o o p s   =   1 0 0 ; 
 	 c o u n t e r = 0 ; 
 	 
 	 p o i n t s . r e s i z e ( n u m P o i n t s ) ;     / / d o   t h i s   t o   i n i t i a t e   t h e   v e c t o r   a r r a y 
 	                                                       / / i f   y o u   d o n t   i n i t i a t e   t h e   s i z e   o f   t h e   a r r a y   y o u   m i g h t   g e t   B a d   A c c e s s   E r r o r 
 	 
 	 / / i n i t i a t e   t h e   p o i n t s   
 	 f o r   ( i n t   i = 0 ;   i <   n u m P o i n t s ;   i + + )   { 	 	 
 	 	 p o i n t s [ i ]   =   g e t V e r t e x F r o m I m g ( i m g ,   o f R a n d o m ( w i d t h ) ,   o f R a n d o m ( h e i g h t ) ) ; 
 	 } 
 	 / / c o u t   < <   o f T o S t r i n g ( p o i n t s )   < <   e n d l ; 
 } 
 
 
 v o i d   t e s t A p p : : u p d a t e ( ) { 
 	 
 	 i f   ( c o u n t e r < = 1 0 0 )   { 
 	 	 
 	 	 / / m o v e   a l l   p o i n t s   r a n d o m l y   a n d   s a v e   t h e s e   t o   n e w   p o s 
 	 	 f o r   ( i n t   i = 0 ;   i <   n u m P o i n t s ;   i + + )   { 
 	 	 	 f l o a t   n o i s e S c a l e = 1 0 ; 	 	 
 	 	 	 / / m o v e   a l l   t h e   p o i n t s   a   l i t t l e   b i t . . a n d   s a v e   t h e s e   p o s i t i o n s   f o r   g e n e r a t i n g   n e w   " n o i s e "   n e x t   l o o p 
 	 	 	 p o i n t s [ i ] + = m o v e m e n t   +     o f S i g n e d N o i s e ( p o i n t s [ i ] . x ,   p o i n t s [ i ] . y ,   p o i n t s [ i ] . z   ) * n o i s e S c a l e ; 
 	 	 } 
 	 	 
 	 	 / / f o r   e a c h   p o i n t ,   f i n d   t h e   c l o s e s t   p o i n t ,   a n d   c o n n e c t   t o   i t   
 	 	 f o r   ( i n t   i = 0 ;   i <   n u m P o i n t s ;   i + + )   { 	   
 	 	 	 / / f i n d   o u t   t h e   c l o s e t   p o i n t 
 	 	 	 f l o a t   s h o r t e s t D i s t ;       
 	 	 	 i n t   s h o r t e s t I n d e x ;     / / w e   o n l y   c a r e   a b o u t   t h e   i n d e x   o f   t h a t   p o i n t 
 	 	 	 f o r   ( i n t   j = i ;   j < n u m P o i n t s ;   j + + )   {     / / s t a r t i n g   f r o m   t h e   p o i n t   i n   c a s e   i t s e l f 
 	 	 	 	 f l o a t   d i s t a n c e   =   p o i n t s [ i ] . d i s t a n c e ( p o i n t s [ j ] ) ; 
 	 	 	 	 i f   ( j   = =   i   | |   d i s t a n c e   <   s h o r t e s t D i s t )   { 
 	 	 	 	 	 s h o r t e s t D i s t = d i s t a n c e ; 
 	 	 	 	 	 s h o r t e s t I n d e x = j ; 
 	 	 	 	 } 
 	 	 	 } 
 	 	 	 m e s h N e t w o r k . a d d V e r t e x ( p o i n t s [ i ] ) ; 
 	 	 	 m e s h N e t w o r k . a d d V e r t e x ( p o i n t s [ s h o r t e s t I n d e x ] ) ; 
 	 	 } 
 	 	 
 	 	 c o u n t e r + + ; 
 	 } 
 } 
 
 
 v o i d   t e s t A p p : : d r a w ( ) { 
 	 
 	 o f B a c k g r o u n d ( 0 ) ; 
 	 c a m . b e g i n ( ) ; 
 	 o f S c a l e ( 1 ,   - 1 ,   1 ) ;   / /   m a k e   y   p o i n t   d o w n 
 	 / / m e s h N e t w o r k . a d d C o l o r ( o f C o l o r   r e d ) ; 
 	 o f S e t C o l o r ( 2 5 5 ) ; 
 	 m e s h N e t w o r k . d r a w ( ) ; 
 	 / / c o u t   < <   o f T o S t r i n g ( m e s h N e t w o r k )   < <   e n d l ; 
 	 / / m e s h I m g . a d d C o l o r ( o f C o l o r   w h i t e ) ; 
 	 m e s h I m g . d r a w W i r e f r a m e ( ) ; 
 	 c a m . e n d ( ) ; 
 } 
