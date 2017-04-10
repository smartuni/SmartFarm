module.exports = function (grunt) {
  grunt.initConfig({
    execute: {
      target: {
        src: ['users.js']
      }
    },
    watch: {
      scripts: {
        files: ['users.js'],
        tasks: ['execute'],
      },
    }
  });

  grunt.loadNpmTasks('grunt-contrib-watch');
  grunt.loadNpmTasks('grunt-execute');

};
