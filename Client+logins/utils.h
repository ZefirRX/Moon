#ifndef UTILS_H
#define UTILS_H

#include <QString>
inline bool isBlank(const QString &text)
{
    return text.trimmed().isEmpty();
}

#endif // UTILS_H
