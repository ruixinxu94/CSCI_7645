#include <stdio.h>

/**
* print myfolder
 *          subfolder1
 *          `   sdf
 *              sdf
 *          subflolder2
 *              dfs
 *              sdf
 *              there is also tab include '\t'
 *
 *              showContents(dirct, depth) , initially the depth would be 1 for default
 *              {
 *                  folder = open(direct);
 *                  nextEntry = read(folder);
 *                  while (nextEntry != null) {
 *                      printout(n  = depths of the diectory)
 *                      print(n tabs + folder);
 *                      if (filter is a directory) {
 *                          showContents(direct, depth+1);
 *                      }
 *                      nextEntry = read(direct);
 *                  }
 *              }
*/


