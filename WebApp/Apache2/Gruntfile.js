module.exports = function(grunt) {

    grunt.initConfig({
        concat: {
            js: {
                src: [
                    'static/js/functions.js'
                ],
                dest: 'static/dist/js/scripts.js',
            },
        },
        uglify: {
            js: {
                files: {
                    'static/dist/js/scripts.min.js': ['static/dist/js/scripts.js'],
                    'static/dist/js/bootstrap.min.js': ['static/js/bootstrap.js']
                }
            },
        },
        cssmin: {
            css: {
                files: {
                    'static/dist/css/styles.min.css': ['static/css/styles.css'],
                    'static/dist/css/bootstrap.min.css': ['static/css/bootstrap.css'],
                    'static/dist/css/bootstrap-theme.min.css': ['static/css/bootstrap-theme.css']
                }
            }
        },
        watch: {
            js: {
                files: ['static/js/**/*.js'],
                tasks: [
                    'concat:js',
                    'uglify:js'
                ],
            },
            css: {
                files: ['static/css/**/*.css'],
                tasks: [
                    'cssmin:css'
                ],
            },
        }
    });

    grunt.loadNpmTasks('grunt-contrib-concat');
    grunt.loadNpmTasks('grunt-contrib-watch');

    grunt.loadNpmTasks('grunt-contrib-uglify');
    grunt.loadNpmTasks('grunt-contrib-cssmin');

}