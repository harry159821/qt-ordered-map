#ifndef ORDEREDMAP_H
#define ORDEREDMAP_H

#include <QHash>
#include <QLinkedList>
#include <QList>
#include <QPair>

template <typename Key> inline bool oMHashEqualToKey(const Key &key1, const Key &key2)
{
    // Key type must provide '==' operator
    return key1 == key2;
}

template <typename Ptr> inline bool oMHashEqualToKey(Ptr *key1, Ptr *key2)
{
    Q_ASSERT(sizeof(quintptr) == sizeof(Ptr *));
    return quintptr(key1) == quintptr(key2);
}

template <typename Ptr> inline bool oMHashEqualToKey(const Ptr *key1, const Ptr *key2)
{
    Q_ASSERT(sizeof(quintptr) == sizeof(const Ptr *));
    return quintptr(key1) == quintptr(key2);
}

template<typename Key, typename OMHashValue>
class OMHash : public QHash < Key, OMHashValue >
{
public:
	bool operator == (const OMHash &other) const
	{
		if (size() != other.size()) {
			return false;
		}

		if (QHash<Key, OMHashValue >::operator ==(other)) {
			return true;
		}

		typename QHash<Key, OMHashValue >::const_iterator it1 = this->constBegin();
		typename QHash<Key, OMHashValue >::const_iterator it2 = other.constBegin();

		while (it1 != this->end()) {
			OMHashValue v1 = it1.value();
			OMHashValue v2 = it2.value();

			if ((v1.first != v2.first) || !oMHashEqualToKey<Key>(it1.key(), it2.key())) {
				return false;
			}
			++it1;
			++it2;
		}
		return true;
	}
};

template <typename Key, typename Value>
class OrderedMap
{
    //class OMHash;

    typedef typename QLinkedList<Key>::iterator QllIterator;
    typedef typename QLinkedList<Key>::const_iterator QllConstIterator;
    typedef QPair<Value, QllIterator> OMHashValue;

	typedef typename OMHash<Key, OMHashValue>::iterator OMHashIterator;
	typedef typename OMHash<Key, OMHashValue>::const_iterator OMHashConstIterator;

public:

    class iterator;
    class const_iterator;

    typedef typename OrderedMap<Key, Value>::iterator Iterator;
    typedef typename OrderedMap<Key, Value>::const_iterator ConstIterator;

    explicit OrderedMap();

    void clear();

    bool contains(const Key &key) const;

    int count() const;

    bool empty() const;

    iterator insert(const Key &key, const Value &value);

    bool isEmpty() const;

	QLinkedList<Key> keys() const;

    int remove(const Key &key);

    int size() const;

#if QT_VERSION >= 0x040800
    void swap(OrderedMap<Key, Value> &other);
#endif

    Value take(const Key &key);

    Value value(const Key &key) const;

    Value value(const Key &key, const Value &defaultValue) const;

    QList<Value> values() const;

    OrderedMap<Key, Value> & operator=(OrderedMap<Key, Value> other);

    bool operator==(const OrderedMap<Key, Value> &other) const;

    bool operator!=(const OrderedMap<Key, Value> &other) const;

    Value& operator[](const Key &key);

    const Value operator[](const Key &key) const;

    iterator begin();

    const_iterator begin() const;

    iterator end();

    const_iterator end() const;

    iterator erase(iterator pos);

    iterator find(const Key& key);

    const_iterator find(const Key& key) const;

    class iterator
    {
    public:

        QllIterator qllIter;
		OMHash<Key, OMHashValue> *data;

        iterator() : data(NULL) {}

		iterator(const QllIterator &qllIter, OMHash<Key, OMHashValue> *data) :
            qllIter(qllIter), data(data) {}

        const Key & key() const
        {
            return *qllIter;
        }

        Value & value() const
        {
            OMHashIterator hit = data->find(*qllIter);
            OMHashValue &pair = hit.value();
            return pair.first;
        }

        Value & operator*() const
        {
            return value();
        }

        iterator operator+(int i) const
        {
            QllIterator q = qllIter;
            q += i;

            return iterator(q, data);
        }

        iterator operator-(int i) const
        {
            return operator +(- i);
        }

        iterator& operator+=(int i)
        {
            qllIter += i;
            return *this;
        }

        iterator& operator-=(int i)
        {
            qllIter -= i;
            return *this;
        }

        iterator& operator++()
        {
            ++qllIter;
            return *this;
        }

        iterator operator++(int)
        {
            iterator it = *this;
            qllIter++;
            return it;
        }

        iterator operator--()
        {
            --qllIter;
            return *this;
        }

        iterator operator--(int)
        {
            iterator it = *this;
            qllIter--;
            return it;
        }

        bool operator ==(const iterator &other) const
        {
            return (qllIter == other.qllIter);
        }

        bool operator !=(const iterator &other) const
        {
            return (qllIter != other.qllIter);
        }
    };

    class const_iterator
    {
    public:

        QllConstIterator qllConstIter;
        const OMHash *data;

        const_iterator() : data(NULL) {}

        const_iterator(const iterator &i) :
            qllConstIter(i.qllIter), data(i.data) {}

        const_iterator(const QllConstIterator &qllConstIter, const OMHash* data) :
            qllConstIter(qllConstIter), data(data) {}

        const Key & key() const
        {
            return *qllConstIter;
        }

        const Value & value() const
        {
            OMHashConstIterator hit = data->find(*qllConstIter);
            const OMHashValue &pair = hit.value();
            return pair.first;
        }

        const Value & operator*() const
        {
            return value();
        }

        const_iterator operator+(int i) const
        {
            QllConstIterator q = qllConstIter;
            q += i;

            return const_iterator(q, data);
        }

        const_iterator operator-(int i) const
        {
            return operator +(- i);
        }

        const_iterator& operator+=(int i)
        {
            qllConstIter += i;
            return *this;
        }

        const_iterator& operator-=(int i)
        {
            qllConstIter -= i;
            return *this;
        }

        const_iterator& operator++()
        {
            ++qllConstIter;
            return *this;
        }

        const_iterator operator++(int)
        {
            const_iterator it = *this;
            qllConstIter++;
            return it;
        }

        const_iterator operator--()
        {
            --qllConstIter;
            return *this;
        }

        const_iterator operator--(int)
        {
            const_iterator it = *this;
            qllConstIter--;
            return it;
        }

        bool operator ==(const const_iterator &other) const
        {
            return (qllConstIter == other.qllConstIter);
        }

        bool operator !=(const const_iterator &other) const
        {
            return (qllConstIter != other.qllConstIter);
        }
    };

private:

	OMHash<Key, OMHashValue> data;
    QLinkedList<Key> insertOrder;
};

template <typename Key, typename Value>
OrderedMap<Key, Value>::OrderedMap() {}

template <typename Key, typename Value>
void OrderedMap<Key, Value>::clear()
{
    data.clear();
    insertOrder.clear();
}

template <typename Key, typename Value>
bool OrderedMap<Key, Value>::contains(const Key &key) const
{
    return data.contains(key);
}

template <typename Key, typename Value>
int OrderedMap<Key, Value>::count() const
{
    return data.count();
}

template <typename Key, typename Value>
bool OrderedMap<Key, Value>::empty() const
{
    return data.empty();
}

template <typename Key, typename Value>
typename OrderedMap<Key, Value>::iterator OrderedMap<Key, Value>::insert(const Key &key, const Value &value)
{
    OMHashIterator it = data.find(key);

    if (it == data.end()) {
        // New key
        QllIterator ioIter = insertOrder.insert(insertOrder.end(), key);
        OMHashValue pair(value, ioIter);
        data.insert(key, pair);
        return iterator(ioIter, &data);
    }

    OMHashValue& pair = it.value();//2.Get Real Value IN Hash

    //remove old reference
    //insertOrder.erase(pair.second);//insertOrder(QLinkedList), remove old LinkedList
	
	//Add new reference
	//QllIterator ioIter = insertOrder.insert(insertOrder.end(), key);

	//Find Old Reference
	//QllIterator ioIter = qFind(insertOrder.begin(), insertOrder.end(), key);	
    pair.first = value;
    //pair.second = ioIter;
	return iterator(pair.second, &data);
}

template <typename Key, typename Value>
bool OrderedMap<Key, Value>::isEmpty() const
{
    return data.isEmpty();
}

template<typename Key, typename Value>
QLinkedList<Key> OrderedMap<Key, Value>::keys() const
{
	//QList<Key> keyOrder;
    return insertOrder;
}

template<typename Key, typename Value>
int OrderedMap<Key, Value>::remove(const Key &key)
{
    OMHashIterator it = data.find(key);
    if (it == data.end()) {
        return 0;
    }
    OMHashValue pair = it.value();
    insertOrder.erase(pair.second);
    data.erase(it);
    return 1;
}

template<typename Key, typename Value>
int OrderedMap<Key, Value>::size() const
{
    return data.size();
}

#if QT_VERSION >= 0x040800
template<typename Key, typename Value>
void OrderedMap<Key, Value>::swap(OrderedMap<Key, Value> &other)
{
    // Swap individual components
    data.swap(other.data);
    insertOrder.swap(other.insertOrder);
}
#endif

template<typename Key, typename Value>
Value OrderedMap<Key, Value>::take(const Key &key)
{
    OMHashIterator it = data.find(key);
    if (it == data.end()) {
        return Value();
    }
    OMHashValue pair = it.value();
    insertOrder.erase(pair.second);
    data.erase(it);
    return pair.first;
}

template <typename Key, typename Value>
Value OrderedMap<Key, Value>::value(const Key &key) const
{
    return data.value(key).first;
}

template <typename Key, typename Value>
Value OrderedMap<Key, Value>::value(const Key &key, const Value &defaultValue) const
{
    OMHashConstIterator it = data.constFind(key);
    if (it == data.end()) {
        return defaultValue;
    }
    OMHashValue pair = it.value();
    return pair.first;
}

template <typename Key, typename Value>
QList<Value> OrderedMap<Key, Value>::values() const
{
    QList<Value> values;
    foreach (const Key &key, insertOrder.toStdList()) {
        OMHashValue v = data.value(key);
        values.append(v.first);
    }
    return values;
}

template <typename Key, typename Value>
OrderedMap<Key, Value> & OrderedMap<Key, Value>::operator=(OrderedMap<Key, Value> other)
{
#if QT_VERSION >= 0x040800
    swap(other);
#else
    data = other.data;
    insertOrder = other.insertOrder;
#endif
    return *this;
}

template <typename Key, typename Value>
bool OrderedMap<Key, Value>::operator==(const OrderedMap<Key, Value> &other) const
{
    // 2 Ordered maps are equal if they have the same contents in the same order
    return ((data == other.data) && (insertOrder == other.insertOrder));
}

template <typename Key, typename Value>
bool OrderedMap<Key, Value>::operator!=(const OrderedMap<Key, Value> &other) const
{
    return ((data != other.data) || (insertOrder != other.insertOrder));
}

template <typename Key, typename Value>
Value& OrderedMap<Key, Value>::operator[](const Key &key)
{
    OMHashIterator it = data.find(key);
    if (it == data.end()) {
        insert(key, Value());
        it = data.find(key);
    }
    OMHashValue &pair = it.value();
    return pair.first;
}

template <typename Key, typename Value>
const Value OrderedMap<Key, Value>::operator[](const Key &key) const
{
    return value(key);
}

template <typename Key, typename Value>
typename OrderedMap<Key, Value>::iterator OrderedMap<Key, Value>::begin()
{
    return iterator(insertOrder.begin(), &data);
}

template <typename Key, typename Value>
typename OrderedMap<Key, Value>::const_iterator OrderedMap<Key, Value>::begin() const
{
    return const_iterator(insertOrder.begin(), &data);
}


template <typename Key, typename Value>
typename OrderedMap<Key, Value>::iterator OrderedMap<Key, Value>::end()
{
    return iterator(insertOrder.end(), &data);
}

template <typename Key, typename Value>
typename OrderedMap<Key, Value>::const_iterator OrderedMap<Key, Value>::end() const
{
    return const_iterator(insertOrder.end(), &data);
}

template <typename Key, typename Value>
typename OrderedMap<Key, Value>::iterator OrderedMap<Key, Value>::erase(iterator pos)
{
    OMHashIterator hit = data.find(*(pos.qllIter));
    if (hit == data.end()) {
        return pos;
    }
    data.erase(hit);
    QllIterator ioIter = insertOrder.erase(pos.qllIter);

    return iterator(ioIter, &data);
}

template <typename Key, typename Value>
typename OrderedMap<Key, Value>::iterator OrderedMap<Key, Value>::find(const Key& key)
{
    OMHashIterator hit = data.find(key);
    if (hit == data.end()) {
        return end();
    }

    return iterator(hit.value().second, &data);
}

template <typename Key, typename Value>
typename OrderedMap<Key, Value>::const_iterator OrderedMap<Key, Value>::find(const Key& key) const
{
    OMHashConstIterator hit = data.find(key);
    if (hit == data.end()) {
        return end();
    }

    return const_iterator(hit.value().second, &data);
}

#endif // ORDEREDMAP_H
