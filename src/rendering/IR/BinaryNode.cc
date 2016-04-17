#include "llvm/IR/Value.h"
#include "llvm/IR/Type.h"

#include "ast/BinaryNode.h"
#include "ast/VariableNode.h"
#include "errors/Error.h"

#include "IRRenderer.h"


llvm::Value *
IRRenderer::render(BinaryNode *node) {
    if ( node->op == '=' ) {
        VariableNode *lhse = dynamic_cast<VariableNode*>(node->lhs);
        if ( ! lhse ) {
          return Error<llvm::Value>::handle("destination of '=' must be a variable");
        }

        llvm::Value *val = render(node->rhs);
        if ( val == 0 ) { return 0; }

        llvm::Value *variable = get_named_value(lhse->getName());
        if ( variable == 0 ) {
          return Error<llvm::Value>::handle("Unknown variable name");
        }

        builder->CreateStore(val, variable);

        return val;
    }

    llvm::Value *left = render(node->lhs);
    llvm::Value *right = render(node->rhs);

    if (left == 0 || right == 0 ) { return nullptr; }

    llvm::Type *llvm_double_type = llvm::Type::getDoubleTy(llvm_context());

    switch (node->op) {
    case '+': return builder->CreateFAdd(left, right, "addtmp");
    case '-': return builder->CreateFSub(left, right, "subtmp");
    case '*': return builder->CreateFMul(left, right, "multmp");
    case '<':
        left = builder->CreateFCmpULT(left, right, "cmptmp");
        return builder->CreateUIToFP(left,
                                                llvm_double_type,
                                                "booltmp");
    case '>':
        right = builder->CreateFCmpULT(right, left, "cmptmp");
        return builder->CreateUIToFP(right,
                                     llvm_double_type,
                                                "booltmp");
    default: break;
    }

    return Error<llvm::Value>::handle("Unknown binary operator!");
}
