external.MainWnd.setWinTitleRect(0,0,480,40);
$(function() {
   $('.min').click(function() {
       $(this).blur();
       external.MainWnd.minimize();
   });

   $('.close').click(function()
   {
       if(!$(this).hasClass("disabled")) {
           external.MainWnd.close();
       }
   });

   $('.mini_index').click(function() {
       $(this).blur();
       external.MainWnd.minimize();
   });

   $('.close_index').click(function() {
      external.MainWnd.close();
   });

   $('.load_mini').click(function() {
       $(this).blur();
       external.MainWnd.minimize();
   });

   $('.three_mini').click(function() {
       $(this).blur();
       external.MainWnd.minimize();
   });

   $('.four_mini').click(function() {
       $(this).blur();
       external.MainWnd.minimize();
   });

   $('.error_mini').click(function() {
       $(this).blur();
       external.MainWnd.minimize();
   });

   $('.error_close').click(function() {
       if(!$(this).hasClass("disable")) {
           external.MainWnd.close();
       }
   });
 });
