#include <iostream>
#include <string>
#include <vector>
#include <memory>

// Strategy Pattern
class ProcessingStrategy {
public:
    virtual void process(const std::string& docType) = 0;
    virtual ~ProcessingStrategy() = default;
};

class PrintStrategy : public ProcessingStrategy {
public:
    void process(const std::string& docType) override {
        std::cout << "[Strategy] Printing " << docType << " document...\n";
    }
};

class SaveStrategy : public ProcessingStrategy {
public:
    void process(const std::string& docType) override {
        std::cout << "[Strategy] Saving " << docType << " document...\n";
    }
};

// Context for Strategy
class DocumentProcessor {
private:
    std::unique_ptr<ProcessingStrategy> strategy;
public:
    void setStrategy(std::unique_ptr<ProcessingStrategy> s) {
        strategy = std::move(s);
    }

    void executeStrategy(const std::string& docType) {
        if (strategy) strategy->process(docType);
        else std::cout << "No strategy selected.\n";
    }
};

// Chain of Responsibility
class Handler {
protected:
    std::shared_ptr<Handler> next;
public:
    void setNext(std::shared_ptr<Handler> n) {
        next = n;
    }
    virtual bool handle(const std::string& docType) = 0;
    virtual ~Handler() = default;
};

class FormatChecker : public Handler {
public:
    bool handle(const std::string& docType) override {
        std::cout << "[Chain] Checking format of " << docType << "...\n";
        if (docType == "PDF" || docType == "TXT" || docType == "DOCX") {
            if (next) return next->handle(docType);
            return true;
        }
        std::cout << "Format not supported.\n";
        return false;
    }
};

class SecurityChecker : public Handler {
public:
    bool handle(const std::string& docType) override {
        std::cout << "[Chain] Security check passed for " << docType << ".\n";
        if (next) return next->handle(docType);
        return true;
    }
};

// Visitor Pattern
class Visitor;

// Element
class Document {
public:
    virtual void accept(Visitor& v) = 0;
    virtual std::string getType() = 0;
    virtual ~Document() = default;
};

class PDFDocument : public Document {
public:
    void accept(Visitor& v) override;
    std::string getType() override { return "PDF"; }
};

class TXTDocument : public Document {
public:
    void accept(Visitor& v) override;
    std::string getType() override { return "TXT"; }
};

// Visitor
class Visitor {
public:
    virtual void visit(PDFDocument& doc) = 0;
    virtual void visit(TXTDocument& doc) = 0;
    virtual ~Visitor() = default;
};

// ConcreteVisitor
class DisplayVisitor : public Visitor {
public:
    void visit(PDFDocument& doc) override {
        std::cout << "[Visitor] Displaying PDF content.\n";
    }
    void visit(TXTDocument& doc) override {
        std::cout << "[Visitor] Displaying TXT content.\n";
    }
};

// Accept definitions
void PDFDocument::accept(Visitor& v) { v.visit(*this); }
void TXTDocument::accept(Visitor& v) { v.visit(*this); }

// Object Structure
class DocumentStructure {
private:
    std::vector<std::unique_ptr<Document>> docs;
public:
    void add(std::unique_ptr<Document> doc) {
        docs.push_back(std::move(doc));
    }

    void process(Visitor& v) {
        for (auto& doc : docs) {
            doc->accept(v);
        }
    }

    std::vector<Document*> getAll() {
        std::vector<Document*> result;
        for (auto& d : docs) result.push_back(d.get());
        return result;
    }
};

// --- Main Demo ---
int main() {
    // Chain of Responsibility
    std::shared_ptr<Handler> checker1 = std::make_shared<FormatChecker>();
    std::shared_ptr<Handler> checker2 = std::make_shared<SecurityChecker>();
    checker1->setNext(checker2);

    std::string docType = "PDF";

    if (checker1->handle(docType)) {
        // Strategy
        DocumentProcessor processor;
        processor.setStrategy(std::make_unique<PrintStrategy>());
        processor.executeStrategy(docType);
    }

    std::cout << "------------------------\n";

    // Visitor
    DocumentStructure structure;
    structure.add(std::make_unique<PDFDocument>());
    structure.add(std::make_unique<TXTDocument>());

    DisplayVisitor visitor;
    structure.process(visitor);

    std::cout << "\nНатисніть Enter для завершення...";
    std::cin.get(); // щоб вікно не закривалося одразу

    return 0;
}
