external.SetTitleRect(1,2,480,40);
$(function() {
   $('.min').click(function() {
       $(this).blur();
       external.MinimizeWindow();
   });

   $('.close').click(function()
   {
       if(!$(this).hasClass("disabled")) {
           external.Close();
       }
   });

   $('.mini_index').click(function() {
       $(this).blur();
       external.MinimizeWindow();
   });

   $('.close_index').click(function() {
      external.Close();
   });

   $('.load_mini').click(function() {
       $(this).blur();
       external.MinimizeWindow();
   });

   $('.three_mini').click(function() {
       $(this).blur();
       external.MinimizeWindow();
   });

   $('.four_mini').click(function() {
       $(this).blur();
       external.MinimizeWindow();
   });

   $('.error_mini').click(function() {
       $(this).blur();
       external.MinimizeWindow();
   });

   $('.error_close').click(function() {
       if(!$(this).hasClass("disable")) {
           external.Close();
       }
   });
 });
