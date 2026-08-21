# LLD: User Permission & Account Roles (Customer, Seller, Admin, Moderator)

> **Core Category:** Dynamic State & Entity Evolution / Runtime Role Changes  
> **Key Design Pattern:** Role-Based Access Control (RBAC) / Dynamic Role Object & Policy Pattern  
> **Target Concepts:** Runtime Role Mutation, Avoiding Account Migration/Object Copying, Fine-Grained Authorization, Single Responsibility Principle (SRP)

---

## 1. The Interview Question

> *"In an e-commerce platform, a user starts as a regular `Buyer` (can browse, add to cart, purchase). Later, they register as a `Seller` (can list products, manage inventory) while retaining their buyer history. The platform can also grant them temporary `Moderator` or `Admin` privileges (ban abusive reviews, view platform metrics). How do you architect this so users dynamically acquire and drop roles without creating new user accounts, copying order history, or breaking database references?"*

---

## 2. The Naive Diagrams (What NOT to Draw on Paper)

### ❌ Anti-Pattern 1: Concrete User Subclassing (Rigid Static Types)

```
                            +--------------------+
                            |        User        |  (userId, email, passwordHash, orders)
                            +--------------------+
                               /        |       \
                              /         |        \
                +---------------+ +-----------+ +---------------+
                |   BuyerUser   | |SellerUser | |   AdminUser   |
                +---------------+ +-----------+ +---------------+
                               \       /
                                \     /
                          +---------------+
                          |  BuyerSeller  |  <-- Deadly Diamond & Explosion!
                          +---------------+
```

#### Why the interviewer rejects this:
1. **The Account Migration Catastrophe:** When a `Buyer` becomes a `Seller`, you cannot mutate a C++ object from `BuyerUser` to `SellerUser`. You would have to **instantiate a new `SellerUser` object, deep-copy all past order histories, and update every foreign-key reference in orders, reviews, and payment logs**!
2. **Loss of Buyer Features:** If you migrate the user to `SellerUser`, how do they buy items from other sellers? Do they need two separate accounts (`BuyerSeller`)?
3. **Temporary Privilege Impossibility:** If an Admin grants a user 24-hour moderator rights to review flagged products, you cannot temporarily change their class type and revert it cleanly.

---

### ❌ Anti-Pattern 2: God User Class with Bitmask/Booleans

```
+-------------------------------------------------------------------------+
|                                  User                                   |
+-------------------------------------------------------------------------+
| - isBuyer: bool                                                         |
| - isSeller: bool                                                        |
| - isAdmin: bool                                                         |
| - sellerGstNumber: string                                               |
| - adminSecurityBadgeId: string                                          |
| + listProduct(...): void -> if (!isSeller) throw NotAuthorized!        |
| + banUser(...): void    -> if (!isAdmin) throw NotAuthorized!           |
+-------------------------------------------------------------------------+
```

#### Why the interviewer rejects this:
- Violates **Single Responsibility Principle (SRP)** and **Open-Closed Principle (OCP)**: Every new role (e.g. `Affiliate`, `Auditor`, `SupportAgent`) bloats the `User` class with nullable fields and conditional checks.

---

## 3. The Winning Whiteboard Architecture

### ✅ Core Insight: *"A User Account is a single, permanent entity holding authentication & order history. Roles are dynamic, pluggable capabilities (`IUserRole`) attached to the account."*

```
+-----------------------------------------------------------------------------------+
|                                    UserAccount                                    |
+-----------------------------------------------------------------------------------+
| - userId: string                                                                  |
| - email: string                                                                   |
| - orderHistory: vector<OrderId>                                                   |
| - activeRoles: unordered_map<RoleType, shared_ptr<IUserRole>>                     |
+-----------------------------------------------------------------------------------+
| + grantRole(role: shared_ptr<IUserRole>): void                                    |
| + revokeRole(type: RoleType): void                                                |
| + hasPermission(perm: Permission): bool                                           |
| + getRole<T>(type: RoleType): shared_ptr<T>                                       |
| + executeAction(action: IUserAction): void                                        |
+-----------------------------------------------------------------------------------+
                                         |
                                         | 1 HAS MANY (0..*)
                                         v
                      +--------------------------------------+
                      |            <<interface>>             |
                      |              IUserRole               |
                      +--------------------------------------+
                      | + getRoleType()*: RoleType           |
                      | + hasPermission(perm)*: bool         |
                      | + getPermissions()*: set<Permission> |
                      +--------------------------------------+
                                         ^
                                         | implements
         +-------------------------------+-------------------------------+
         |                               |                               |
+-----------------+             +-----------------+             +-----------------+
|    BuyerRole    |             |   SellerRole    |             |  ModeratorRole  |
+-----------------+             +-----------------+             +-----------------+
| - savedAddresses|             | - storeName     |             | - assignedQueue |
| - paymentMethods|             | - bankAccount   |             | - badgeLevel    |
+-----------------+             +-----------------+             +-----------------+
| + placeOrder()  |             | + addListing()  |             | + reviewReport()|
| + writeReview() |             | + fulfillOrder()|             | + banContent()  |
+-----------------+             +-----------------+             +-----------------+
```

---

## 4. Concise Whiteboard Code (What to Write on Paper)

```cpp
// 1. Fine-Grained Permissions
enum class Permission {
    BROWSE_CATALOG, PLACE_ORDER, POST_REVIEW,
    LIST_PRODUCT, MANAGE_INVENTORY,
    DELETE_REVIEW, BAN_USER, VIEW_METRICS
};

enum class RoleType { BUYER, SELLER, MODERATOR, ADMIN };

// 2. Pure Role Interface
class IUserRole {
public:
    virtual ~IUserRole() = default;
    virtual RoleType getRoleType() const = 0;
    virtual bool hasPermission(Permission p) const = 0;
};

// 3. Concrete Role Implementations
class BuyerRole : public IUserRole {
public:
    RoleType getRoleType() const override { return RoleType::BUYER; }
    bool hasPermission(Permission p) const override {
        return p == Permission::BROWSE_CATALOG || p == Permission::PLACE_ORDER || p == Permission::POST_REVIEW;
    }
};

class SellerRole : public IUserRole {
    std::string storeName;
    std::string gstNumber;
public:
    SellerRole(std::string store, std::string gst) : storeName(std::move(store)), gstNumber(std::move(gst)) {}
    RoleType getRoleType() const override { return RoleType::SELLER; }
    bool hasPermission(Permission p) const override {
        return p == Permission::LIST_PRODUCT || p == Permission::MANAGE_INVENTORY;
    }
};

// 4. Stable User Account Entity (Dynamic Role Hub)
class UserAccount {
private:
    std::string userId;
    std::string email;
    std::unordered_map<RoleType, std::shared_ptr<IUserRole>> roles;

public:
    UserAccount(std::string id, std::string email) : userId(std::move(id)), email(std::move(email)) {
        // Every new account defaults to regular Buyer
        grantRole(std::make_shared<BuyerRole>());
    }

    // Dynamic Runtime Role Attachment
    void grantRole(std::shared_ptr<IUserRole> role) {
        roles[role->getRoleType()] = std::move(role);
    }
    void revokeRole(RoleType type) { roles.erase(type); }

    // Centralized Permission Checker (Aggregates across all active roles)
    bool hasPermission(Permission p) const {
        for (const auto& [type, role] : roles) {
            if (role->hasPermission(p)) return true;
        }
        return false;
    }
};
```

---

## 5. The 3-Minute Verbal Pitch (What to Say to the Interviewer)

> 1. *"If we model `Buyer`, `Seller`, and `Admin` as separate subclasses of `User`, we create a critical architectural defect: When a buyer registers as a seller, we would have to migrate their database entity, copy their order history, and reassign foreign keys."*
> 2. *"Putting boolean flags (`isBuyer`, `isSeller`, `isAdmin`) inside a single monolithic `User` class violates the **Single Responsibility Principle (SRP)** and creates an unmaintainable god object."*
> 3. *"Instead, I decouple the persistent identity (`UserAccount`) from dynamic privileges using **Role-Based Access Control (RBAC) via the Role Object Pattern**."*
> 4. *"A `UserAccount` holds a collection of active `IUserRole` objects. A user starts with `BuyerRole`. When they open a store, we attach `SellerRole` with $O(1)$ runtime cost. When granted temporary moderator access, we attach `ModeratorRole` with zero data copying."*
> 5. *"Authorization checks are centralized: Calling `account->hasPermission(Permission::LIST_PRODUCT)` iterates through active roles and returns `true` if any role grants the privilege."*

---

## 6. Common Interviewer Follow-Ups & Quick Answers

| Follow-up Question | Recommended Response |
| :--- | :--- |
| **"How do you implement temporary role expiration (e.g. Admin for 2 hours)?"** | Wrap the role in a `TimedRoleDecorator` holding an `expiryTimestamp` that revokes permissions once `now() > expiryTimestamp`. |
| **"What if two roles have conflicting permissions (e.g. Deny overrides Allow)?"** | Introduce a Policy Engine that checks explicit **Deny Rules** before aggregating **Allow Rules** (Policy Pattern). |
| **"How do you secure access to Seller-specific data (e.g. Bank Account details)?"** | Expose a type-safe getter `account->getRole<SellerRole>(RoleType::SELLER)` that returns `nullptr` if the user lacks the role. |
