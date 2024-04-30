#Component: dependenc_example
#Prototye of a component for development of C dependency manager
name = "dependenc_example"
version = "1.0.0"

#test
require = "depend1/1.0.0@github.com/Flight020/dependenc1.git", #This is an inline comment
          "depend2/0.1.0@github.com/Flight020/dependenc2.git" #This is another inline comment

