#ifndef ROLES_H
#define ROLES_H

#include <Qt>

namespace UserRoles {
enum Roles{
    MovieId = Qt::UserRole +1,
    ActorId = Qt::UserRole +2,
    CategoryId = Qt::UserRole + 3
};
}


namespace WindowSelectionRoles{
enum Roles{
    Actor = 1,
    Movie = 0,
    Category = 2
};
}


#endif // ROLES_H
