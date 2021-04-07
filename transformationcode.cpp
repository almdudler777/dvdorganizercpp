    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("D:/Entw/Qt/dvdorganizer/database/data.sqlite");

    if(!db.open()){
        qDebug("No Connection to Database");
    }

    QSqlQuery qry(db);
    qry.exec("UPDATE dvds SET `titel` = replace(`titel`,\"\\'\",\"'\");");
    qry.exec("UPDATE dvds SET `cast` = replace(`cast`,\"\\'\",\"'\");");
    qry.exec("SELECT `cast`, `titel`, `id` FROM dvds WHERE trim(`cast`) != ''");

    QStringList castList;
    QString left, right, full;

    while(qry.next()){
        left.clear(); full.clear(); right.clear();
        castList.clear(); //empty the list, just to be sure...
        //qry.value holds all castmembers for a film in the form of:
        //Prename1 Name1, Prename2 Name2, ... , prenameN NameN therefor we split at the , char
        castList = qry.value(0).toString().trimmed().split(",", QString::SkipEmptyParts);

        for(int i = 0; i<castList.count(); i++){ //for every castmember 1 to N
            full = castList.value(i).trimmed(); //complete name as found betwen , ... , (e.g. Angus T. Jones, ...)
            left = full.left(full.lastIndexOf(" ")).trimmed(); //the guessed prename, currently also holds everything else
                                                               //like Angus T.
            right = full.right(full.size() - full.lastIndexOf(" ")).trimmed(); //the surname e.g. Jones

            //following is used to fill the actorlist, which will later be used to transfer the data into atomic sql tables
            if(actorList.contains(full)){ //if there is already one actor entry for this name
                QHash<QString, Actor>::iterator iter = actorList.find(full);
               iter.value().movieList.append(qry.value(2).toInt()); //append the movie id to the actors movielist
            }else{ //first appereance of this actor -> get a new item
                Actor tmp;
                tmp.movieList.append(qry.value(2).toInt()); //append the movie id to the actors movielist
                tmp.setName(right);
                tmp.setPreName(left);
                actorList.insert(full, tmp);
            }
        }
    }

    /* Just to show what the actorlist now looks like
    QHash<QString, Actor>::iterator iter = actorList.begin(); //get an iterator at the beginning of the actorlist
    //next part is to check the actorlist, just to see if this would fitt into the new sql tables
    QString movies;
    while (iter != actorList.end()) {
        for (int i = 0; i < iter.value().movieList.size(); i++){ //for every movie_id in the actors movielist
            qry.prepare("SELECT `titel` FROM dvds WHERE id = ? ");
            qry.addBindValue(iter.value().movieList.at(i));
            qry.exec();
            if(qry.next())
                movies += qry.value(0).toString() + ", ";
            qry.clear();
        }

        //next will output something like; to the debug output
        //"Chris" ; "Hemsworth" :  "[BluRay] Thor, [BluRay] Marvel's The Avengers, The Cabin in the Woods, Thor - The Dark Kingdom, "
        qDebug() << iter.value().getPreName() << ";" << iter.value().getName() << ": " << movies;
        ++iter;
        movies.clear();
    }*/

    //insert actors into actors table and save their id
    qry.exec("DELETE FROM actors;");
    qry.prepare("INSERT INTO actors (`name`, `prename`) VALUES ( ? , ? );");
    QHash<QString, Actor>::iterator iter = actorList.begin(); //get an iterator at the beginning of the actorlist
    while (iter != actorList.end()) {
        qry.addBindValue(iter.value().getName());
        qry.addBindValue(iter.value().getPreName());
        if(qry.exec())
            iter.value().setId(qry.lastInsertId().toInt());

        qDebug() << iter.value().toString();
        ++iter;
    }

    //fill the cast table to link actors to movies
    qry.exec("DELETE FROM cast;");
    qry.prepare("INSERT INTO cast (`actor_id`, `movie_id`) VALUES ( ? , ? );");
    iter = actorList.begin(); //get an iterator at the beginning of the actorlist
    while (iter != actorList.end()) {
        for(int i = 0; i < iter.value().movieList.size(); i++){
            qry.addBindValue(iter.value().getId());
            qry.addBindValue(iter.value().movieList.at(i));
            qry.exec();
        }
        ++iter;
    }




    QHash<QString, QList<int> > categoryList;
    QStringList tmpList;
    qry.exec("SELECT id, kategorie FROM dvds WHERE trim(`kategorie`) != ''");
    while(qry.next()){
        tmpList = qry.value(1).toString().trimmed().split(",", QString::SkipEmptyParts);
        for(int i = 0; i < tmpList.size(); i++){
            if(categoryList.contains(tmpList.at(i).trimmed())){ //if this category is already known
                categoryList.find(tmpList.at(i).trimmed()).value().append(qry.value(0).toInt());
            }else{
                QList<int> moviecategorylist;
                moviecategorylist.append(qry.value(0).toInt());
                categoryList.insert(tmpList.at(i).trimmed(),moviecategorylist);
            }
        }
        tmpList.clear();
    }

    QHash<QString, QList<int> >::iterator iter2 = categoryList.begin();
    QSqlQuery qryTags(db);
    int categoryId;
    qry.prepare("INSERT INTO category (`name`) VALUES ( ? )");
    qryTags.prepare("INSERT INTO TAGS (`movie_id`, `category_id`) VALUES ( ? , ? ) ");
    while(iter2 != categoryList.end()){
        qry.addBindValue(iter2.key().trimmed());
        if(qry.exec()){
            categoryId = qry.lastInsertId().toInt();
            for(int i=0; i<iter2.value().size(); i++){
                qryTags.addBindValue(iter2.value().at(i));
                qryTags.addBindValue(categoryId);
                if(!qryTags.exec()){
                    qDebug() << "mooeeep";
                    exit(12);
                }
            }
        }else{
            qDebug() << "anderes Moeep";
            exit(12);
        }
        iter2++;
    }

    /* just to show the categorylist
    QHash<QString, QList<int> >::iterator iter = categoryList.begin();
    QString movieIds;
    while(iter != categoryList.end()){
        movieIds.clear();
        for (int i = 0; i< iter.value().size(); i++){
            movieIds += QString::number(iter.value().at(i));
            movieIds += ",";
        }
        qDebug() << iter.key() << ": " << movieIds;
        iter++;
    }*/

    db.close();