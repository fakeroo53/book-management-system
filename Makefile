.PHONY: help build clean test run install lint format check-format dev-setup all

# Variables
PROJECT_NAME = book-management-system
PYTHON = python3
PIP = pip3
VENV_DIR = venv
SRC_DIR = src
TESTS_DIR = tests
BUILD_DIR = build
DIST_DIR = dist

# Color output
CYAN = \033[0;36m
GREEN = \033[0;32m
YELLOW = \033[0;33m
RED = \033[0;31m
NC = \033[0m # No Color

##@ Help
help: ## Display this help screen
	@echo "$(CYAN)$(PROJECT_NAME) - Makefile Commands$(NC)"
	@echo ""
	@awk 'BEGIN {FS = ":.*##"; printf "$(YELLOW)Usage:$(NC)\n  make $(CYAN)<target>$(NC)\n\n$(YELLOW)Targets:$(NC)\n"} /^[a-zA-Z_-]+:.*?##/ { printf "  $(CYAN)%-15s$(NC) %s\n", $$1, $$2 } /^##@/ { printf "\n$(YELLOW)%s$(NC)\n", substr($$0, 5) } ' $(MAKEFILE_LIST)

##@ Setup & Installation
install: ## Install project dependencies
	@echo "$(GREEN)Installing dependencies...$(NC)"
	$(PIP) install -r requirements.txt

dev-setup: ## Setup development environment with all tools
	@echo "$(GREEN)Setting up development environment...$(NC)"
	$(PIP) install -r requirements.txt
	$(PIP) install -r requirements-dev.txt
	@echo "$(GREEN)Development environment ready!$(NC)"

venv: ## Create virtual environment
	@echo "$(GREEN)Creating virtual environment...$(NC)"
	$(PYTHON) -m venv $(VENV_DIR)
	@echo "$(GREEN)Virtual environment created. Activate with: source $(VENV_DIR)/bin/activate$(NC)"

##@ Build & Compilation
build: clean ## Build the project
	@echo "$(GREEN)Building project...$(NC)"
	$(PYTHON) setup.py build
	@echo "$(GREEN)Build complete!$(NC)"

dist: clean ## Create distribution packages
	@echo "$(GREEN)Creating distribution packages...$(NC)"
	$(PYTHON) setup.py sdist bdist_wheel
	@echo "$(GREEN)Distribution packages created in $(DIST_DIR)/$(NC)"

##@ Code Quality
lint: ## Run code linting (pylint)
	@echo "$(YELLOW)Running linter...$(NC)"
	$(PYTHON) -m pylint $(SRC_DIR) --disable=all --enable=E,F || true
	@echo "$(GREEN)Linting complete!$(NC)"

format: ## Format code with black and isort
	@echo "$(YELLOW)Formatting code...$(NC)"
	$(PYTHON) -m black $(SRC_DIR) $(TESTS_DIR)
	$(PYTHON) -m isort $(SRC_DIR) $(TESTS_DIR)
	@echo "$(GREEN)Code formatted!$(NC)"

check-format: ## Check code format without changes
	@echo "$(YELLOW)Checking code format...$(NC)"
	$(PYTHON) -m black --check $(SRC_DIR) $(TESTS_DIR)
	$(PYTHON) -m isort --check-only $(SRC_DIR) $(TESTS_DIR)
	@echo "$(GREEN)Format check complete!$(NC)"

##@ Testing
test: ## Run all tests
	@echo "$(YELLOW)Running tests...$(NC)"
	$(PYTHON) -m pytest $(TESTS_DIR) -v
	@echo "$(GREEN)Tests complete!$(NC)"

test-coverage: ## Run tests with coverage report
	@echo "$(YELLOW)Running tests with coverage...$(NC)"
	$(PYTHON) -m pytest $(TESTS_DIR) --cov=$(SRC_DIR) --cov-report=html --cov-report=term
	@echo "$(GREEN)Coverage report generated!$(NC)"

test-quick: ## Run quick smoke tests
	@echo "$(YELLOW)Running quick tests...$(NC)"
	$(PYTHON) -m pytest $(TESTS_DIR) -q --tb=short
	@echo "$(GREEN)Quick tests complete!$(NC)"

##@ Running
run: ## Run the application
	@echo "$(GREEN)Starting application...$(NC)"
	$(PYTHON) -m $(SRC_DIR).main

debug: ## Run application in debug mode
	@echo "$(GREEN)Starting application in debug mode...$(NC)"
	$(PYTHON) -m $(SRC_DIR).main --debug

##@ Database
db-init: ## Initialize database
	@echo "$(GREEN)Initializing database...$(NC)"
	$(PYTHON) -m $(SRC_DIR).db init
	@echo "$(GREEN)Database initialized!$(NC)"

db-migrate: ## Run database migrations
	@echo "$(GREEN)Running database migrations...$(NC)"
	$(PYTHON) -m $(SRC_DIR).db migrate
	@echo "$(GREEN)Migrations complete!$(NC)"

db-reset: ## Reset database (warning: destructive)
	@echo "$(RED)WARNING: This will reset the database!$(NC)"
	@read -p "Are you sure? [y/N] " -n 1 -r; \
	echo; \
	if [[ $$REPLY =~ ^[Yy]$$ ]]; then \
		$(PYTHON) -m $(SRC_DIR).db reset; \
		echo "$(GREEN)Database reset!$(NC)"; \
	else \
		echo "$(YELLOW)Database reset cancelled.$(NC)"; \
	fi

##@ Cleaning
clean: ## Remove build artifacts and cache files
	@echo "$(YELLOW)Cleaning build artifacts...$(NC)"
	rm -rf $(BUILD_DIR) $(DIST_DIR)
	rm -rf .pytest_cache .coverage htmlcov
	find . -type d -name __pycache__ -exec rm -rf {} + 2>/dev/null || true
	find . -type f -name "*.pyc" -delete
	find . -type f -name "*.pyo" -delete
	find . -type f -name ".DS_Store" -delete
	@echo "$(GREEN)Clean complete!$(NC)"

clean-all: clean ## Remove everything including virtual environment
	@echo "$(YELLOW)Removing virtual environment...$(NC)"
	rm -rf $(VENV_DIR)
	@echo "$(GREEN)Full clean complete!$(NC)"

##@ Utilities
docs: ## Generate documentation
	@echo "$(GREEN)Generating documentation...$(NC)"
	$(PYTHON) -m sphinx -b html docs docs/_build/html
	@echo "$(GREEN)Documentation generated in docs/_build/html/$(NC)"

version: ## Display project version
	@echo "$(CYAN)$(PROJECT_NAME) version:$(NC)"
	@$(PYTHON) -c "import sys; sys.path.insert(0, '$(SRC_DIR)'); from __init__ import __version__; print(__version__)" 2>/dev/null || echo "Version not found"

requirements: ## Show all installed packages
	$(PIP) list

freeze: ## Freeze current dependencies to requirements.txt
	@echo "$(GREEN)Freezing dependencies...$(NC)"
	$(PIP) freeze > requirements.txt
	@echo "$(GREEN)Dependencies frozen to requirements.txt$(NC)"

##@ Combined Targets
check: check-format lint test ## Run all checks (format, lint, tests)
	@echo "$(GREEN)All checks passed!$(NC)"

all: install test build ## Build everything
	@echo "$(GREEN)Build successful!$(NC)"

.DEFAULT_GOAL := help
