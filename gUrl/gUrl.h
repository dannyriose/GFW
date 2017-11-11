#ifndef GURL_H
#define GURL_H
#include <gString/gString.h>

namespace gfw{
class SHARED_GFW gUrl:public gBaseShared{
public:
    gUrl();
    gUrl(const gUrl &other);
    gUrl(const gString &url);
    ~gUrl();
    gUrl &operator = (const gUrl &other);
    //! Sets the full url
    void setUrl(const gString &url);
    //! Returns host i.e. example.com
    const gString &host() const;
    //! Returns main domain i.e. blog.example.com returns example.com
    const gString &domain() const;
    //! Returns how many subdomains the url contains..i.e. blog.example.com has 1 subdomain blog
    gu32 subDomainCount() const;
    //! Returns an specified subdomain
    const gString &subDomain(gu32 index) const;
    //! Returns the path of the url
    const gString &path(gu32 index) const;
    //! Gets the path element count
    gu32 pathCount() const;
    //! Returns if contains a path element with a given name
    bool containsPath(const gString &name, gu32 *indexOut = 0) const;
    //! Gets the absolute path
    const gString &absPath() const;
    //! Returns the scheme
    const gString &scheme() const;
    //! Returns the query value
    const gString &query(const gString &key) const;
    //! Returns true if contains a query with a given value
    bool containsQuery(const gString &key) const;
    //! Returns the fragment
    const gString &fragment() const;
    //! Gets the entire url
    const gString &url() const;
    //! The user if any
    const gString &user() const;
    //! The password if any
    const gString &password() const;
    //! The port
    gs32 port() const;

    //! @todo add more functionality
};
}
#endif // GURL_H
