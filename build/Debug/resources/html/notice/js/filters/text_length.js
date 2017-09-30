appModule.filter('textLength',function(){
  return function(input){
    input = input || '';
    return input.length;
  };
});
