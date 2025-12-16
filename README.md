# Book Management System

A comprehensive application for managing your personal or institutional book collection. This system provides an intuitive interface to catalog, organize, and track books with detailed metadata and advanced search capabilities.

## Table of Contents

- [Features](#features)
- [Getting Started](#getting-started)
  - [Prerequisites](#prerequisites)
  - [Installation](#installation)
  - [Configuration](#configuration)
- [Usage](#usage)
  - [Basic Operations](#basic-operations)
  - [Advanced Features](#advanced-features)
- [Project Structure](#project-structure)
- [API Reference](#api-reference)
- [Database Schema](#database-schema)
- [Contributing](#contributing)
- [License](#license)
- [Support](#support)

## Features

### Core Functionality
- **Book Management**: Add, edit, delete, and view books in your collection
- **Advanced Search**: Search books by title, author, ISBN, genre, and more
- **Categorization**: Organize books by genre, reading status, and custom categories
- **Reading Tracking**: Monitor reading progress and maintain reading lists
- **Review System**: Add personal reviews and ratings to books
- **Export/Import**: Backup and restore your book collection

### User Management
- User authentication and authorization
- Personal book collections and preferences
- Reading history and statistics
- User profiles and settings

### Analytics & Reporting
- Collection statistics and insights
- Reading habits analysis
- Genre distribution charts
- Annual reading goals and tracking

## Getting Started

### Prerequisites

Before you begin, ensure you have the following installed:

- **Node.js** (v14.0.0 or higher)
- **npm** or **yarn** (v6.0.0 or higher)
- **Database**: MongoDB (v4.4+) or PostgreSQL (v12+)
- **Git** (for version control)

### Installation

1. **Clone the repository**
   ```bash
   git clone https://github.com/fakeroo53/book-management-system.git
   cd book-management-system
   ```

2. **Install dependencies**
   ```bash
   npm install
   # or
   yarn install
   ```

3. **Create environment variables file**
   ```bash
   cp .env.example .env
   ```

4. **Update .env file with your configuration**
   ```env
   NODE_ENV=development
   PORT=3000
   DATABASE_URL=mongodb://localhost:27017/bookmanagement
   JWT_SECRET=your-secret-key-here
   API_KEY=your-api-key-here
   ```

5. **Initialize the database**
   ```bash
   npm run db:init
   # or
   npm run db:migrate
   ```

6. **Start the application**
   ```bash
   npm start
   # For development with hot reload
   npm run dev
   ```

The application will be available at `http://localhost:3000`

### Configuration

#### Environment Variables

| Variable | Description | Default |
|----------|-------------|---------|
| `NODE_ENV` | Application environment | `development` |
| `PORT` | Server port | `3000` |
| `DATABASE_URL` | Database connection string | `mongodb://localhost:27017/bookmanagement` |
| `JWT_SECRET` | JWT signing secret | Required |
| `API_KEY` | API authentication key | Optional |
| `LOG_LEVEL` | Logging level | `info` |

## Usage

### Basic Operations

#### Adding a Book

```bash
POST /api/books
Content-Type: application/json

{
  "title": "The Great Gatsby",
  "author": "F. Scott Fitzgerald",
  "isbn": "978-0743273565",
  "genre": "Fiction",
  "publicationYear": 1925,
  "pages": 180,
  "coverUrl": "https://example.com/cover.jpg",
  "description": "A novel of the Jazz Age..."
}
```

#### Searching for Books

```bash
GET /api/books/search?query=gatsby&author=fitzgerald&genre=fiction
```

#### Updating a Book

```bash
PUT /api/books/:id
Content-Type: application/json

{
  "title": "The Great Gatsby",
  "rating": 5,
  "status": "completed"
}
```

#### Deleting a Book

```bash
DELETE /api/books/:id
```

### Advanced Features

#### Reading List Management

Create and manage reading lists:

```bash
POST /api/reading-lists
Content-Type: application/json

{
  "name": "Summer Reading 2024",
  "description": "Books to read during summer",
  "books": ["bookId1", "bookId2", "bookId3"]
}
```

#### Personal Reviews

Add detailed reviews and ratings:

```bash
POST /api/books/:id/reviews
Content-Type: application/json

{
  "rating": 4.5,
  "review": "An excellent novel with compelling characters...",
  "dateRead": "2024-12-16"
}
```

#### Statistics & Analytics

```bash
GET /api/statistics/summary
GET /api/statistics/by-genre
GET /api/statistics/reading-timeline
```

## Project Structure

```
book-management-system/
├── src/
│   ├── api/
│   │   ├── controllers/
│   │   │   ├── bookController.js
│   │   │   ├── userController.js
│   │   │   └── reviewController.js
│   │   ├── routes/
│   │   │   ├── books.js
│   │   │   ├── users.js
│   │   │   └── reviews.js
│   │   ├── middleware/
│   │   │   ├── auth.js
│   │   │   ├── validation.js
│   │   │   └── errorHandler.js
│   │   └── validators/
│   │       └── bookValidator.js
│   ├── models/
│   │   ├── Book.js
│   │   ├── User.js
│   │   ├── Review.js
│   │   └── ReadingList.js
│   ├── services/
│   │   ├── bookService.js
│   │   ├── userService.js
│   │   └── analyticsService.js
│   ├── config/
│   │   ├── database.js
│   │   ├── auth.js
│   │   └── logger.js
│   └── app.js
├── tests/
│   ├── unit/
│   ├── integration/
│   └── fixtures/
├── migrations/
├── .env.example
├── .gitignore
├── package.json
└── README.md
```

## API Reference

### Books Endpoints

| Method | Endpoint | Description |
|--------|----------|-------------|
| GET | `/api/books` | Get all books with pagination |
| GET | `/api/books/:id` | Get a specific book |
| POST | `/api/books` | Create a new book |
| PUT | `/api/books/:id` | Update a book |
| DELETE | `/api/books/:id` | Delete a book |
| GET | `/api/books/search` | Search books |

### Users Endpoints

| Method | Endpoint | Description |
|--------|----------|-------------|
| POST | `/api/auth/register` | Register a new user |
| POST | `/api/auth/login` | Login a user |
| GET | `/api/users/profile` | Get user profile |
| PUT | `/api/users/profile` | Update user profile |
| POST | `/api/auth/logout` | Logout a user |

### Reviews Endpoints

| Method | Endpoint | Description |
|--------|----------|-------------|
| POST | `/api/books/:id/reviews` | Add a review |
| GET | `/api/books/:id/reviews` | Get book reviews |
| PUT | `/api/reviews/:id` | Update a review |
| DELETE | `/api/reviews/:id` | Delete a review |

## Database Schema

### Books Collection

```javascript
{
  _id: ObjectId,
  title: String,
  author: String,
  isbn: String,
  genre: String,
  publicationYear: Number,
  pages: Number,
  coverUrl: String,
  description: String,
  rating: Number,
  status: String, // "unread", "reading", "completed"
  dateAdded: Date,
  dateUpdated: Date,
  userId: ObjectId
}
```

### Users Collection

```javascript
{
  _id: ObjectId,
  username: String,
  email: String,
  password: String (hashed),
  firstName: String,
  lastName: String,
  avatar: String,
  bio: String,
  createdAt: Date,
  updatedAt: Date
}
```

### Reviews Collection

```javascript
{
  _id: ObjectId,
  bookId: ObjectId,
  userId: ObjectId,
  rating: Number,
  review: String,
  dateRead: Date,
  dateCreated: Date,
  dateUpdated: Date
}
```

## Contributing

We welcome contributions! Please follow these steps:

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

### Development Guidelines

- Follow the existing code style and conventions
- Write unit tests for new features
- Ensure all tests pass before submitting a PR
- Update documentation as needed
- Use meaningful commit messages

### Running Tests

```bash
# Run all tests
npm test

# Run tests with coverage
npm run test:coverage

# Run tests in watch mode
npm run test:watch
```

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Support

For support, please:

- Check the [Issues](https://github.com/fakeroo53/book-management-system/issues) section
- Create a new issue with detailed information
- Contact the maintainers directly

### Frequently Asked Questions (FAQ)

**Q: How do I reset my password?**
A: Use the password reset feature on the login page or contact support.

**Q: Can I export my book collection?**
A: Yes, use the export feature in your account settings. Formats supported: CSV, JSON, PDF.

**Q: Is there a mobile app?**
A: Currently web-based only. Mobile apps are in development.

**Q: How do I backup my data?**
A: The system automatically backs up data daily. You can also manually export your collection.

---

**Last Updated**: December 16, 2024

**Current Version**: 1.0.0

For more information, visit our [website](https://example.com) or [documentation](https://docs.example.com)
