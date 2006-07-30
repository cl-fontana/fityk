// This file is part of fityk program. Copyright (C) Marcin Wojdyr
// $Id$

#ifndef FITYK__CALC__H__
#define FITYK__CALC__H__

#include "common.h"

struct OpTree;
class Variable;


class AnyFormula
{
public:
    AnyFormula(fp &value_, std::vector<fp>& derivatives_) 
        : value(value_), derivatives(derivatives_) {}
    AnyFormula(std::vector<OpTree*> const &op_trees_, 
               fp &value_, std::vector<fp>& derivatives_) 
        : value(value_), derivatives(derivatives_), op_trees(op_trees_) {}
    /// (re-)create bytecode, required after ::set_var_idx()
    void tree_to_bytecode(std::vector<int> const& var_idx); 
    void run_vm(std::vector<Variable*> const &variables) const; 
    std::vector<OpTree*> const& get_op_trees() const { return op_trees; }

protected:
    // these are recalculated every time parameters or variables are changed
    mutable fp &value; 
    mutable std::vector<fp> &derivatives; 

    std::vector<OpTree*> op_trees; 
    std::vector<int> vmcode; //OP_PUT_DERIV, OP_PUT_VAL, OP_VAR, OP_SIN, etc. 
    std::vector<fp> vmdata; 

    void exec_vm_op_action(std::vector<int>::const_iterator &i,
                           std::vector<double>::iterator &stackPtr) const;
};

class AnyFormulaO : public AnyFormula
{
public:
    AnyFormulaO(std::vector<OpTree*> const &op_trees_, 
               fp &value_, std::vector<fp>& derivatives_) 
        : AnyFormula(op_trees_, value_, derivatives_) {}
    void tree_to_bytecode(std::vector<int> const& var_idx); 
    void prepare_optimized_codes(std::vector<Variable*> const &variables);
    fp run_vm_val(fp x) const;
    void run_vm_der(fp x) const { run_vm(vmcode_der, x); }
    std::string get_vmcode_info() const;
private:
    int vmdata_size;
    std::vector<int> vmcode_val;
    std::vector<int> vmcode_der;
    void run_vm(std::vector<int> const& code, fp x) const;
};

#endif