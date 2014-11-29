
struct Face{
  int vertices[3];
  int normals[3];

  //Assume user initializes all themselves
  //Default constructor doesn't make sense and is a waste of time
  //Face(){
  //  for (int i = 0; i < 3; i++){
  //    vertices[i] = 0;
  //    normals[i] = 0;
  //  }
  //}
};