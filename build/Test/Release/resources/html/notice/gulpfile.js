var gulp = require('gulp');
var less = require('gulp-less');
var path = require('path');
var concat = require('gulp-concat');
var uglify = require('gulp-uglify');
var minifyCss = require('gulp-minify-css');
var rename=require("gulp-rename");
var clean = require("gulp-clean");
var replace = require("gulp-replace");
var paths = {
    lesses: ['./theme/css/*.less'],
    csses: ['./theme/css/*.css'],
    scripts: ['js/app.js','js/controllers/**/*.js','js/directives/**/*.js','js/services/**/*.js','js/filters/**/*.js'],
    copyFiles:['thirdparty/*.*','theme/**/*.*','!theme/css/*.less','templates/*.*','nls/**/*.*']
};
gulp.task('script',function(){
   gulp.src(paths.scripts)
  .pipe(concat('all.js'))
  .pipe(rename('all.min.js'))
  .pipe(uglify())
  .pipe(gulp.dest('build'));
});

gulp.task('minApp',function(){
  return gulp.src('js/app.js')
  .pipe(uglify())
  .pipe(gulp.dest('build/js'));
});

gulp.task('copy',function(){
  return gulp.src(paths.copyFiles,{ base: '.' })
  .pipe(gulp.dest('build'));
});

gulp.task('clean',function(){
  return gulp.src('build/*',{read:false})
  .pipe(clean())
  .end();
});

gulp.task('replace',function(){
  return gulp.src('*.html')
  .pipe(replace(/<!-- replace start-->[\s\S]+<!-- replace end-->/g,'<script src="all.min.js"></script>'))
  .pipe(gulp.dest('build'));
});

gulp.task('less', function() {
    return gulp.src(paths.lesses)
        .pipe(less({
            paths: [path.join(__dirname, 'less', 'includes')]
        }))
        .pipe(gulp.dest('./theme/css'));
});

gulp.task('minify-css', function() {
    return gulp.src(paths.csses)
        .pipe(minifyCss({
            compatibility: 'ie8'
        }))
        .pipe(gulp.dest('theme/css'));
});

gulp.task('default', ['clean','script','replace','minify-css','copy'], function() {});

gulp.task('dev', ['less'], function() {
    gulp.watch(paths.lesses, ['less']);
});

gulp.task('release', ['dev'], function() {});
gulp.task('r', ['release'], function() {});
