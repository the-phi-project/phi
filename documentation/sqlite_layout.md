### Database Initialization Located In: phi/src/Database.cpp

---

## Table: self
| Col | Type | Other |
| :----: | :----: | :----: |
| id | PRIMARY KEY | ONLY ONE, SO ID CHECKED TO == 1 |
| rsa2048_pub | TEXT | N/A |
| rsa2048_priv | TEXT | N/A |
| rsa4096_pub | TEXT | N/A |
| rsa4096_priv | TEXT | N/A |
| kyber512_pub | TEXT | N/A |
| kyber512_priv | TEXT | N/A |
| kyber768_pub | TEXT | N/A |
| kyber768_priv | TEXT | N/A |

---

## Table: contacts
| Col | Type | Other |
| :----: | :----: | :----: |
| id | PRIMARY KEY | AUTOINCREMENT |
| name | TEXT | DEFAULT "Contact #<id>" |
| rsa2048 | TEXT | N/A |
| rsa4096 | TEXT | N/A |
| kyber512 | TEXT | N/A |
| kyber768 | TEXT | N/A |
```sql
UNIQUE(name)
```