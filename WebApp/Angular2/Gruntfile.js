module.exports = function(grunt) {

    grunt.initConfig({
        concat: {
            js: {
                src: [
                    'src/static/js/functions.js'
                ],
                dest: 'src/static/dist/js/scripts.js',
            },
            css: {
                src: [
                    'src/static/css/style.css'
                ],
                dest: 'src/static/dist/css/styles.css',
            },
        },
        uglify: {
            js: {
                files: {
                    'src/static/dist/js/scripts.min.js': ['src/static/dist/js/scripts.js']
                }
            },
        },
        cssmin: {
            css: {
                files: {
                    'src/static/dist/css/styles.min.css': ['src/static/dist/css/styles.css']
                }
            }
        },
        watch: {
            js: {
                files: ['src/static/js/**/*.js'],
                tasks: [
                    'concat:js',
                    'uglify:js'
                ],
            },
            css: {
                files: ['src/static/css/**/*.css'],
                tasks: [
                    'concat:css',
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